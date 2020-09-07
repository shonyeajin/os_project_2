#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
		int des_p[2];
		if(pipe(des_p)==-1){
				perror("pipe failed\n");
				exit(1);
		}

		if(fork()==0)//first fork
		{
				close(STDOUT_FILENO);//closing stdout
				dup(des_p[1]);//replacing stdout with pipe write
				close(des_p[0]);//closing pipe read
				close(des_p[1]);

				char* prog1[]={"ls","-l",(char *)0};
				execvp(prog1[0],prog1);
				perror("execyp of ls failed\n");
				exit(1);
		}

		if(fork()==0)//creating second child
		{
				close(STDIN_FILENO);//closing stdin
				dup(des_p[0]);//replacing stdin with pipe read
				close(des_p[1]);//closing pipe write
				close(des_p[0]);

				char* prog2[]={"wc","-l",(char *)0};
				execvp(prog2[0],prog2);
				perror("execvp of wc failed");
				exit(1);

		}
		close(des_p[0]);
		close(des_p[1]);
		printf("parent's stdout fd is open\n");
		wait(0);
		wait(0);
		return 0;


}

