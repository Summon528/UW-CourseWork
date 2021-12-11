#ifndef __UDP_h__
#define __UDP_h__

//
// includes
//

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

//
// prototypes
//

int UDP_Open(int port);
int UDP_Close(int fd);

int UDP_Read(int fd, struct sockaddr_in *addr, char *buffer, int n);
int UDP_Write(int fd, struct sockaddr_in *addr, char *buffer, int n);

int UDP_FillSockAddr(struct sockaddr_in *addr, char *hostName, int port);

#endif  // __UDP_h__
