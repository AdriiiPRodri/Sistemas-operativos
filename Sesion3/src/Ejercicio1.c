//Ejercicio1.c
//Trabajo con llamadas al sistema del Subsistema de Procesos "POSIX 2.10 compliant"

#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

pid_t pid;
int i;
sscanf(argv[1], "%d", &i);

if(argc == 2) {
	if( (pid=fork())<0) {
		perror("\nError en el fork");
		exit(-1);
	}
	else if(pid==0) {  //proceso hijo ejecutando el programa
		if(i % 2 == 0)
			printf("\n%i es un número par.\n", i);
		else
			printf("\n%i es un número impar.\n", i);
	}else  //proceso padre ejecutando el programa
		if(i % 4 == 0)
			printf("\n%i es divisible por 4.\n", i);
		else
			printf("\n%i no es divisible por 4.\n", i);
}
else
	printf("\nDebes especificar un número como argumento.\n");

exit(0);
}
