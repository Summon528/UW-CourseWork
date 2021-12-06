#ifndef __MFS_h__
#define __MFS_h__

#define MFS_DIRECTORY (0)
#define MFS_REGULAR_FILE (1)

#define MFS_BLOCK_SIZE (4096)
#define MFS_MAX_NAME (28)

typedef struct __MFS_Stat_t {
    int type;  // MFS_DIRECTORY or MFS_REGULAR
    int size;  // bytes
    // note: no permissions, access times, etc.
} MFS_Stat_t;

int MFS_Init(char *hostname, int port);
int MFS_Lookup(int pinum, char *name);
int MFS_Stat(int inum, MFS_Stat_t *m);
int MFS_Write(int inum, char *buffer, int block);
int MFS_Read(int inum, char *buffer, int block);
int MFS_Creat(int pinum, int type, char *name);
int MFS_Unlink(int pinum, char *name);
int MFS_Shutdown();

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

#endif  // __MFS_h__