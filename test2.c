#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>


int main(int argc,char *argv[]){

		//pps를 코드로 실행시키는 방법
		pid_t pid;

		if((pid=fork())==0){

				if(execl("./pps","pps",(const char *)0)==-1)
						printf("exec failed\n");
		}
		while(wait((int *)0)!=-1);
		exit(0);

	

}
