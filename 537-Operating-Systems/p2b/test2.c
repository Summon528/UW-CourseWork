#include "types.h"
#include "user.h"
#include "pstat.h"

int main(int argc, char* argv[]) {
    struct pstat pstat;
    
    printf(1, "%d\n", settickets(2));
    printf(1, "uspace pid: %d\n", getpid());
    printf(1, "%d\n", getpinfo(&pstat));
    for (int i = 0; i < 64; i++) {
        printf(1, "pid: %d, inuse: %d, tickets: %d,ticks: %d\n", pstat.pid[i], pstat.inuse[i], pstat.tickets[i], pstat.ticks[i]);
    }

    exit();
}