#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>

typedef struct _ProcInfo{
	int p_pid[10];
	char p_state[10];
	char p_name[50];
	char p_VSZ[20];
	//char p_stat[1];
} ProcInfo;

ProcInfo ProcInfoArr[200];
FILE *fp;
char buf[1024];
int ProcInfoIdx=0;


int main(int argc, char *argv[]){

		struct dirent *dentry;
		struct stat fstat;
		DIR *dirp;
		char dirname[100];

		if(chdir("/proc")<0){
				printf("chdir error\n");
				exit(1);
		}
		if((dirp=opendir("."))==NULL){
				printf("open directory error\n");
				exit(1);
		}
		while(dentry=readdir(dirp)){
				if(dentry->d_ino!=0){
						if((!strcmp(dentry->d_name,"."))||(!strcmp(dentry->d_name,"..")))
								continue;
						stat(dentry->d_name,&fstat);
						if(S_ISDIR(fstat.st_mode)){
								memset(dirname,0,sizeof(dirname));
								strcpy(dirname,dentry->d_name);
								if(isdigit(dirname[0])){
										//printf("%s\n",dentry->d_name);
										//해당프로세스의 정보를 얻기위해 이동

	
										ProcInfo p;
										*p.p_pid=atoi(dentry->d_name);
										//p.p_pid=atoi(dentry->d_name);
										//strcpy(p.p_stat,buf);
										


										chdir(dentry->d_name);
										if((fp=fopen("./stat","r"))==NULL){
												printf("fopen error\n");
												exit(1);
										}

										memset(buf,0,sizeof(buf));
										if(fread(buf,sizeof(buf),1,fp)<0){
												printf("fread error\n");
												exit(1);
										}
										//stat에서 추출할 정보 추출하는 영역



										fclose(fp);
										//-------------------------
										if((fp=fopen("./status","r"))==NULL){
												printf("fopen error\n");
												exit(1);
										}

										memset(buf,0,sizeof(buf));
										if(fread(buf,sizeof(buf),1,fp)<0){
												printf("fread error\n");
												exit(1);
										}
										//status에서 추출할 정보 추출하는 영역

										char *p_state;
										p_state=strstr(buf,"State");
										p_state=strtok(p_state," ");
										p_state=strtok(p_state," ");
										strcpy(p.p_state,p_state);

										char *p_VSZ;
										if(p_VSZ=strstr(buf,"VmSize:")){
												p_VSZ=strtok(p_VSZ," ");
												p_VSZ=strtok(p_VSZ," ");
												strcpy(p.p_VSZ,p_VSZ);
										}else{
												strcpy(p.p_VSZ,"0");
										}

									

										fclose(fp);
										chdir("..");
										
										
																			
										ProcInfoArr[ProcInfoIdx++]=p;


								}

									


						}

				}
		}
		closedir(dirp);

		for(int i=0;i<ProcInfoIdx;i++){
				printf("pid:%d, state:%s, VSZ:%s\n",*ProcInfoArr[i].p_pid,ProcInfoArr[i].p_state,ProcInfoArr[i].p_VSZ);


		}



}
