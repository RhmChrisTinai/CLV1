#include<process_copy.h>

void process_wait()
{
	pid_t zpid;
	while((zpid=wait(NULL))>0){
		printf("parents wait success,zpid=%d\n",zpid);
	}

}
