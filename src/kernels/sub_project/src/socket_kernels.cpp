/*************************************************************************
	> File Name: socket_kernels.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 25 Apr 2025 10:16:25 AM CST
 ************************************************************************/

#include "./include/socket_kernels.h"

int open_listenfd(char *port){
	struct addrinfo hints;
	struct addrinfo *result,*rp;
	int socket_fd,optval=1;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	int s = getaddrinfo(NULL,port,&hints,&result);
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
	return socket_fd;
}


int open_clientfd(char *addr,char *port){
	struct addrinfo hints;
	struct addrinfo *result,*rp;
	int socket_fd;

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	int s = getaddrinfo(addr,port,&hints,&result);
	if(s != 0){
		fprintf(stderr,"getaddrinfo: %s \n",gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for(rp=result; rp != NULL ; rp->ai_next){
		socket_fd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
		if(socket_fd == -1) continue;
		if(connect(socket_fd,rp->ai_addr,rp->ai_addrlen)!=-1) break;
		close(socket_fd);
	}
	freeaddrinfo(result);

	if(rp==NULL){
		fprintf(stderr,"Could not connect\n");
		return -1;
	}
	return socket_fd;
}

int Open_listenfd(char *port){
	int rc;
	if((rc = open_listenfd(port)) < 0){
		fprintf(stderr,"Usage: %s port msg...\n",port);
	}
	return rc;
}

int Open_clientfd(char *addr,char *port){
	int rc;
	if((rc = open_clientfd(addr,port)) < 0){
		fprintf(stderr,"Usage: %s host port msg...\n",port);
	}
	return rc;
}

void Errif(bool condition,const char *errmsg){
	if(condition){
		perror(errmsg);
		exit(EXIT_FAILURE);
	}
}
