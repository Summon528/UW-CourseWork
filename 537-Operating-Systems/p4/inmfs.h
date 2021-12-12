#include "mfs.h"

enum MFS_Req_Type { MLOOKUP, MSTAT, MWRITE, MREAD, MCREAT, MUNLINK, MSHUTDOWN };

typedef struct {
    int pinum;
    char name[28];
} MFS_Lookup_Req_t;

typedef struct {
    int inum;
} MFS_Stat_Req_t;

typedef struct {
    int inum;
    char buf[MFS_BLOCK_SIZE];
    int block;
} MFS_Write_Req_t;

typedef struct {
    int inum;
    int block;
} MFS_Read_Req_t;

typedef struct {
    int pinum;
    int type;
    char name[28];
} MFS_Creat_Req_t;

typedef struct {
    int pinum;
    char name[28];
} MFS_Unlink_Req_t;

typedef struct {
    enum MFS_Req_Type type;
    union {
        MFS_Lookup_Req_t lookup;
        MFS_Creat_Req_t creat;
        MFS_Read_Req_t read;
        MFS_Write_Req_t write;
        MFS_Unlink_Req_t unlink;
        MFS_Stat_Req_t stat;
    };
} MFS_Req_t;

typedef struct {
    int ret;
    union {
        MFS_Stat_t stat;
        char buf[MFS_BLOCK_SIZE];
    };
} MFS_Res_t;