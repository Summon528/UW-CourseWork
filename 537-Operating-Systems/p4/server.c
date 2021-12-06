#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "mfs.h"
#include "udp.h"

#define WRITE(fd, buf, len)           \
    if (write(fd, buf, len) != len) { \
        perror("write");              \
        assert(0);                    \
    }

#define WRITEEND(fd, buf, len)        \
    lseek(fd, 0, SEEK_END);           \
    if (write(fd, buf, len) != len) { \
        perror("write");              \
        assert(0);                    \
    }                                 \
    cp->end += len;

#define READ(fd, buf, len)           \
    if (read(fd, buf, len) != len) { \
        perror("read");              \
        assert(0);                   \
    }

#define BUFFER_SIZE (1000)
#define MAX_NAME (28)
#define MAX_INODE (4096)
#define MAX_DPTR (14)
#define INODE_IN_PIECE (16)
#define MAX_PIECE (MAX_INODE / INODE_IN_PIECE)

typedef struct {
    int end;
    int inode_map_ptrs[MAX_PIECE];
} Checkpoint_t;

typedef struct {
    int inode_ptrs[INODE_IN_PIECE];
} Inodemap_t;

typedef struct Inode_t {
    uint8_t type;
    int size;
    int dptrs[MAX_DPTR];
} Inode_t;

typedef struct {
    char name[MAX_NAME];
    int inum;
} Dent_t;

Checkpoint_t* cp;
int fd;

void initfs() {
    cp->inode_map_ptrs[0] = sizeof(Checkpoint_t);
    for (int i = 1; i < MAX_PIECE; i++) {
        cp->inode_map_ptrs[i] = cp->inode_map_ptrs[i - 1] + sizeof(Inodemap_t);
    }
    int root_off = cp->inode_map_ptrs[MAX_PIECE - 1] + sizeof(Inodemap_t);
    cp->end = root_off + sizeof(Inode_t) + sizeof(Dent_t) * 2;
    WRITE(fd, cp, sizeof(Checkpoint_t));
    Inodemap_t* im = malloc(sizeof(Inodemap_t));
    memset(im->inode_ptrs, -1, sizeof(im->inode_ptrs));
    im->inode_ptrs[0] = root_off;
    WRITE(fd, im, sizeof(Inodemap_t));
    im->inode_ptrs[0] = -1;
    for (int i = 1; i < MAX_PIECE; i++) {
        WRITE(fd, im, sizeof(Inodemap_t));
    }
    free(im);

    Inode_t* root = malloc(sizeof(Inode_t));
    memset(root, -1, sizeof(Inode_t));
    root->type = MFS_DIRECTORY;
    root->size = 0;
    root->dptrs[0] = root_off + sizeof(Inode_t);
    root->dptrs[1] = root_off + sizeof(Inode_t) + sizeof(Dent_t);
    WRITE(fd, root, sizeof(Inode_t));
    free(root);

    Dent_t* de = calloc(1, sizeof(Dent_t));
    de->name[0] = '.';
    de->inum = 0;
    WRITE(fd, de, sizeof(Dent_t));

    de->name[1] = '.';
    WRITE(fd, de, sizeof(Dent_t));
    free(de);
    fsync(fd);
}

void syncfs() {
    lseek(fd, 0, SEEK_SET);
    WRITE(fd, cp, sizeof(Checkpoint_t));
    fsync(fd);
}

Inode_t* finode(int inum) {
    if (inum < 0 && inum >= MAX_INODE) return NULL;
    int off = cp->inode_map_ptrs[inum / INODE_IN_PIECE];
    lseek(fd, off, SEEK_SET);
    Inodemap_t im;
    READ(fd, &im, sizeof(Inodemap_t));
    int idx = inum % INODE_IN_PIECE;
    int off2 = im.inode_ptrs[idx];
    if (off2 == -1) return NULL;
    lseek(fd, off2, SEEK_SET);
    Inode_t* inode = malloc(sizeof(Inode_t));
    READ(fd, inode, sizeof(Inode_t));
    return inode;
}

void commit_inode(int inum, Inode_t* inode) {
    int off = cp->inode_map_ptrs[inum / INODE_IN_PIECE];
    lseek(fd, off, SEEK_SET);
    Inodemap_t im;
    READ(fd, &im, sizeof(Inodemap_t));

    int idx = inum % INODE_IN_PIECE;
    if (inode) {
        im.inode_ptrs[idx] = cp->end;
        WRITEEND(fd, inode, sizeof(Inode_t));
    } else {
        im.inode_ptrs[idx] = -1;
    }

    cp->inode_map_ptrs[inum / INODE_IN_PIECE] = cp->end;
    WRITEEND(fd, &im, sizeof(Inodemap_t));
}

int findfree() {
    for (int i = 0; i < MAX_PIECE; i++) {
        int off = cp->inode_map_ptrs[i];
        lseek(fd, off, SEEK_SET);
        Inodemap_t im;
        READ(fd, &im, sizeof(Inodemap_t));
        for (int j = 0; j < INODE_IN_PIECE; j++) {
            if (im.inode_ptrs[j] == -1) {
                return i * INODE_IN_PIECE + j;
            }
        }
    }
    return -1;
}

int MFS_Lookup(int pinum, char* name) {
    Inode_t* inode = finode(pinum);
    if (inode == NULL) return -1;
    assert(inode->type == MFS_DIRECTORY);
    Dent_t de;
    for (int i = 0; inode->dptrs[i] != -1; i++) {
        lseek(fd, inode->dptrs[i], SEEK_SET);
        READ(fd, &de, sizeof(Dent_t));
        if (strcmp(name, de.name) == 0) {
            free(inode);
            return de.inum;
        }
    }
    free(inode);
    return -1;
}

