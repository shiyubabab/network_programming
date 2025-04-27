#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "../socket_kernels.h"
#include "Epoll.h"

#define READ_BUFFER 1024
using namespace std;

void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void handleReadEvent(int sockfd){
	char buf[READ_BUFFER];
    while(true){
		memset(&buf,0,sizeof(buf));
		ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
		if(bytes_read > 0){
			printf("message from client fd %d: %s\n", sockfd, buf);
			write(sockfd, buf, sizeof(buf));
		} else if(bytes_read == -1 && errno == EINTR){
			printf("continue reading");
			continue;
		} else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
			printf("finish reading once, errno: %d\n", errno);
			break;
		} else if(bytes_read == 0){  
			printf("EOF, client fd %d disconnected\n", sockfd);
			close(sockfd);
			break;
		}
	}
}

int main(int argc,char *argv[]) {
	Errif(argc !=2,"Please input format as ./server <port>");
	int sockfd = Open_listenfd(argv[1]);
	cout << "i am in main and fd is " << sockfd << endl;

	Epoll *ep = new Epoll();
	setnonblocking(sockfd);
	ep->addFd(sockfd, EPOLLIN | EPOLLET);

    while(true){
		std::vector<epoll_event> events = ep->poll();
		int nfds = events.size();
        for(int i = 0; i < nfds; ++i){
			cout<< "nfds is " << nfds << endl;
            if(events[i].data.fd == sockfd){        
                struct sockaddr_in clnt_addr;
                memset(&clnt_addr, 0,sizeof(clnt_addr));
                socklen_t clnt_addr_len = sizeof(clnt_addr);

                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
				std::cout << "clnt_sockfd is " << clnt_sockfd<< endl;
                Errif(clnt_sockfd == -1, "socket accept error");
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
                setnonblocking(clnt_sockfd);
				ep->addFd(clnt_sockfd,EPOLLIN | EPOLLET);
            } else if(events[i].events & EPOLLIN){  
				handleReadEvent(events[i].data.fd);
			} else{         
                printf("something else happened\n");
            }
        }
    }
    close(sockfd);
    return 0;
}
