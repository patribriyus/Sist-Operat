/* Autor
 * Prueba voluntaria: 1 */

#include <signal.h>
#include <stdio.h>
#include "error.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc,char* argv[]) {
	char letra;

	int n = atoi(argv[1]);	// numero de procesos
	int m = atoi(argv[2]);	// numero de caracteres
	
	for(int i=0; i<n; i++){	// creamos los procesos
		letra = 97 + i;
		if(fork()==0){
			for(int j=0; j<m; j++){
				write(1,&letra,1);
			}
			exit(1);
		}
	}
	
	for(int i=0; i<n; i++){
		wait(NULL);
	}
	
	printf("\n");
	exit(0);
}