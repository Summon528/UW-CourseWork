#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mfs.h"

int UDP_Open(int port) {
    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        return 0;
    }

    // set up the bind
    struct sockaddr_in my_addr;
    bzero(&my_addr, sizeof(my_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        close(fd);
        return -1;
    }

    return fd;
}

int UDP_Write(int fd, struct sockaddr_in* addr, char* buffer, int n) {
    int addr_len = sizeof(struct sockaddr_in);
    int rc = sendto(fd, buffer, n, 0, (struct sockaddr*)addr, addr_len);
    return rc;
}

int UDP_Read(int fd, struct sockaddr_in* addr, char* buffer, int n) {
    int len = sizeof(struct sockaddr_in);
    int rc =
        recvfrom(fd, buffer, n, 0, (struct sockaddr*)addr, (socklen_t*)&len);
    assert(len == sizeof(struct sockaddr_in));
    return rc;
}

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
    char name[MFS_MAX_NAME];
    int inum;
} Dent_t;

Checkpoint_t* cp;
int fd;

void initfs() {
    memset(cp, -1, sizeof(Checkpoint_t));
    cp->inode_map_ptrs[0] = sizeof(Checkpoint_t);
    int root_off = cp->inode_map_ptrs[0] + sizeof(Inodemap_t);
    cp->end = root_off + sizeof(Inode_t) + sizeof(Dent_t) * 2;
    WRITE(fd, cp, sizeof(Checkpoint_t));
    Inodemap_t* im = malloc(sizeof(Inodemap_t));
    memset(im->inode_ptrs, -1, sizeof(im->inode_ptrs));
    im->inode_ptrs[0] = root_off;
    WRITE(fd, im, sizeof(Inodemap_t));
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
    Inodemap_t im;
    if (off != -1) {
        lseek(fd, off, SEEK_SET);
        READ(fd, &im, sizeof(Inodemap_t));
    } else {
        memset(&im, -1, sizeof(Inodemap_t));
    }

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
        if (off == -1) return i * INODE_IN_PIECE;
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
    if (strlen(name) > MFS_MAX_NAME) return -1;

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
    assert(didx < MAX_DPTR);

    pinode->dptrs[didx] = cp->end;

    Dent_t de;
    memset(&de, 0, sizeof(Dent_t));
    de.inum = free_inum;
    strncpy(de.name, name, MFS_MAX_NAME);
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
    if (inode->dptrs[block] == -1) inode->size += MFS_BLOCK_SIZE;
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
        if (strncmp(de.name, name, MFS_MAX_NAME) == 0) {
            foundinum = de.inum;
            inode->dptrs[i] = -1;
            commit_inode(pinum, inode);
            break;
        }
    }
    if (foundinum == -1) return 0;
    commit_inode(foundinum, NULL);
    syncfs();
    return 0;
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

    int sd = UDP_Open(port);
    while (1) {
        struct sockaddr_in addr;
        MFS_Req_t req;
        int rc = UDP_Read(sd, &addr, (char*)&req, sizeof(MFS_Req_t));
        if (rc < 0) {
            puts("Bad UDP Read");
            continue;
        }
        int ret;
        MFS_Res_t res;
        switch (req.type) {
            case MLOOKUP:
                res.ret = MFS_Lookup(req.lookup.pinum, req.lookup.name);
                break;
            case MSTAT:
                res.ret = MFS_Stat(req.stat.inum, &res.stat);
                break;
            case MWRITE:
                res.ret =
                    MFS_Write(req.write.inum, req.write.buf, req.write.block);
                break;
            case MREAD:
                res.ret = MFS_Read(req.read.inum, res.buf, req.read.block);
                break;
            case MCREAT:
                res.ret =
                    MFS_Creat(req.creat.pinum, req.creat.type, req.creat.name);
                break;
            case MUNLINK:
                res.ret = MFS_Unlink(req.unlink.pinum, req.unlink.name);
                break;
            case MSHUTDOWN:
                res.ret = MFS_Shutdown();
                break;
            default:
                assert(0);
                break;
        }
        UDP_Write(sd, &addr, (char*)&res, sizeof(MFS_Res_t));
    }

    return 0;
}
