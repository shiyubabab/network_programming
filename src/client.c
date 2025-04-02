/*************************************************************************
	> File Name: client.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 28 Mar 2025 10:08:16 AM CST
 ************************************************************************/

#include "../helper/csapp.h"
#define MAX_LINE 1025

int main(int argc,char **argv){
	if(argc != 3){
		unix_error("fomat ./client localhost xxxx");
	}
	int clientfd;
	clientfd = Open_clientfd(argv[1],argv[2]);
	if(clientfd <= 0){
		unix_error("it is fail to open client");
	}
	rio_t rp;
	Rio_readinitb(&rp,clientfd);

	char str[MAX_LINE];
	char stroutput[MAX_LINE];
	char* rptr =NULL;

	while((rptr = Fgets(str,MAX_LINE,stdin))!=NULL){
		Rio_writen(clientfd,str,strlen(str));
		Rio_readlineb(&rp,stroutput,MAX_LINE);
		printf("%s",stroutput);
	}
	exit(EXIT_SUCCESS);
}

