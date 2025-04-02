/*************************************************************************
	> File Name: server.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 28 Mar 2025 01:41:06 PM CST
 ************************************************************************/

#include "../helper/csapp.h"
#define MAX_LINE 1025

void echo(int client_fd){
	rio_t rp;
	Rio_readinitb(&rp,client_fd);
	char buf[MAX_LINE];
	int n;
	while((n=Rio_readlineb(&rp,buf,MAX_LINE))>0){
		printf("receive from client : %s\n",buf);
		Rio_writen(client_fd,buf,n);
	}
}

static void *thread_start(void *arg){
	int connfd = *(int *)arg;
	Free(arg);
	echo(connfd);
	Close(connfd);
	return NULL;
}

int main(int argc,char **argv){
	if(argc!=2){
		unix_error("format ./server <port>");
	}

	int listenfd = Open_listenfd(argv[1]);

	int *connfd;
	pthread_t tid;
	struct sockaddr client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	memset(&client_addr,0,client_addr_len);


	while(1){


		/* Just for print client hostname */
		//Getnameinfo(&client_addr,client_addr_len,hostname,MAX_LINE,NULL,0,0);
		//printf("resceive from client %s\n",hostname);

		connfd = (int *)Malloc(sizeof(int *));
		*connfd = Accept(listenfd,&client_addr,&client_addr_len);
		Pthread_create(&tid,NULL,thread_start,(void *)connfd);
	}


	exit(EXIT_FAILURE);
}

