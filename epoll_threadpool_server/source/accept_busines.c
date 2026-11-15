#include<epoll_threadpool_server.h>

void* accept_busines(void *arg){
	int accept_sock=*(int*)arg;
	int client_sock;
	char cip[16];
	struct sockaddr_in client_addr;
	struct epoll_event node;
	socklen_t addrlen=sizeof(client_addr);
	if((client_sock=accept(accept_sock,(struct sockaddr*)&client_addr,&addrlen))==-1){
		perror("accept call failed");
		return NULL;
	}
	//设置监听
	node.data.fd=client_sock;
	node.events=EPOLLIN|EPOLLET;
	if((epoll_ctl(epfd,EPOLL_CTL_ADD,client_sock,&node))==-1){
		perror("add call failed");
		return NULL;
	}
	//首次响应客户端
	bzero(cip,16);
	inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,cip,16);
	first_response(client_sock,cip);
	return NULL;
}
