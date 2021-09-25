#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[]) {
    int x1 = getiocount();

    int rc = fork();

    int total = 0;
    int i;

    
    if (rc > 0) 
        (void) wait();

    for (i = 0; i < 1000; i++) {
        char buf[100];
        if (i % 2 == 0) 
            (void) read(4, buf, 1);
        else
            (void) write(4, buf, 1);
    }

   // https://wiki.osdev.org/Shutdown
    // (void) shutdown();
    if (rc > 0) {
	int x2 = getiocount();
	total += (x2 - x1);
	printf(1, "XV6_TEST_OUTPUT %d\n", total);
    }

    exit();
}
