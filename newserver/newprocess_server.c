#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>

void* client_thread(void* arg)
{
	int client_sockfd;
	int len;
	client_sockfd=*(int*)arg;
	free(arg);
	//响应系统时间业务
	char buffer[4096];
	char* dfl_msg="Please Try Again\n";
	time_t ti;
	char time_buf[1024];
	bzero(buffer,sizeof(buffer));
	while ((len=recv(client_sockfd,buffer,sizeof(buffer),0))>0)
	{
		//时间处理及响应
		buffer[strlen(buffer)-1]='\0';
		if (strcmp(buffer,"time")==0)
		{
			bzero(time_buf,sizeof(time_buf));
			ti=time(NULL);
			ctime_r(&ti,time_buf);
			send(client_sockfd,time_buf,strlen(time_buf),MSG_NOSIGNAL);

		}
		else
		{
			send(client_sockfd,dfl_msg,strlen(dfl_msg),MSG_NOSIGNAL);
		}
		bzero(buffer,sizeof(buffer));

	}
	if(len==-1)
	{
		perror("recv call fail");
	}
	else if(len==0)
	{
		printf("client exit close clientsock %d\n",client_sockfd);
	}
	close(client_sockfd);
	printf("client thread 0x%x exit\n", (unsigned int)pthread_self());
	return NULL;
}
int main()
{
	//初始化网络
	int server_sock;
	int client_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t addrlen;
	if((server_sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket call failed");

	}
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(8080);
	if((bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr)))==-1)
	{
		perror("bind call fail");
	}
	listen(server_sock,128);
	//创建分离太
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	int err;
	pthread_t tid;
	int* pclient_sock;
	printf("thread server Runnning,main thread 0x%x Accepting..\n", (unsigned int)pthread_self());
	while(1)
	{
		//主线程用于连接
		addrlen=sizeof(client_addr);
		if((client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&addrlen))==-1)
		{
			perror("accept error");
			exit(0);
		}

		int len;
		char response[4096];
		char cip[16];
		bzero(response,sizeof(response));
		bzero(cip,16);
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,cip,16);
		sprintf(response,"hi %s ,connect server success\n",cip);
		send(client_sock,response,strlen(response),MSG_NOSIGNAL);
		printf("server accept success,client ip %s client port %d",cip,ntohs(client_addr.sin_port));
		pclient_sock = (int*)malloc(sizeof(int));
		if (pclient_sock==NULL) {
			perror("malloc fail");
			close(client_sock);
			continue;
		}
		*pclient_sock=client_sock;
		err=pthread_create(&tid,&attr,client_thread,(void*)pclient_sock);
		if (err!=0) {
			perror("pthread_create error");
			free(pclient_sock);
			close(client_sock);
			continue; 
		}
	}
	printf("server exit\n");
	close(server_sock);
	exit(0);

}
