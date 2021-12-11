#include "mfs.h"

#include "udp.h"

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
    UDP_Write(sd, addr, (char *)&req, sizeof(MFS_Req_t));
    return 0;
}