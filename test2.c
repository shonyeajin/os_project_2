#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
		char term[100]="/proc/2151";
		char *tmp;
		tmp=ctermid(term);
		printf("%s\n",tmp);


}
