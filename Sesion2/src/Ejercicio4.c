/*
Ejercicio3.c
Ejercicio 3. Programa una nueva orden que recorra la jerarquía de subdirectorios existentes a
partir de uno dado como argumento y devuelva la cuenta de todos aquellos archivos regulares
que tengan permiso de ejecución para el grupo y para otros. Además del nombre de los archivos
encontrados, deberá devolver sus números de inodo y la suma total de espacio ocupado por
dichos archivos. El formato de la nueva orden será:
$> ./buscar <pathname>
donde <pathname> especifica la ruta del directorio a partir del cual queremos que empiece a
analizar la estructura del árbol de subdirectorios. En caso de que no se le de argumento, tomará
como punto de partida el directorio actual. Ejemplo de la salida después de ejecutar el
programa:
Los i-nodos son:
./a.out 55
./bin/ej 123
./bin/ej2 87
...
Existen 24 archivos regulares con permiso x para grupo y otros
El tamaño total ocupado por dichos archivos es 2345674 bytes
*/

//La estructura struct dirent conforme a POSIX 2.1 es la siguiente:
#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include <string.h>
#include<errno.h>
#include<stdlib.h>
#include "dirent.h"  //cabecera necesaria para el tratamiento de directorios
#define _XOPEN_SOURCE 500
#include <ftw.h>

unsigned long int n_files;
unsigned long int ino_files;

int Buscar(const char* path, const struct stat* stat, int flags, struct FTW* ftw){        // Función a la que llamará reiteradamente nftw
  if(S_ISREG(stat->st_mode) && (stat->st_mode & S_IXGRP == S_IXGRP) && (stat->st_mode & S_IXOTH == S_IXOTH)){
    n_files++;
    ino_files += stat->st_size;
  }
  printf("%s\t%lu\n", path, stat->st_size);

  return 0;
}

int main(int argc, char *argv[]) {

  printf("Los i-nodos son:\n");

  if (nftw(argc >= 2 ? argv[1] : ".", Buscar, 10, 0) != 0) {      // Con esta función recorremos los directorios siempre teniendo como máximo 10 ficheros abiertos, esto lo hemos especificado en el camp flags
      perror("nftw");
  }

  printf("Existen %lu archivos regulares con permiso x para grupo y otros.\nEl tamaño total ocupado por dichos archivos es %lu bytes\n", n_files, ino_files);

  return 0;
}
