#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/types.h>

const unsigned long WORKSIZE = (1 << 24);

typedef struct ret {
    char* chrs;
    int* cnt;
    int sz;
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
        int bufsz = 1 << 10, idx = 0;
        rets[id].chrs = malloc(sizeof(char) * bufsz);
        rets[id].cnt = malloc(sizeof(int) * bufsz);
        char prevchr = arg.src[arg.st];
        int curcnt = 1;
        for (int i = arg.st + 1; i <= arg.st + arg.sz; i++) {
            if (i == arg.st + arg.sz || prevchr != arg.src[i]) {
                if (idx >= bufsz) {
                    bufsz <<= 1;
                    rets[id].chrs =
                        realloc(rets[id].chrs, sizeof(char) * bufsz);
                    rets[id].cnt = realloc(rets[id].cnt, sizeof(int) * bufsz);
                }
                rets[id].chrs[idx] = prevchr;
                rets[id].cnt[idx++] = curcnt;
                if (i == arg.sz) break;
                curcnt = 1;
                prevchr = arg.src[i];
            } else {
                curcnt++;
            }
        }
        rets[id].sz = idx;
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
        assert(stat(argv[i], &st) != -1);
        fsizes[i - 1] = st.st_size;
        que->qsize +=
            (fsizes[i - 1] / WORKSIZE) + (fsizes[i - 1] % WORKSIZE != 0);
    }
    que->data = malloc(sizeof(arg_t) * que->qsize);
    for (int i = 1, qidx = 0; i < argc; i++) {
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

    for (int i = 0; i < que->qsize; i++) {
        for (int j = 0; j < rets[i].sz; j++) {
            if (rets[i].chrs[j] != prevchar) {
                if (prevchar != 0) {
                    putchar_unlocked(cnt & 0x000000ff);
                    putchar_unlocked((cnt & 0x0000ff00) >> 8);
                    putchar_unlocked((cnt & 0x00ff0000) >> 16);
                    putchar_unlocked((cnt & 0xff000000) >> 24);
                    putchar_unlocked(prevchar);
                }
                prevchar = rets[i].chrs[j];
                cnt = rets[i].cnt[j];
            } else {
                cnt += rets[i].cnt[j];
            }
        }
    }
    putchar_unlocked(cnt & 0x000000ff);
    putchar_unlocked((cnt & 0x0000ff00) >> 8);
    putchar_unlocked((cnt & 0x00ff0000) >> 16);
    putchar_unlocked((cnt & 0xff000000) >> 24);
    putchar_unlocked(prevchar);
}