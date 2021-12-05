#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mfs.h"
#include "udp.h"

#define WRITE(fd, buf, len)           \
    if (write(fd, buf, len) != len) { \
        perror("write");              \
        assert(0);                    \
    }

#define BUFFER_SIZE (1000)
#define MFS_MAX_INODE (4096)
#define MFS_INODE_IN_PIECE (16)
#define MFS_MAX_PIECE (MFS_MAX_INODE / MFS_INODE_IN_PIECE)

typedef struct __MFS_Checkpoint_t {
    int end;
    int inode_map_ptrs[MFS_MAX_PIECE];
} MFS_Checkpoint_t;

typedef struct __MFS_Inodemap_t {
    int inode_ptrs[MFS_INODE_IN_PIECE];
} MFS_Inodemap_t;

typedef struct __MFS_Inode_t {
    uint8_t type;
    int size;
    int dptrs[14];
} MFS_Inode_t;

typedef struct __MFS_Dent_t {
    char name[28];
    int inode;
} MFS_Dent_t;

void initfs(MFS_Checkpoint_t* cp, int fd) {
    cp->inode_map_ptrs[0] = sizeof(MFS_Checkpoint_t);
    for (int i = 1; i < MFS_MAX_PIECE; i++) {
        cp->inode_map_ptrs[i] =
            cp->inode_map_ptrs[i - 1] + sizeof(MFS_Inodemap_t);
    }
    int root_off =
        cp->inode_map_ptrs[MFS_MAX_PIECE - 1] + sizeof(MFS_Inodemap_t);
    cp->end = root_off + sizeof(MFS_Inode_t) + sizeof(MFS_Dent_t) * 2;
    WRITE(fd, cp, sizeof(MFS_Checkpoint_t));
    MFS_Inodemap_t* im = malloc(sizeof(MFS_Inodemap_t));
    memset(im->inode_ptrs, -1, sizeof(im->inode_ptrs));
    im->inode_ptrs[0] = root_off;
    WRITE(fd, im, sizeof(MFS_Inodemap_t));
    im->inode_ptrs[0] = -1;
    for (int i = 1; i < MFS_MAX_PIECE; i++) {
        WRITE(fd, im, sizeof(MFS_Inodemap_t));
    }
    free(im);

    MFS_Inode_t* root = calloc(1, sizeof(MFS_Inode_t));
    root->type = MFS_DIRECTORY;
    root->size = 0;
    root->dptrs[0] = root_off + sizeof(MFS_Inode_t);
    root->dptrs[1] = root_off + sizeof(MFS_Inode_t) + sizeof(MFS_Dent_t);
    WRITE(fd, root, sizeof(MFS_Inode_t));
    free(root);

    MFS_DirEnt_t* de = calloc(1, sizeof(MFS_DirEnt_t));
    de->name[0] = '.';
    de->inum = 0;
    WRITE(fd, de, sizeof(MFS_DirEnt_t));

    de->name[1] = '.';
    WRITE(fd, de, sizeof(MFS_DirEnt_t));
    free(de);
    fsync(fd);
}

int main(int argc, char* argv[]) {
    assert(argc == 3);
    int port = atoi(argv[1]);
    char* img = argv[2];
    MFS_Checkpoint_t* cp = malloc(sizeof(MFS_Checkpoint_t));
    int fd;
    if ((fd = open(img, O_CREAT | O_RDWR | O_EXCL, 0644)) >= 0) {
        initfs(cp, fd);
    } else if (errno = EEXIST) {
        fd = open(img, O_RDWR);
        read(fd, cp, sizeof(MFS_Checkpoint_t));
    } else {
        perror("open");
        assert(0);
    }

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
