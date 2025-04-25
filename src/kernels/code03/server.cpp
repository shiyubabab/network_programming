/*************************************************************************
	> File Name: server.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Fri 25 Apr 2025 02:38:00 PM CST
 ************************************************************************/

#include "../socket_kernels.h"
using namespace std;
const int N = 1024;

int main(int argc,char *argv[]){
	Errif(argc != 2,"Please input format as ./server <port>");
	int sockfd = Open_listenfd(argv[1]);
	

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof(clnt_addr);
	memset(&clnt_addr, 0 ,sizeof(clnt_addr));

	int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
	Errif(clnt_sockfd == -1, "socket accept error");

	printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
	while (true) {
		char buf[N];
		memset(&buf, 0 ,sizeof(buf));
		ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
		if(read_bytes > 0){
			printf("message from client fd %d: %s\n", clnt_sockfd, buf);
			write(clnt_sockfd, buf, sizeof(buf));
		} else if(read_bytes == 0){
			printf("client fd %d disconnected\n", clnt_sockfd);
			close(clnt_sockfd);
			break;
		} else if(read_bytes == -1){
			close(clnt_sockfd);
			Errif(true, "socket read error");
		}
	}
	close(sockfd);
	return 0;
}
