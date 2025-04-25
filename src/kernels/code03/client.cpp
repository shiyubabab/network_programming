/*************************************************************************
	> File Name: client.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 25 Apr 2025 02:14:18 PM CST
 ************************************************************************/

#include "../socket_kernels.h"
using namespace std;
const int N = 1024;


int main(int argc,char *argv[]){
	if(argc < 3){
		fprintf(stderr,"Usage: %s host port msg...\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int clientfd = Open_clientfd(argv[1],argv[2]);
	Errif(clientfd == -1,"Could not connect\n");

	while(true){
		char buf[N];
		memset(&buf,0,sizeof(buf));
		cin >> buf;
		ssize_t write_bytes = write(clientfd,buf,sizeof(buf));
		if(write_bytes == -1){
			printf("socket already disconnected, can't write any more!\n");
			break;
		}

		memset(&buf,0,sizeof(buf));
		ssize_t read_bytes = read(clientfd, buf, sizeof(buf));
		if(read_bytes > 0){
			printf("message from server: %s\n", buf);
		}else if(read_bytes == 0){
			printf("server socket disconnected!\n");
			break;
		}else if(read_bytes == -1){
			close(clientfd);
			Errif(true, "socket read error");
		}
	}
	close(clientfd);
	return 0;
}
