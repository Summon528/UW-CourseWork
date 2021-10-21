#include "types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    printf(1, "%d\n", *((int*)atoi(argv[1])));

    exit();
}