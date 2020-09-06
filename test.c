#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
		//자식 프로세스에 받은 입력을 전달해주고 자식은 수행한 후 exit하고 부모는 wait한다.
		pid_t pid;

		if(argc==1){
				if((pid=fork())==0){
						char *args[]={"hihi",0};
						//if(execl("/bin/sh","-c","cd",(const char *)0)==-1){
						if(execlp("cat","cat","commands.txt",(const char *)0)==-1){
								printf("exec failed\n");
						}

				}
				while(wait((int *)0)!=-1);
				printf("parent: all children terminated, my pid:%d\n",getpid());
				exit(0);


				


		}


}

