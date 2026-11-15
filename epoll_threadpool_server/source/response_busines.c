#include<epoll_threadpool_server.h>

void* response_busines(void *arg){
	int client_sock=*(int *)arg;
	char buffer[1500];
	char *msg="please try again\n";
	time_t tp;
	char time_buf[1024];
	//读取请求
	bzero(buffer,sizeof(buffer));
	int recvlen;
	while((recvlen=recv(client_sock,buffer,sizeof(buffer),MSG_DONTWAIT))>0){
		//处理请求
		buffer[strlen(buffer)-1]='\0';
		if(strcmp(buffer,"localtime")==0){
			bzero(time_buf,sizeof(time_buf));
			tp=time(NULL);
			ctime_r(&tp,time_buf);
			//反馈响应
			send(client_sock,time_buf,strlen(time_buf),MSG_NOSIGNAL);
		}
		else{
			send(client_sock,msg,strlen(msg),MSG_NOSIGNAL);
		}
	}
	//读取客户端断开并取消监听
	if(recvlen==0){
		printf("client close\n");
		epoll_ctl(epfd,EPOLL_CTL_DEL,client_sock,NULL);
		close(client_sock);
	}
	else{
		if(errno==EAGAIN){
			//非阻塞返回
		}
		else{
			perror("recv call failed");
		}
		sleep(1);
	}
	return NULL;
}
