/*Ejercicio 2. Implementa un programa que realice la siguiente funcionalidad. El programa
acepta como argumento el nombre de un archivo (pathname), lo abre y lo lee en bloques de
tamaño 80 Bytes, y crea un nuevo archivo de salida, salida.txt, en el que debe aparecer la
siguiente información:

Bloque 1
// Aquí van los primeros 80 Bytes del archivo pasado como argumento.
Bloque 2
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
...
Bloque n
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.

Si no se pasa un argumento al programa se debe utilizar la entrada estándar como archivo de
entrada.

Modificación adicional. ¿Cómo tendrías que modificar el programa para que una vez
finalizada la escritura en el archivo de salida y antes de cerrarlo, pudiésemos indicar en su
primera línea el número de etiquetas ''Bloque i'' escritas de forma que tuviese la siguiente
apariencia?:

El número de bloques es <no_bloques>
Bloque 1
// Aquí van los primeros 80 Bytes del archivo pasado como argumento.
Bloque 2
// Aquí van los siguientes 80 Bytes del archivo pasado como argumento.
...*/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc, char *argv[]){

	char buffer[1024];			// Declaramos nuestro buffer el cual contendrá la cadena a introducir
	int fd;					// Archivo el cual abriremos en caso de pasarlo por comandos
	int fd2;				// Archivo donde escribiremos
	char cadena[] = "salida.txt";		// Nombre del archivo donde escribiremos
	char bloque[50];			// Indicador de bloque
	int comprobador = 0;			// Comprobador de errores
	int contador = 0;			// Contador de bloques


	// Abrimos primero nuestro archivo de salida, con la posibilidad de crear, entre otras opciones
	if( (fd2=open(cadena,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR)) < comprobador) {	// Abrimos/creamos nuestro archivo de salida
		printf("\nError %d en open",errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	}

	if(argc == 1){				// Vamos a leer de la entrada estándar cuando no tenemos argumentos
        	printf("\nProcede a escribir por consola, para finalizar pulsa CTRL+d\n\n");
		fd = STDIN_FILENO;		// Ponemos nuestro archivo a leer de la entrada estándar --- fd=0
	}
	else {					// En caso de tener intentamos abrir el archivo que nos pasan por argumento
		if( (fd=open(argv[1],O_CREAT,S_IRUSR|S_IWUSR)) < comprobador) {		// Devuelve -1 en caso de fallo
				printf("\nError %d en open",errno);
				perror("\nError en open");
				exit(EXIT_FAILURE);
		}
	}

	while(read(fd, buffer, 80) > comprobador){			// La función read devuelve 0 en el final del archivo y -1 en caso de error por lo que ponemos como acierto >0
		snprintf(bloque, 15, "\nBloque %d: \n\n", contador);
		write(fd2, bloque, 15);

		if(write(fd2, buffer, 80) < comprobador)		// Devuelve -1 en caso de error
			printf("\nError al escribir");
		contador++;
	}

	if(lseek(fd2,0,SEEK_SET) < 0) {
		perror("\nError en lseek");
		exit(EXIT_FAILURE);
	}

	snprintf(buffer, 30, "\nEl número de bloques es %d: \n", contador);
	write(fd2, buffer, 30);

	// Cerramos los archivos
	close(fd);
	close(fd2);

	return EXIT_SUCCESS;
}
