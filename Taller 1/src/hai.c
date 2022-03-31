#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int k=0;
void h(int n){
	k++;
	printf("Ya va!\n");
}
void f(int n){
	wait(NULL);	//ESPERAMOS EL CAMBIO DEL HIJO
	exit(1);
}

void fhandler(int n){
}
int main(int argc, char* argv[]) {
  	pid_t pid=fork();
	if(pid==0){	//Hjo
		signal(SIGURG, h);
		while(1){
			if(k==5){
				kill(getppid(), SIGINT);
				execvp(argv[1], argv+1);
			}
		}
	}
	else{
		signal(SIGINT, f);
		while(1){
			printf("Sup!\n");
			kill(pid, SIGURG);
			sleep(1);
		}

	}
	return 0;
}
