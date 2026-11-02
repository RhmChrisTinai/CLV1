#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>

int main()
{
	char buffer[4096];
	//构造目标的网络信息
	struct sockaddr_in destaddr;
	bzero(&destaddr,sizeof(destaddr));
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(8080);
	inet_pton(AF_INET,"127.0.0.1",&destaddr.sin_addr.s_addr);
	//create sock
	int sockfd;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	//发起tcp连接
	if((connect(sockfd,(struct sockaddr*)&destaddr,sizeof(destaddr)))==-1)
	{
		perror("connect call failed");
		exit(0);
	}
	printf("connect success\n");
	bzero(buffer,sizeof(buffer));
	recv(sockfd,buffer,sizeof(buffer),0);
	printf("client recv response=%s\n",buffer);
	char*msg="time\n";
	 send(sockfd,msg,strlen(msg),0);
	 bzero(buffer,sizeof(buffer));
	 recv(sockfd,buffer,sizeof(buffer),0);
	printf("back time %s\n",buffer);
	printf("client fin..\n");
	close(sockfd);

	return 0;
}
