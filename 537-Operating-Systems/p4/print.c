
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mfs.h"
#define BUFFER_SIZE (1000)
#define MAX_NAME (28)
#define MAX_INODE (4096)
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
    int dptrs[14];
} Inode_t;

typedef struct {
    char name[MAX_NAME];
    int inum;
} Dent_t;

#define MAX_FILE_IN_DBLOCK (MFS_BLOCK_SIZE / sizeof(Dent_t))
typedef struct {
    Dent_t dent[MAX_FILE_IN_DBLOCK];
} Dblock_t;

Checkpoint_t* cp;
int fd;

int main() {
    fd = open("file.img", O_RDONLY);
    assert(fd != -1);
    cp = malloc(sizeof(Checkpoint_t));
    read(fd, cp, sizeof(Checkpoint_t));
    printf("Checkpoint:\n");
    printf("\tend: 0x%x\n", cp->end);
    for (int i = 0; i < MAX_PIECE; i++) {
        if (cp->inode_map_ptrs[i] == -1) continue;
        printf("\tinode_map_ptrs[%d]: 0x%x\n", i, cp->inode_map_ptrs[i]);
    }

    Inodemap_t im;
    puts("Inodes:");
    Inode_t inode;
    for (int i = 0; i < MAX_PIECE; i++) {
        if (cp->inode_map_ptrs[i] == -1) continue;
        lseek(fd, cp->inode_map_ptrs[i], SEEK_SET);
        read(fd, &im, sizeof(Inodemap_t));
        printf("\tpiece[%d]:\n", i);
        for (int j = 0; j < INODE_IN_PIECE; j++) {
            if (im.inode_ptrs[j] == -1) continue;
            lseek(fd, im.inode_ptrs[j], SEEK_SET);
            read(fd, &inode, sizeof(Inode_t));
            printf("\t\tinode[%d]:\n", j);
            printf("\t\t\ttype: %s\n",
                   inode.type == MFS_DIRECTORY ? "dir" : "file");
            printf("\t\t\tsize: %d\n", inode.size);
            for (int k = 0; k < 14; k++) {
                if (inode.dptrs[k] == -1) continue;
                printf("\t\t\tdptrs[%d]: 0x%x\n", k, inode.dptrs[k]);
                if (inode.type == MFS_DIRECTORY) {
                    Dblock_t db;
                    lseek(fd, inode.dptrs[k], SEEK_SET);
                    read(fd, &db, sizeof(Dblock_t));
                    for (int l = 0; l < MAX_FILE_IN_DBLOCK; l++) {
                        if (db.dent[l].inum == -1) continue;
                        printf("\t\t\t\tname: %s\n", db.dent[l].name);
                        printf("\t\t\t\tinum: %d\n", db.dent[l].inum);
                    }
                } else {
                    char buf[MFS_BLOCK_SIZE];
                    memset(&buf, 0, MFS_BLOCK_SIZE);
                    lseek(fd, inode.dptrs[k], SEEK_SET);
                    read(fd, &buf, MFS_BLOCK_SIZE);
                    printf("\t\t\t\t%s\n", buf);
                }
            }
        }
    }
}