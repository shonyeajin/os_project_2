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
	char p_RSS[20];
	char p_command[200];
	char p_uid[10];
	char p_gid[10];
	char p_user_num[20];
	char p_user[20];
	char p_stime[10];
	char p_ujiff[20];
	char p_sjiff[20];
	int p_time;
	int p_time_min;
	int p_time_sec;
	double p_cpu;
	double p_mem;
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

										char buf2[1024];


										//TIME 얻기 위해
										char *p_ujiff;
										strcpy(buf2,buf);
										p_ujiff=strtok(buf," ");
										for(int tokIdx=0;tokIdx<13;tokIdx++){
												p_ujiff=strtok(NULL," ");
										}
										strcpy(p.p_ujiff,p_ujiff);
										p_ujiff=strtok(NULL," ");
										strcpy(p.p_sjiff,p_ujiff);

										p.p_time=atoi(p.p_ujiff)+atoi(p.p_sjiff);
										//분초로 나눠서 계산하기
										int p_min=0;
										int p_sec=p.p_time;
										while(p_sec>=6000){
												p_sec-=6000;
												p_min++;
										}
										p.p_time_min=p_min;
										p.p_time_sec=p_sec/100;

										//출력할 때 이런식으로 출력하면 됨
										//printf("TIME=%d:%d\n",p.p_time_min,p.p_time_sec/100);








										

	



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
										memset(buf2,0,sizeof(buf2));
										strcpy(buf2,buf);
										p_state=strstr(buf2,"State");
										p_state=strtok(p_state,"\t");
										p_state=strtok(NULL," ");
										strcpy(p.p_state,p_state);

										char *p_VSZ;
										memset(buf2,0,sizeof(buf2));
										strcpy(buf2,buf);
										if((p_VSZ=strstr(buf2,"VmSize"))!=NULL){
												p_VSZ=strtok(p_VSZ," ");
												p_VSZ=strtok(NULL," ");
												strcpy(p.p_VSZ,p_VSZ);
										}else{
												strcpy(p.p_VSZ,"0");
										}

										char *p_RSS;
										memset(buf2,0,sizeof(buf2));
										strcpy(buf2,buf);
										if((p_RSS=strstr(buf2,"VmRSS"))!=NULL){
												p_RSS=strtok(p_RSS," ");
												p_RSS=strtok(NULL," ");
												strcpy(p.p_RSS,p_RSS);
										}else{
												strcpy(p.p_RSS,"0");
										}
										
										char *p_command;
										memset(buf2,0,sizeof(buf2));
										strcpy(buf2,buf);
										p_command=strstr(buf2,"Name");
										p_command=strtok(p_command,"\t");
										p_command=strtok(NULL,"\n");
										strcpy(p.p_command,p_command);

										char p_user_num[20];
										char *p_uid;
										char *p_gid;
										memset(buf2,0,sizeof(buf2));
										strcpy(buf2,buf);
										p_uid=strstr(buf2,"Uid");
										p_uid=strtok(p_uid,"\t");
										p_uid=strtok(NULL,"\t");
										strcpy(p.p_uid,p_uid);

										memset(buf2,0,sizeof(buf2));
										strcpy(buf2,buf);
										p_gid=strstr(buf2,"Gid");
										p_gid=strtok(p_gid,"\t");
										p_gid=strtok(NULL,"\t");
										strcpy(p.p_gid,p_gid);
										
										strcpy(p_user_num,p.p_uid);
										strcat(p_user_num,":");
										strcat(p_user_num,p.p_gid);
										strcpy(p.p_user_num,p_user_num);

										


	
										fclose(fp);
										//-------------------------
										if((fp=fopen("./cmdline","r"))==NULL){
												printf("fopen error\n");
												exit(1);
										}

										memset(buf,0,sizeof(buf));
										if(fread(buf,sizeof(buf),1,fp)<0){
												printf("fread error\n");
												exit(1);
										}
										if(strlen(buf)==0){
												strcpy(buf,"[");
												strcat(buf,p.p_command);
												strcat(buf,"]");
												strcpy(p.p_command,buf);
										}else{
												//memset(p.p_command,0,sizeof(p.p_command));
												strcpy(p.p_command,buf);
										}
										
													

										fclose(fp);
										//---------------/etc로 이동
										chdir("/etc");
										if((fp=fopen("./passwd","r"))==NULL){
												printf("fopen error\n");
												exit(1);
										}

										memset(buf,0,sizeof(buf));
	/*									if(fread(buf,sizeof(buf),1,fp)<0){
												printf("fread error\n");
												exit(1);
										}

										char *p_user;

										while((p_user=strstr(buf,p.p_user_num))==NULL){
												memset(buf,0,sizeof(buf));
												if(fread(buf,sizeof(buf),1,fp)<0){
														printf("fread error\n");
														exit(1);
												}
										}
										p_user=strtok(p_user,":");
										printf("user: %s\n",p_user);
*/
										char *p_user;
										fgets(buf,sizeof(buf),fp);
										while(strstr(buf,p.p_user_num)==NULL){
												memset(buf,0,sizeof(buf));
												if(fgets(buf,sizeof(buf),fp)==NULL){
														//못찾았으면 적어도 uid만으로라도 sorting해서 user값 넣어주기
														rewind(fp);
														fgets(buf,sizeof(buf),fp);
														while(strstr(buf,p.p_uid)==NULL){
																memset(buf,0,sizeof(buf));
																if(fgets(buf,sizeof(buf),fp)==NULL){
																		
																	printf("못찾음\n");
																	break;
																}
														}

														break;
												}

										}
										p_user=strtok(buf,":");
										strcpy(p.p_user,p_user);





										//STIME을 구조체에 저장
										char p_atime[100];
										char *p_stime;

										strcpy(p_atime,ctime(&fstat.st_atime));
										p_stime=strtok(p_atime," ");
										p_stime=strtok(NULL," ");
										p_stime=strtok(NULL," ");
										p_stime=strtok(NULL," ");

										strcpy(p_atime,p_stime);
										p_atime[5]='\0';
										p_atime[6]='\0';
										strcpy(p.p_stime,p_atime);




										fclose(fp);
										chdir("/proc");

										//%CPU 얻기 위해서
										// ----/proc디렉토리로 이동하여 stat파일 열음
										if((fp=fopen("./stat","r"))==NULL){
												printf("fopen error\n");
												exit(1);
										}

										memset(buf,0,sizeof(buf));

										//맨 첫줄 읽어오고 토큰화 해서 값 더해서 cpu_total값
										
										if(fgets(buf,sizeof(buf),fp)==NULL){
												printf("fgets error\n");
												exit(1);

										}
										char *cputok;
										int cpu_jiff_sum=0;
										cputok=strtok(buf," ");
										for(int i=0;i<4;i++){
												cputok=strtok(NULL," ");
												cpu_jiff_sum+=atoi(cputok);

										}

										p.p_cpu=((p.p_time*100.0)/cpu_jiff_sum);
										//출력할 때 아래처럼 출력하면 됨
										//printf("CPU(persent): %1.1lf\n",((p.p_time*100.0)/cpu_jiff_sum));

										fclose(fp);

										//mem_percentage계산하기 위해
										//-------------/proc/meminfo에서 총 메모리량 가져옴
										if((fp=fopen("./meminfo","r"))==NULL){
												printf("fopen error\n");
												exit(1);
										}

										memset(buf,0,sizeof(buf));
										if(fread(buf,sizeof(buf),1,fp)<0){
												printf("fread error\n");
												exit(1);
										}
									
										char *mem_sum;

										mem_sum=strtok(buf," ");
										mem_sum=strtok(NULL," ");

										p.p_mem=(atoi(p.p_RSS)*100.0)/atoi(mem_sum);
										//printf("mem: %lf\n",p.p_mem);



	

										fclose(fp);

										//마지막에 항상 /proc디렉토리로 이동한 후 끝내야함						
										ProcInfoArr[ProcInfoIdx++]=p;


								}

									


						}

				}
		}
		closedir(dirp);

		for(int i=0;i<ProcInfoIdx;i++){
				printf("user:%s, pid:%d, state:%s, VSZ:%skB, RSS:%skB, COMMAND:%s, STIME:%s, TIME:%d:%d, cpu_persent:%1.1lf, mem_persent:%1.1lf\n",ProcInfoArr[i].p_user,*ProcInfoArr[i].p_pid,ProcInfoArr[i].p_state,ProcInfoArr[i].p_VSZ,ProcInfoArr[i].p_RSS,ProcInfoArr[i].p_command,ProcInfoArr[i].p_stime,ProcInfoArr[i].p_time_min,ProcInfoArr[i].p_time_sec,ProcInfoArr[i].p_cpu,ProcInfoArr[i].p_mem);


		}



}
