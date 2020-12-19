/* Autor
 * Prueba voluntaria: 2 (señales seguras) */

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
	turno = 1;
	fprintf(stderr,&proceso);
	fprintf(stderr,"-");
}

int main(){
	int pid, ppid;

	// declaro las funciones de sigset_t que voy a usar
	sigset_t set1, set2;
	sigemptyset(&set1);	// inicializa un conjunto de señales vacio
	sigemptyset(&set2);
	sigaddset(&set1,SIGUSR1);	// añade la señal SIGUSR1
	sigdelset(&set2,SIGUSR1);	// elimina la señal SIGUSR1
	
	struct sigaction act;	
	act.sa_handler = &f;
	act.sa_flags = SA_RESTART;
	
	sigaction (SIGUSR1, &act, NULL);

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
			sigprocmask(SIG_SETMASK,&set1,NULL);	// bloqueo de SIGUSR1
			kill(pid,SIGUSR1);
			sigsuspend(&set2);	// desbloqueo de SIGUSR1
		}
	}
}