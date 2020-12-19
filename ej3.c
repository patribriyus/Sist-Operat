/* Autor
 * Prueba voluntaria: 3 */

#include <signal.h>
#include <stdio.h>
#include "error.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Divide el comando en la instrucción y parámetros
void dividir(char *arg[], char *comando){
	int i = 0;
	arg[0] = strtok(comando," ");
	while(arg[i] != NULL){
		i++;
		arg[i] = strtok(NULL," ");
	}
}

int main(int argc, char *argv[]){
	int fd = open(argv[1],O_RDONLY), i;	// Abrimos el fichero para leerlo
	char buf[4096], *comando, *arg[50];
	
	read(fd,buf,sizeof(buf));
	int cont = 0;
	comando = strtok(buf,"|\n");	// dividimos el fichero por comandos
	while(comando != NULL){
		if(fork()){
			dividir(arg,comando);
			execvp(arg[0], &arg[0]);
			exit(1);
		}
		comando = strtok(NULL,"|\n");
		cont++;
	}
	for(int i=0; i<cont; i++){
		wait(NULL);
	}
	exit(0);
}