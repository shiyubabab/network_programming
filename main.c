/*************************************************************************
	> File Name: main.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Thu 27 Mar 2025 05:28:55 PM CST
 ************************************************************************/

#include "csapp.h"
#define NI_MAXHOST 1025


int main(int argc,char **argv){
	if(argc != 2){
		unix_error("fomat ./mygetinfo www.website.com");
	}
	struct addrinfo hints,*res,*cur;
	memset(&hints,0,sizeof(hints));
	hints.ai_family	  = AF_INET;
	hints.ai_socktype =	SOCK_STREAM;
	hints.ai_flags	  = 0;
	Getaddrinfo(argv[1],NULL,&hints,&res);

	char hbuf[NI_MAXHOST];
	for(cur=res;cur!=NULL;cur=cur->ai_next){
		Getnameinfo(cur->ai_addr,cur->ai_addrlen,hbuf,sizeof(hbuf),NULL,0,NI_NUMERICHOST);
		printf("flags : %d\n",cur->ai_flags);
		printf("family : %d\n",cur->ai_family);
		printf("socktype : %d\n",cur->ai_socktype);
		printf("addrlen : %d\n",cur->ai_addrlen);
		printf("host : %s\n",hbuf);
	}

	return 0;
}
