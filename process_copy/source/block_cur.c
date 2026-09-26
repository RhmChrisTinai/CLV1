#include<process_copy.h>

int block_cur(const char* srcfile,int pronum){
	int filesize;
	struct stat st;
	stat(srcfile,&st);
	filesize=(int)st.st_size;
	int persize;
	if(filesize%pronum==0){
		persize=filesize/pronum;
		return persize;
	}else{
		persize=filesize/pronum+1;
		return persize;
	}
}
