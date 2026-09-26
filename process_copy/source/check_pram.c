#include<process_copy.h>
int check_pram(int argc,const char* srcfile,int pronum){
	if(argc<3){
		perror("parameter number error\n");
		exit(0);
	}
	int src;//承接access函数返回值
	src=access(srcfile,F_OK);//判源文件是否存在

	if(src!=0){
		perror("srcfile not exist\n");
		exit(0);
	}
	if(pronum<3||pronum>100){
		perror("pronum illegal\n");
		exit(0);
	}
	return 1;
}

