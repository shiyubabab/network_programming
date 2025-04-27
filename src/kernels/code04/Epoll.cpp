/*************************************************************************
	> File Name: Epoll.cpp
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 27 Apr 2025 02:45:00 PM CST
 ************************************************************************/

#include "Epoll.h"
#include "../socket_kernels.h"

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd(-1),events(nullptr) {
	epfd = epoll_create1(0);
	Errif(epfd == -1,"epoll create error");
	events = new epoll_event[MAX_EVENTS];
	memset(events,0,sizeof(struct epoll_event)*MAX_EVENTS);
}

Epoll::~Epoll(){
	if(epfd !=-1){
		close(epfd);
		epfd = -1;
	}
	delete [] events;
}

void Epoll::addFd(int fd,uint32_t op){
	struct epoll_event ev;
	memset(&ev,0,sizeof(ev));
	ev.data.fd = fd;
	ev.events = op;
	Errif(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev) == -1,"epoll add event error");
}

std::vector<epoll_event> Epoll::poll(int timeout){
	std::vector<epoll_event> activeEvents;
	int nfds = epoll_wait(epfd,events,MAX_EVENTS,timeout);
	Errif(nfds == -1,"epoll wait error");
	for(int i = 0;i<nfds;++i){
		activeEvents.push_back(events[i]);
	}
	return activeEvents;
}
