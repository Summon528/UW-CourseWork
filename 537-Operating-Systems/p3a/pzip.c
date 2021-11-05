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

#define PREPR(c, cnt)                               \
    if (cnt != 0) {                                 \
        putchar_unlocked((cnt)&0x000000ff);         \
        putchar_unlocked(((cnt)&0x0000ff00) >> 8);  \
        putchar_unlocked(((cnt)&0x00ff0000) >> 16); \
        putchar_unlocked(((cnt)&0xff000000) >> 24); \
        putchar_unlocked((c));                      \
    }

const unsigned long WORKSIZE = (1 << 24);

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

que_t* que;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
char gprevchar = 0;
int turn = 0, charcnt = 0;

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

        char* str = NULL;

        int st = arg.st, ed = arg.st + arg.sz;
        for (; st < ed && arg.src[st] == 0; st++)
            ;
        if (st == ed) {
            pthread_mutex_lock(&mutex_cond);
            while (turn != id) {
                pthread_cond_wait(&cond, &mutex_cond);
            }
            goto done;
        }
        for (; ed - 1 >= 0 && arg.src[ed - 1] == 0; ed--)
            ;

        char front = 0, back = 0;
        int flen = 0, blen = 0, slen = 0;

        char prevchar = arg.src[st];
        int curcnt = 1, ssz = 0, sidx = 0;

        for (int i = st + 1; i < ed; i++) {
            if (arg.src[i] == 0) continue;
            if (prevchar == arg.src[i]) {
                curcnt++;
                continue;
            }
            if (front == 0) {
                // First
                front = prevchar;
                flen = curcnt;
            } else {
                // Mid
                if (sidx + 4 >= ssz) {
                    ssz += sizeof(char) * (5 * (1 << 16));
                    str = realloc(str, ssz);
                }
                str[sidx++] = curcnt & 0x000000ff;
                str[sidx++] = (curcnt & 0x0000ff00) >> 8;
                str[sidx++] = (curcnt & 0x00ff0000) >> 16;
                str[sidx++] = (curcnt & 0xff000000) >> 24;
                str[sidx++] = prevchar;
            }
            prevchar = arg.src[i];
            curcnt = 1;
        }
        back = prevchar;
        blen = curcnt;
        slen = sidx;

        pthread_mutex_lock(&mutex_cond);
        while (turn != id) {
            pthread_cond_wait(&cond, &mutex_cond);
        }
        if (front == 0) {
            if (gprevchar == back) {
                charcnt += blen;
            } else {
                PREPR(gprevchar, charcnt);
                gprevchar = back;
                charcnt = blen;
            }
            goto done;
        }

        if (gprevchar == front) {
            PREPR(gprevchar, charcnt + flen);
        } else {
            PREPR(gprevchar, charcnt);
            PREPR(front, flen);
        }
        fwrite_unlocked(str, sizeof(char), slen, stdout);
        gprevchar = back;
        charcnt = blen;

    done:
        free(str);
        turn++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex_cond);
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

    int nprocs = get_nprocs() * 3 / 2;

    pthread_t pthreads[nprocs];
    for (int i = 0; i < nprocs; i++) {
        pthread_create(&pthreads[i], NULL, worker, NULL);
    }
    for (int i = 0; i < nprocs; i++) {
        pthread_join(pthreads[i], NULL);
    }
    PREPR(gprevchar, charcnt);
}