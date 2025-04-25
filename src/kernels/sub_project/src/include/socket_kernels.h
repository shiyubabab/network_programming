/*************************************************************************
	> File Name: socket_kernels.h
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 25 Apr 2025 02:06:34 PM CST
 ************************************************************************/

#ifndef __KERNELS__
#define __KERNELS__

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

int Open_listenfd(char *port);
int Open_clientfd(char *addr,char *port);
void Errif(bool condition,const char *errmsg);

#endif
