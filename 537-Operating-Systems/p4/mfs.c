#include "mfs.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int UDP_FillSockAddr(struct sockaddr_in *addr, char *hostname, int port) {
    bzero(addr, sizeof(struct sockaddr_in));
    if (hostname == NULL) {
        return 0;  // it's OK just to clear the address
    }

    addr->sin_family = AF_INET;    // host byte order
    addr->sin_port = htons(port);  // short, network byte order

    struct in_addr *in_addr;
    struct hostent *host_entry;
    if ((host_entry = gethostbyname(hostname)) == NULL) {
        perror("gethostbyname");
        return -1;
    }
    in_addr = (struct in_addr *)host_entry->h_addr;
    addr->sin_addr = *in_addr;

    return 0;
}

int UDP_Write(int fd, struct sockaddr_in *addr, char *buffer, int n) {
    int addr_len = sizeof(struct sockaddr_in);
    int rc = sendto(fd, buffer, n, 0, (struct sockaddr *)addr, addr_len);
    return rc;
}

int UDP_Read(int fd, struct sockaddr_in *addr, char *buffer, int n) {
    int len = sizeof(struct sockaddr_in);
    int rc =
        recvfrom(fd, buffer, n, 0, (struct sockaddr *)addr, (socklen_t *)&len);
    assert(len == sizeof(struct sockaddr_in));
    return rc;
}

struct sockaddr_in *addr = NULL;
int sd = -1;
int MFS_Init(char *hostname, int port) {
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    addr = malloc(sizeof(struct sockaddr_in));
    if (UDP_FillSockAddr(addr, hostname, port) == -1) {
        free(addr);
        addr = NULL;
        return -1;
    }
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    assert(setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) >= 0);
    return 0;
}

int MFS_Lookup(int pinum, char *name) {
    assert(sd != -1 && addr != NULL);
    if (strlen(name) > MFS_MAX_NAME) return -1;
    MFS_Req_t req;
    req.type = MLOOKUP;
    req.lookup.pinum = pinum;
    strncpy(req.lookup.name, name, MFS_MAX_NAME);
    while (1) {
        puts("lookup");
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        return res.ret;
    }
}

int MFS_Stat(int inum, MFS_Stat_t *m) {
    assert(sd != -1 && addr != NULL);
    MFS_Req_t req;
    req.type = MSTAT;
    req.stat.inum = inum;
    while (1) {
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        memcpy(m, &res.stat, sizeof(MFS_Stat_t));
        return res.ret;
    }
}

int MFS_Write(int inum, char *buffer, int block) {
    assert(sd != -1 && addr != NULL);
    MFS_Req_t req;
    req.type = MWRITE;
    req.write.inum = inum;
    req.write.block = block;
    memcpy(&req.write.buf, buffer, MFS_BLOCK_SIZE);
    while (1) {
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        return res.ret;
    }
}

int MFS_Read(int inum, char *buffer, int block) {
    assert(sd != -1 && addr != NULL);
    MFS_Req_t req;
    req.type = MREAD;
    req.read.inum = inum;
    req.read.block = block;
    while (1) {
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        memcpy(buffer, &res.buf, MFS_BLOCK_SIZE);
        return res.ret;
    }
}

int MFS_Creat(int pinum, int type, char *name) {
    assert(sd != -1 && addr != NULL);
    if (strlen(name) > MFS_MAX_NAME) return -1;
    MFS_Req_t req;
    req.type = MCREAT;
    req.creat.pinum = pinum;
    req.creat.type = type;
    strncpy(req.creat.name, name, MFS_MAX_NAME);
    while (1) {
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        return res.ret;
    }
}

int MFS_Unlink(int pinum, char *name) {
    assert(sd != -1 && addr != NULL);
    if (strlen(name) > MFS_MAX_NAME) return -1;
    MFS_Req_t req;
    req.type = MUNLINK;
    req.unlink.pinum = pinum;
    strncpy(req.unlink.name, name, MFS_MAX_NAME);
    while (1) {
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        return res.ret;
    }
}

int MFS_Shutdown() {
    assert(sd != -1 && addr != NULL);
    MFS_Req_t req;
    req.type = MSHUTDOWN;
    while (1) {
        UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
        MFS_Res_t res;
        int rc = UDP_Read(sd, addr, (char *)&res, sizeof(MFS_Res_t));
        if (rc == -1 && errno == EAGAIN) continue;
        return res.ret;
    }
}