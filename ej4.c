/* Autor
 * Prueba voluntaria: 4 */

#include <signal.h>
#include <stdio.h>
#include "error.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

struct datos{
	double init, tam_trozo, resultado;
	int tro_x_thr;
};

double area(const double init, const double tam_trozo){
	return tam_trozo * (4.0/(1+pow(init,2)));
}

void *func(void *arg){
	pthread_t t;
	struct datos *p = (struct datos *)arg;
	p->resultado = 0.0;
	
	for(int i=0; i<p->tro_x_thr; i++){
		p->resultado += area(p->init,p->tam_trozo);
		p->init += p->tam_trozo;
	}
	
	pthread_exit(NULL);
}

int main(int argc,char* argv[]) {
		
	int n_trozos = atoi(argv[1]);	// numero de trozos
	int n_threads = atoi(argv[2]);	// numero de threads
	
	pthread_t *t = calloc(n_threads,sizeof(pthread_t));
	struct datos *p = calloc(n_threads,sizeof(struct datos));
	
	int tro_x_thr = n_trozos / n_threads;	// numero de trozos por thread
	int resto = n_trozos % n_threads;	// trozos que sobran
	double tam_trozo = 1.0 / n_trozos;	// tamaño de cada trozo
	double init = 0.0;	// cada trozo empezará su cálculo en un punto diferente
	double total = 0.0;
	
	for(int i=0; i<n_threads; i++){	// ejecución de los threads
	// construimos la estructura de datos
		p[i].tam_trozo = tam_trozo;
		p[i].init = init;
		
		if(i > resto){ p[i].tro_x_thr = tro_x_thr; }
		else{ p[i].tro_x_thr = tro_x_thr + 1; }
		
		init += tam_trozo * p[i].tro_x_thr; // actualizamos el valor del empiece
		if(pthread_create(&t[i],NULL,func,&p[i]) != 0){	// Lanzamos thread
			syserr("Erro al crear el thread");
		}
	}
	
	for(int i=0; i<n_threads; i++){
		pthread_join(t[i],NULL);
		total += p[i].resultado;
	}
	
	printf("Trozos: %d\nThreads: %d\nValor de la integral: %lf\n",n_trozos,n_threads,total);
	exit(0);
}