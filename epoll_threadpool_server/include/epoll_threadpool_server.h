#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<errno.h>
typedef struct
{
	void*(*busines)(void*);
	void*arg;
}task_t;

typedef struct
{
	int thread_shutdown;//线程池开关，开关关闭，所有线程退出
	int thread_max;//线程最大值
	int thread_min;//最小值，预创建
	int thread_busy;//获取任务并执行的线程
	int thread_alive;//有效线程数
	int thread_exitcode;//缩减值
	//队列相关
	int front;
	int reat;
	int cur;
	int max;
	task_t*list;//队列地址
	//线程安全相关
	pthread_mutex_t lock;
	pthread_cond_t NOT_FULL;
	pthread_cond_t NOT_EMPTY;
	//tids
	pthread_t*ctids;//存储消费者tid
	pthread_t mid;//管理线程tid
}pool_t;

int epfd;//全局的监听树描树赋

int server_start(int shutdown);//服务器启动端口
int net_initializer(char*ip,int port);//网络初始化
int first_response(int sockfd ,char*cip);//首次响应
int epoll_initializer(int sockfd);
int epoll_listen(int server_sock,pool_t*pt);
int producer_add_task(pool_t*pt,task_t ts);//调用一次添加任务(producer)
pool_t * thread_pool_initializer(int tmax,int tmin,int qmax,int shutdown);//线程池创建初始化
int if_thread_alive(pthread_t);//判断线程是否存活
void *consumer_bs(void*arg);//消费者线程函数
void* manager_bs(void *arg);//管理线程任务函数
void *accept_busines(void*arg);//IO处理，连接
void *response_busines(void*arg);//IO处理，读写sock
