#include<process_copy.h>

int process_create(const char* srcfile,const char* destfile,int pronum,int blocksize){
	pid_t pid;
	int i=0;
	int offset;
	for(i=0;i<pronum;i++){
		pid=fork();
		if(pid==0)
			break;
	}
	if(pid>0){
		printf("parent process %d alive\n",getpid());
		sleep(1);
		process_wait();
	}else if(pid==0){
		printf("child process %d alive\n",getpid());
		offset=i*blocksize;
		char str_offset[100];
		sprintf(str_offset,"%d",offset);
		char str_blocksize[100];
		sprintf(str_blocksize,"%d",blocksize);
		execl("/home/colin/20250915/Process/process_copy/MOD/copy","copy",srcfile,destfile,str_offset,str_blocksize,NULL);
		if((execl("/home/colin/20250915/Process/process_copy/MOD/copy","copy",srcfile,destfile,str_offset,str_blocksize,NULL))==-1)
			perror("execl call error");
	}else {
		perror("fork() error");
	}

}
