#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

#pragma GCC optimize("Ofast")

#define PREPR(c, cnt)                                 \
    if (cnt != 0) {                                   \
        putchar_unlocked((cnt)&0x000000ff);           \
        putchar_unlocked((((cnt)&0x0000ff00) >> 8));  \
        putchar_unlocked((((cnt)&0x00ff0000) >> 16)); \
        putchar_unlocked((((cnt)&0xff000000) >> 24)); \
        putchar_unlocked((c));                        \
    }

const unsigned long WORKSIZE = (1 << 24);

typedef struct ret {
    char* str;
    char front, back;
    int flen, blen, slen;
} ret_t;

typedef struct arg {
    char* src;
    int st;
    int sz;
} arg_t;

typedef struct que {
    arg_t* data;
    int qsize;
    int used;
} que_t;

ret_t* rets;
que_t* que;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* worker() {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (que->used == que->qsize) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        int id = que->used;
        arg_t arg = que->data[que->used++];
        pthread_mutex_unlock(&mutex);
        memset(&rets[id], 0, sizeof(ret_t));

        int st = arg.st, ed = arg.st + arg.sz;
        for (; st < ed && arg.src[st] == 0; st++)
            ;
        if (st == ed) continue;
        for (; ed - 1 >= 0 && arg.src[ed - 1] == 0; ed--)
            ;

        char prevchar = arg.src[st];
        int curcnt = 1, ssz = 0, sidx = 0;

        for (int i = st + 1; i < ed; i++) {
            if (arg.src[i] == 0) continue;
            if (prevchar == arg.src[i]) {
                curcnt++;
                continue;
            }
            if (rets[id].front == 0) {
                // First
                rets[id].front = prevchar;
                rets[id].flen = curcnt;
            } else {
                // Mid
                if (sidx + 4 >= ssz) {
                    ssz += sizeof(char) * (5 * (1 << 16));
                    rets[id].str = realloc(rets[id].str, ssz);
                }
                rets[id].str[sidx++] = curcnt & 0x000000ff;
                rets[id].str[sidx++] = (curcnt & 0x0000ff00) >> 8;
                rets[id].str[sidx++] = (curcnt & 0x00ff0000) >> 16;
                rets[id].str[sidx++] = (curcnt & 0xff000000) >> 24;
                rets[id].str[sidx++] = prevchar;
            }
            prevchar = arg.src[i];
            curcnt = 1;
        }
        rets[id].back = prevchar;
        rets[id].blen = curcnt;
        rets[id].slen = sidx;
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        puts("pzip: file1 [file2 ...]");
        return 1;
    }

    int* fsizes = malloc(sizeof(long) * argc - 1);
    que = malloc(sizeof(que));
    que->used = 0;
    for (int i = 1; i < argc; i++) {
        struct stat st;
        if (stat(argv[i], &st) == -1) continue;
        fsizes[i - 1] = st.st_size;
        que->qsize +=
            (fsizes[i - 1] / WORKSIZE) + (fsizes[i - 1] % WORKSIZE != 0);
    }
    que->data = malloc(sizeof(arg_t) * que->qsize);
    for (int i = 1, qidx = 0; i < argc; i++) {
        if (fsizes[i - 1] == 0) continue;
        int fd = open(argv[i], O_RDONLY);
        assert(fd != -1);
        char* mem =
            (char*)mmap(NULL, fsizes[i - 1], PROT_READ, MAP_PRIVATE, fd, 0);
        assert(mem != (char*)-1);
        int st = 0;
        for (int j = 0; j < fsizes[i - 1] / WORKSIZE; j++) {
            que->data[qidx].src = mem;
            que->data[qidx].sz = WORKSIZE;
            que->data[qidx].st = st;
            st += WORKSIZE;
            qidx++;
        }
        if (fsizes[i - 1] % WORKSIZE != 0) {
            que->data[qidx].src = mem;
            que->data[qidx].sz = fsizes[i - 1] % WORKSIZE;
            que->data[qidx].st = st;
            qidx++;
        }
    }

    rets = malloc(sizeof(ret_t) * que->qsize);

    int nprocs = get_nprocs();

    pthread_t pthreads[nprocs];
    for (int i = 0; i < nprocs; i++) {
        pthread_create(&pthreads[i], NULL, worker, NULL);
    }
    for (int i = 0; i < nprocs; i++) {
        pthread_join(pthreads[i], NULL);
    }

    char prevchar = 0;
    int cnt = 0;
    // if rets[i].front == 0, then the whole chunck contains the same char
    for (int i = 0; i < que->qsize; i++) {
        if (rets[i].front == 0) {
            if (prevchar == rets[i].back) {
                cnt += rets[i].blen;
            } else {
                PREPR(prevchar, cnt);
                prevchar = rets[i].back;
                cnt = rets[i].blen;
            }
            continue;
        }

        if (prevchar == rets[i].front) {
            PREPR(prevchar, cnt + rets[i].flen);
        } else {
            PREPR(prevchar, cnt);
            PREPR(rets[i].front, rets[i].flen);
        }

        fwrite_unlocked(rets[i].str, sizeof(char), rets[i].slen, stdout);

        prevchar = rets[i].back;
        cnt = rets[i].blen;
    }

    PREPR(prevchar, cnt);
}