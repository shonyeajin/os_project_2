#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>


int main(int argc,char *argv[]){
		struct stat sb;
		char term[100]="/proc/1687";
		if(stat(term,&sb)==-1){
				perror("stat error\n");
				return 1;
		}

		printf("last access time:%s\n",ctime(&sb.st_atime));
		return 0;


}
