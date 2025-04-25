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
#include <arpa/inet.h>
using namespace std;

int main(int argc,char *argv[]){
	if (argc !=2){
		fprintf(stderr,"Usage: %s port msg...\n",argv[1]);
		exit(EXIT_FAILURE);
	}

	struct addrinfo hints;
	struct addrinfo *result,*rp;
	int socket_fd,optval=1;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	int s = getaddrinfo(NULL,argv[1],&hints,&result);
	if(s != 0){
		fprintf(stderr,"getaddrinfo: %s \n",gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for(rp=result; rp != NULL ;rp->ai_next){
		socket_fd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(socket_fd == -1) continue;

		setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,
				(const void *)&optval , sizeof(int));

		if(bind(socket_fd,rp->ai_addr,rp->ai_addrlen) == 0) break;

		close(socket_fd);
	}
	freeaddrinfo(result);

	if(rp == NULL){
		fprintf(stderr,"Could not connect\n");
		exit(EXIT_FAILURE);
	}

	if(listen(socket_fd,SOMAXCONN)<0){
		close(socket_fd);
		return -1;
	}

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);
	bzero(&clnt_addr, sizeof(clnt_addr));

	int clnt_sockfd = accept(socket_fd, (sockaddr*)&clnt_addr, &clnt_addr_len);

	printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

	return 0;
}
