#include<regex.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<string.h>
#include<unistd.h>
int main(){
	char *regStr="<a[^>]*\\(class=\"\\([^\"]*\\)\"[^>]*\\)\\?href=\"\\([^\"]*\\)\"\\([^>]*target=\"\\([^\"]*\\)\"\\)\\?\\([^>]*title=\"\\([^\"]*\\)\"\\)\\?[^>]*>\\([^<]*\\)</a>";
	regex_t reg;
	regcomp(&reg,regStr,0);
	int fd=open("URL.txt",O_RDWR);

	int size=lseek(fd,0,SEEK_END);
	char*mmap_ptr=NULL;
	mmap_ptr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	close(fd);

	int regnum=9;
	regmatch_t match[regnum];
	char class[1024];
	char href[1024];
	char target[1024];
	char text[1024];
	char title[1024];
	while((regexec(&reg,mmap_ptr,regnum,match,0))==0){
		bzero(target,sizeof(target));
		bzero(href,sizeof(href));
		bzero(class,sizeof(class));
		bzero(text,sizeof(text));
		bzero(title,sizeof(title));
		if(match[2].rm_so!=-1){
			snprintf(class,match[2].rm_eo-match[2].rm_so+1,"%s",mmap_ptr+match[2].rm_so);

		}
		snprintf(href,match[3].rm_eo-match[3].rm_so+1,"%s",mmap_ptr+match[3].rm_so);
		if(match[5].rm_so!=-1){
			snprintf(target,match[5].rm_eo-match[5].rm_so+1,"%s",mmap_ptr+match[5].rm_so); 
        }
		if(match[7].rm_so!=-1){
             snprintf(title,match[7].rm_eo-match[7].rm_so+1,"%s",mmap_ptr+match[7].rm_so);
		 }
		snprintf(text,match[8].rm_eo-match[8].rm_so+1,"%s",mmap_ptr+match[8].rm_so);

		printf("class: %s href: %s target: %s title: %s text: %s\n",class,href,target,title,text);
		mmap_ptr+=match[0].rm_eo;
	}
	return 0;
}
