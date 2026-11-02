#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<time.h>
char* get_current_time()
{
	time_t rawtime;
	time(&rawtime);
	return ctime(&rawtime);
}
int server_netinitlizer(const char* ip,int port)
{
	int server_sockfd;
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	if(!ip)
		addr.sin_addr.s_addr=htonl(INADDR_ANY);
	else
		inet_pton(AF_INET,ip,&addr.sin_addr.s_addr);
		if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		{
			perror("sock creat call fail");

		}
		bind(server_sockfd,(struct sockaddr*)&addr,sizeof(addr));
		listen(server_sockfd,128);
		return server_sockfd;
}

//服务器启动端口
//客户端向服务器发送time关键字，服务器回复时间
int server_start(int shutdown)
{
	//单进程非阻塞模型
	//server_sockfd设置，变为非阻塞sockfd
	int server_sockfd=server_netinitlizer(NULL,8080);
	int flag;
	flag=fcntl(server_sockfd,F_GETFL);
	flag|=O_NONBLOCK;
	fcntl(server_sockfd,F_SETFL,flag);
	
	//网络初始化
	char response_buffer[4096];
	socklen_t addrlen;
	char cip[16];
	//client_sockfd数组，保存所有连接成功的sockfd,client_sock_array,初始化-1
	int client_sockfd[128];
	for(int i=0;i<128;i++)
	{
		client_sockfd[i]=-1;
	}

	//server_sockfd设置，变为非阻塞sockfd
	//循环等待连接即可
	while(shutdown)
	{
		//调用socket,查看是否有连接请求，如果右，建立连接，而后存储到client_sock
		struct sockaddr_in client_addr;
		addrlen=sizeof(client_addr);

		int new_socket=accept(server_sockfd,(struct sockaddr*)&client_addr,&addrlen);
		if(new_socket==-1)
		{
			if(errno!=EAGAIN){
				perror("accept call failed");
				exit(0);
			}
		}
		else
		{
			int flag2=fcntl(new_socket,F_GETFL);
			flag2|=O_NONBLOCK;
			fcntl(new_socket,F_SETFL,flag2);
			printf("new uer connet\n");
		
		for(int i=0;i<128;i++)
		{
			if(client_sockfd[i]==-1){
				client_sockfd[i]=new_socket;
				inet_ntop(AF_INET,&client_addr.sin_addr,cip,sizeof(cip));
				
				sprintf(response_buffer,"hi %s connect successfully\n",cip);
				send(client_sockfd[i],response_buffer,strlen(response_buffer),MSG_NOSIGNAL);
				break;
			}
		}
		}
		//遍历client_sock数组，读取每一个sock，如果右请求数据则处理，没有继续，遍历到末尾

		for(int i=0;i<128;i++)
		{
			if(client_sockfd[i]!=-1)
			{
				char read_buffer[4096];
				int result=recv(client_sockfd[i],read_buffer,sizeof(read_buffer),0);
				if(result>0)
				{
					read_buffer[result]='\0';
					if(strncmp(read_buffer,"time",4)==0)
					{
						char *time_str=get_current_time();
						send(client_sockfd[i],time_str,strlen(time_str),MSG_NOSIGNAL);

					}
					else
					{
						sprintf(response_buffer,"please try again..\n");
						send(client_sockfd[i],response_buffer,strlen(response_buffer),MSG_NOSIGNAL);
					}
				}
				else if(result==0)
				{

						close(client_sockfd[i]);
						client_sockfd[i]=-1;
				}
				else if(result==-1){
					if(errno!=EAGAIN)
					{
						close(client_sockfd[i]);
						client_sockfd[i]=-1;
					}
				}
				
			}
		}
		//遍历client_sock数组，读取每一个sock，如果右请求数据则处理，没有继续，遍历到末尾
		//复位
	}
}

int main()
{
	server_start(1);
}
