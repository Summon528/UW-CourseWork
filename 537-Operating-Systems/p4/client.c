#include <stdio.h>

#include "mfs.h"

int main(int argc, char* argv[]) {
    MFS_Init("127.0.0.1", 8080);

    printf("%d\n", MFS_Lookup(0, "."));
    MFS_Stat_t stat;
    MFS_Stat(0, &stat);
    printf("%d\n", stat.type);
    printf("%d\n", stat.size);
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "test"));

    printf("%d\n", MFS_Creat(0, MFS_DIRECTORY, "test2"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "test3"));
    char s[] = "hello world!";
    printf("%d\n", MFS_Write(1, s, 2));
    puts("==");
    MFS_Stat(1, &stat);
    printf("%d\n", stat.type);
    printf("%d\n", stat.size);
    puts("==");
    char buf[4096];
    printf("%d\n", MFS_Read(1, buf, 2));
    printf("%s\n", buf);
    printf("%d\n", MFS_Unlink(0, "test"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "apple"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "banaa"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "5"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "6"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "7"));
    printf("%d\n", MFS_Creat(0, MFS_REGULAR_FILE, "8"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "9"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "10"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "11"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "12"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "13"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "14"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "15"));
    printf("%d\n", MFS_Creat(2, MFS_REGULAR_FILE, "16"));
    return 0;
}