int MFS_Stat(int inum, MFS_Stat_t* stat) {
    Inode_t* inode = finode(inum);
    if (inode == NULL) return -1;
    stat->type = inode->type;
    stat->size = inode->size;
    free(inode);
    return 0;
}

int MFS_Creat(int pinum, int type, char* name) {
    if (strlen(name) > MAX_NAME) return -1;

    Inode_t* pinode = finode(pinum);
    int free_inum = findfree();
    assert(free_inum != -1);

    Inode_t* cinode = malloc(sizeof(Inode_t));
    memset(cinode, -1, sizeof(Inode_t));
    cinode->type = type;
    cinode->size = 0;
    if (type == MFS_DIRECTORY) {
        cinode->dptrs[0] = cp->end;
        Dent_t tmpde;
        memset(&tmpde, 0, sizeof(Dent_t));
        tmpde.name[0] = '.';
        tmpde.inum = free_inum;
        WRITEEND(fd, &tmpde, sizeof(Dent_t));

        cinode->dptrs[1] = cp->end;
        tmpde.name[1] = '.';
        tmpde.inum = pinum;
        WRITEEND(fd, &tmpde, sizeof(Dent_t));
    }
    commit_inode(free_inum, cinode);

    int didx = 0;
    while (pinode->dptrs[didx] != -1) {
                didx++;
    }

    pinode->dptrs[didx] = cp->end;

    Dent_t de;
    memset(&de, 0, sizeof(Dent_t));
    de.inum = free_inum;
    strncpy(de.name, name, MAX_NAME);
    WRITEEND(fd, &de, sizeof(Dent_t));

    commit_inode(pinum, pinode);

    free(pinode);
    free(cinode);
    syncfs();
    return 0;
}

int MFS_Write(int inum, char* buffer, int block) {
    if (block < 0 || block >= MAX_DPTR) return -1;
    Inode_t* inode = finode(inum);
    if (inode == NULL || inode->type == MFS_DIRECTORY) return -1;
    inode->dptrs[block] = cp->end;
    WRITEEND(fd, buffer, MFS_BLOCK_SIZE);
    commit_inode(inum, inode);
    free(inode);
    syncfs();
    return 0;
}

int MFS_Read(int inum, char* buffer, int block) {
    if (block < 0 || block >= MAX_DPTR) return -1;
    Inode_t* inode = finode(inum);
    if (inode == NULL || inode->dptrs[block] == -1) return -1;
    lseek(fd, inode->dptrs[block], SEEK_SET);
    READ(fd, buffer, MFS_BLOCK_SIZE);
    return 0;
}

int MFS_Unlink(int pinum, char* name) {
    Inode_t* inode = finode(pinum);
    if (inode->type != MFS_DIRECTORY) return -1;
    Dent_t de;
    int foundinum = -1;
    for (int i = 0; i < MAX_DPTR; i++) {
        if (inode->dptrs[i] == -1) continue;
        lseek(fd, inode->dptrs[i], SEEK_SET);
        READ(fd, &de, sizeof(Dent_t));
        if (strncmp(de.name, name, MAX_NAME) == 0) {
            foundinum = de.inum;
            inode->dptrs[i] = -1;
            commit_inode(pinum, inode);
            break;
        }
    }
    if (foundinum == -1) return 0;
    commit_inode(foundinum, NULL);
    syncfs();
}

int MFS_Shutdown() {
    syncfs();
    exit(0);
}

int main(int argc, char* argv[]) {
    assert(argc == 3);
    int port = atoi(argv[1]);
    char* img = argv[2];
    cp = malloc(sizeof(Checkpoint_t));
    if ((fd = open(img, O_CREAT | O_RDWR | O_EXCL, 0644)) >= 0) {
        initfs();
    } else if (errno = EEXIST) {
        fd = open(img, O_RDWR);
        READ(fd, cp, sizeof(Checkpoint_t));
    } else {
        perror("open");
        assert(0);
    }

    Inode_t* inode = finode(0);
    printf("%d\n", inode->size);
    printf("%d\n", inode->type);
    printf("%x\n", inode->dptrs[0]);
    printf("%x\n", inode->dptrs[1]);

    printf("%d\n", MFS_Lookup(0, "."));
    MFS_Stat_t stat;
    MFS_Stat(0, &stat);
    printf("%d\n", stat.type);
    printf("%d\n", stat.size);
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "test"));

    printf("%d\n", MFS_Creat(0, MFS_DIRECTORY, "test2"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "test3"));
    char s[] = "hello world!";
    printf("%d\n", MFS_Write(1, s, 2));
    char buf[4096];
    printf("%d\n", MFS_Read(1, buf, 2));
    printf("%s\n", buf);
    printf("%d\n", MFS_Unlink(0, "test"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "apple"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "banaa"));
    // printf("%d\n", MFS_Lookup(0, "."));
    // int sd = UDP_Open(10000);
    // assert(sd > -1);
    // while (1) {
    //     struct sockaddr_in addr;
    //     char message[BUFFER_SIZE];
    //     printf("server:: waiting...\n");
    //     int rc = UDP_Read(sd, &addr, message, BUFFER_SIZE);
    //     printf("server:: read message [size:%d contents:(%s)]\n", rc,
    //     message); if (rc > 0) {
    //         char reply[BUFFER_SIZE];
    //         sprintf(reply, "goodbye world");
    //         rc = UDP_Write(sd, &addr, reply, BUFFER_SIZE);
    //         printf("server:: reply\n");
    //     }
    // }
    return 0;
}
