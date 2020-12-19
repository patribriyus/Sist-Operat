/* Autor
 * Prueba voluntaria: 2 (señales no seguras) */

#include <signal.h>
#include <stdio.h>
#include "error.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char proceso;
int turno = 0;

void f(){
	signal(SIGUSR1,f);
	turno = 1;
	fprintf(stderr,&proceso);
	fprintf(stderr,"-");
}

int main(){
	int pid, ppid;

	signal(SIGUSR1,f);

	if((pid=fork())==0){	// proceso hijo
		proceso = 'h';
		ppid = getppid();
		pid=getppid();
		if((pid=fork())==0){	// proceso nieto
			pid = ppid;
			proceso = 'n';
		}
	}
	else{	// proceso padre
		proceso = 'p';
		turno = 1;
		sleep(1);	// espera al nieto y al hijo
		
	}
	while(1){
		if(turno == 1){
			turno = 0;
			kill(pid,SIGUSR1);
			pause();
		}
	}
}