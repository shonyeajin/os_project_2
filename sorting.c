#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>



int main(int argc,char *argv[]){


		uid_t user_id;
		struct passwd *user_pw;

		user_id=getuid();
		user_pw=getpwuid(user_id);
		printf("%s\n",user_pw->pw_name);



		if(argc<2){
				printf("argument fault\n");
				exit(1);
		}
		if(!strcmp(argv[1],"a")){
		
				printf("a option\n");
		}else if(!strcmp(argv[1],"u")){
		
				printf("u option\n");
		}else if(!strcmp(argv[1],"x")){
		
				printf("x option\n");
		}else if(!strcmp(argv[1],"au")|!strcmp(argv[1],"ua")){
		
				printf("au option\n");
		}else if(!strcmp(argv[1],"ax")|!strcmp(argv[1],"xa")){
		
				printf("ax option\n");
		}else if(!strcmp(argv[1],"ux")|!strcmp(argv[1],"xu")){
		
				printf("ux option\n");
		}else if(!strcmp(argv[1],"aux")|!strcmp(argv[1],"axu")|!strcmp(argv[1],"uax")|!strcmp(argv[1],"xau")|!strcmp(argv[1],"uxa")|!strcmp(argv[1],"xua")){
		
				printf("aux option\n");
				
		}else{
				printf("잘못된 옵션 입력\n");
		}











}
