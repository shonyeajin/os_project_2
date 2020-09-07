#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

void exitfunc(int signo);



//splits the string by space and returns the array of tokens
//스트링을 스페이스 단위로 쪼개서 토큰들의 배열로 return, tokens배열에 담아서 리턴, 마지막 token다음칸엔 NULL 담겨있음

char **tokenize(char *line)
{
		char **tokens=(char **)malloc(MAX_NUM_TOKENS *sizeof(char *));
		char *token=(char *)malloc(MAX_TOKEN_SIZE *sizeof(char));
		int i,tokenIndex=0,tokenNo=0;
		
		for(i=0;i<strlen(line);i++){

				char readChar=line[i];

				if(readChar==' ' || readChar=='\n' || readChar=='\t'){

						token[tokenIndex]='\0';
						if(tokenIndex !=0){

								tokens[tokenNo]=(char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
								strcpy(tokens[tokenNo++],token);
								tokenIndex=0;


						}
						

				}else{

						token[tokenIndex++]=readChar;


				}

		}
		free(token);
		tokens[tokenNo]=NULL;
		return tokens;


}


int main(int argc, char *argv[]){
		char line[MAX_INPUT_SIZE];
		char **tokens;
		int i;
		FILE* fp;
		char *pipe="|";

		if(argc==2){//배치식일때 
				fp=fopen(argv[1],"r");
				if(fp<0){
						printf("File doesn't exists.");
						return -1;
				}
		}

		//나중에 삭제하기
		signal(SIGINT,exitfunc);


		while(1){
				//BEGIN: TAKING INPUT
				bzero(line, sizeof(line));
				if(argc==2){//batch mode
						if(fgets(line, sizeof(line),fp)==NULL){//file reading finished
								break;

						}
						line[strlen(line)-1]='\0';//line의 마지막에 공백 넣음

				}else{//interactive mode
						printf("$");
						scanf("%[^\n]",line);//enter까지 읽어들여서 line에 저장
						getchar();//stdin에서 한글자 가져옴
				}
				printf("Command entered: %s (remove this debug output later)\n",line);
				//END: TAKING INPUT
				
				line[strlen(line)]='\n';	//terminate with new line
				tokens=tokenize(line);

				//do whatever you want with the commands, here we just print them

				for(i=0;tokens[i]!=NULL;i++){
						printf("found token %s (remove this debug output later)\n",tokens[i]);

				}


				//line에 pipe line 포함되어 있으면 따로 처리해줘야 함

				if(strstr(line,pipe)!=NULL){	
						int numArr[10]={0,};//배열의 모든 요소를 0으로 초기화
						int num=0;//line에 포함된 pipe의 갯수

						for(i=0;tokens[i]!=NULL;i++){
								if(!strcmp(tokens[i],"|")){
										numArr[num++]=i;
										
								}
						}
						/*

						i=0;
						while(numArr[i]!=0){
								printf("%d번째 토큰이 파이프임\n",numArr[i]);
								i++;
						}
						*/

						
						for(int j=0;j<num;j++){
								printf("파이프 작업\n");
								int des_p[2];
								if(pipe(des_p)==-1){
										perror("pipe failed\n");
										exit(1);
								}

								if(fork()==0){
										close(STDOUT_FILENO);
										dup(des_p[1]);
										close(des_p[0]);
										close(des_p[1]);


								}


						}
						



				}else{

						//fork시키고, exec 시키고 wait시킴
						pid_t pid;
						i=0;

						
						
						
						if((pid=fork())==0){
								if(execvp(tokens[0],tokens)==-1){
										printf("exec failed\n");

								}

						}

						while(wait((int *)0)!=-1);
						printf("parent: children terminated, my pid:%d\n",getpid());
						

						
						//Freeing the allocated memory
						for(i=0;tokens[i]!=NULL;i++){
								free(tokens[i]);
						}
						free(tokens);



				}




		}
}




void exitfunc(int signo){
		printf("SIGINT\n");
		raise(SIGKILL);

}
