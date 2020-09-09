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
		char *pipestr="|";

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

				if(strstr(line,pipestr)!=NULL){	
						int numArr[10]={0,};//배열의 모든 요소를 0으로 초기화
						int num=0;//line에 포함된 pipe의 갯수

						//파이프가 몇개고, 파이프가 line에서 몇번쨰 토큰인지 알아냄
						for(i=0;tokens[i]!=NULL;i++){
								if(!strcmp(tokens[i],"|")){
										numArr[num++]=i;
										
								}
						}
						printf("파이프갯수:%d\n",num);
						//파이프 갯수 +1만큼의 subline으로 쪼개기
						char *sublineptr[MAX_INPUT_SIZE/2];
						char *tmp;
						i=0;

						tmp=strtok(line,"|");
						while(tmp)
						{
								sublineptr[i]=tmp;
								printf("sublineptr[%d]:%s\n",i,sublineptr[i]);
								tmp=strtok(NULL,"|");
								i++;
						}
						i=0;

						//line쪼개는거는 함, 쓸 떼 마지막에  마지막에 \n 붙이는거 잊지말기
						char subline[MAX_INPUT_SIZE/2];	
						char **subtokens;




						//fork & exec
						//pipe 갯수만큼 생성함
	/*					int *intArr[MAX_NUM_TOKENS];

						for(i=0;i<num;i++){
								int des_p[2];
								intArr[i]=des_p;

								if(pipe(intArr[i])==-1){
										perror("pipe failed\n");
										exit(1);
								}
						}
	*/



						int intArr[MAX_NUM_TOKENS][2];
						for(i=0;i<num;i++){
								if(pipe(intArr[i])==-1){
										perror("pipe failed\n");
										exit(1);

								}

						}





						printf("파이:%d\n%d\n%d\n%d\n",intArr[0][0],intArr[0][1],intArr[1][0],intArr[1][1]);

						//fork문 작성
						//first fork statement
						if(fork()==0){
								close(STDOUT_FILENO);
								dup(intArr[0][1]);

								//close(intArr[0][0]);
								//close(intArr[0][1]);

								close(intArr[0][0]);
								for(i=1;i<num;i++){
										close(intArr[i][0]);
										close(intArr[i][1]);
								}



								strcpy(subline,sublineptr[0]);
								subline[strlen(subline)]='\n';
								subtokens=tokenize(subline);

								if(execvp(subtokens[0],subtokens)==-1){
									printf("exec failed\n");

								}
								perror("execvp of first failed\n");
								exit(1);

	




						}
						wait(0);



						//medium fork statement
						for(i=0;i<num-1;i++){
								if(fork()==0)
								{

										dup2(intArr[i][0],STDIN_FILENO);
										close(STDIN_FILENO);
										close(intArr[i][0]);



										dup2(intArr[i+1][1],STDOUT_FILENO);
										close(STDOUT_FILENO);
/*
										close(STDIN_FILENO);
										dup(intArr[i][0]);
										close(intArr[i][0]);

										close(STDOUT_FILENO);
										dup(intArr[i+1][1]);
*/										

										for(int j=0;j<num;j++){
												if(j==(i+1)){
														close(intArr[j][0]);
												}else{
														close(intArr[j][0]);
														close(intArr[j][1]);
												}
										}



										strcpy(subline,sublineptr[i+1]);
										subline[strlen(subline)]='\n';
										subtokens=tokenize(subline);



										if(execvp(subtokens[0],subtokens)==-1){
											printf("exec failed\n");

										}
										perror("execvp of medium failed\n");
										exit(1);



								}
								wait(0);
								

						}

						//last fork statement
						if(fork()==0){

								//char buf[MAX_INPUT_SIZE];
								//int buf_size=read(intArr[num-1][0],buf,sizeof(buf));
								//printf("buf_size: %d\n",buf_size);
								//printf("buf_input:%s\n",buf);


								
								close(STDIN_FILENO);
								dup(intArr[num-1][0]);

								//close(intArr[num-1][1]);
								//close(intArr[num-1][0]);
								for(i=0;i<num;i++){
										close(intArr[i][0]);
										close(intArr[i][1]);
								}



								strcpy(subline,sublineptr[num]);
								subline[strlen(subline)]='\n';
								subtokens=tokenize(subline);

								printf("subline:%s\n",subline);
								for(i=0;subtokens[i]!=NULL;i++){
										printf("subtokens[%d]:%s\n",i,subtokens[i]);


								}

								if(execvp(subtokens[0],subtokens)==-1){
									perror("exec failed\n");

								}
								perror("execvp of second failed\n");
								exit(1);

	




						}

						//close(intArr[0][0]);
						//close(intArr[0][1]);
						for(i=0;i<num;i++){
								close(intArr[i][0]);
								close(intArr[i][1]);
						}

						wait(0);
						//while(wait((int *)0)!=-1);
						printf("parent: all children terminated, my pid:%d\n",getpid());

/*
						//메모리 free

						for(i=0;subtokens[i]!=NULL;i++){
								free(subtokens[i]);
						}

						free(subtokens);

*/









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
						

				}
						


				//Freeing the allocated memory
				for(i=0;tokens[i]!=NULL;i++){
						free(tokens[i]);
				}
				free(tokens);









		}
}




void exitfunc(int signo){
		printf("SIGINT\n");
		raise(SIGKILL);

}
