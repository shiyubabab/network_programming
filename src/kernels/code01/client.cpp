/*************************************************************************
	> File Name: client.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 25 Apr 2025 10:16:25 AM CST
 ************************************************************************/

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
using namespace std;

int main(int argc,char *argv[]){
	if (argc < 3){
		fprintf(stderr,"Usage: %s host port msg...\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	struct addrinfo hints;
	struct addrinfo *result,*rp;
	int socket_fd;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	int s = getaddrinfo(argv[1],argv[2],&hints,&result);
	if(s != 0){
		fprintf(stderr,"getaddrinfo: %s \n",gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for(rp=result; rp != NULL ;rp->ai_next){
		socket_fd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(socket_fd == -1) continue;
		if(connect(socket_fd,rp->ai_addr,rp->ai_addrlen) != -1) break;
		close(socket_fd);
	}
	freeaddrinfo(result);

	if(rp == NULL){
		fprintf(stderr,"Could not connect\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
