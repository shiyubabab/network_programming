/*************************************************************************
	> File Name: server.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 28 Mar 2025 01:41:06 PM CST
 ************************************************************************/

#include "../helper/csapp.h"
#include "../helper/producer_consumer.h"
#define MAX_LINE 1025

void echo(int client_fd){
	rio_t rp;
	Rio_readinitb(&rp,client_fd);
	char buf[MAX_LINE];
	int n;
	pthread_t tid = pthread_self();
	while((n=Rio_readlineb(&rp,buf,MAX_LINE))>0){
		printf("I am thread[%ld],receive from client : %s\n",tid,buf);
		Rio_writen(client_fd,buf,n);
	}
}

static void *thread_start(void *arg){
	Pthread_detach(pthread_self());
	while(1){
		int connfd = sbuf_remove(arg);
		echo(connfd);
		Close(connfd);
	}
	return NULL;
}

int main(int argc,char **argv){
	if(argc!=2){
		unix_error("format ./server <port>");
	}

	int listenfd = Open_listenfd(argv[1]);

	int thread_number = 10;
	sbuf_t sp;
	sbuf_init(&sp,thread_number);

	for(int i = 0;i<thread_number;i++){
		pthread_t tid;
		Pthread_create(&tid,NULL,thread_start,(void *)&sp);
		printf("thread[%ld] is created.\n",tid);
	}


	int connfd;
	struct sockaddr client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	memset(&client_addr,0,client_addr_len);
	char hostname[MAX_LINE];


	while(1){
		connfd = Accept(listenfd,&client_addr,&client_addr_len);
		sbuf_insert(&sp,connfd);

		/* Just for print client hostname */
		Getnameinfo(&client_addr,client_addr_len,hostname,MAX_LINE,NULL,0,0);
		printf("resceive from client %s\n",hostname);
		
	}

	sbuf_deinit(&sp);


	exit(EXIT_FAILURE);
}

