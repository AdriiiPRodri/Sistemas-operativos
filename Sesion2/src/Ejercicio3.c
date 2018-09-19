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

struct INFO_files{
   unsigned long int n_files;
   unsigned long int ino_files;
};

void hierarchy(char* const path, struct INFO_files* info){
  DIR* dir = opendir(path);
  chdir(path);
  char* actual;
  struct dirent* DIR_info;
  struct stat atributos;
  char ruta_dir[1024];

  if(dir==NULL){
    perror("\nError al abrir el directorio.");
    exit(-1);
  }

  while((DIR_info = readdir(dir)) != NULL){
    actual = DIR_info->d_name;
    struct stat atributos;
    stat(actual, &atributos);
    sprintf(ruta_dir, "%s/%s", path, actual);

    if(S_ISREG(atributos.st_mode) && (atributos.st_mode & S_IXGRP == S_IXGRP) && (atributos.st_mode & S_IXOTH == S_IXOTH)){
      printf("%s\t%lu\n", ruta_dir, DIR_info->d_ino);
      info->n_files++;
      info->ino_files += atributos.st_size;
    }
    else if(S_ISDIR(atributos.st_mode) && (strcmp(actual, ".") != 0) && (strcmp(actual, "..") != 0))
      hierarchy(ruta_dir, info);
  }
  chdir("..");
  closedir(dir);
}

int main(int argc, char *argv[]) {

  struct INFO_files info = {0,0};

  if(argc == 2){
    printf("Los i-nodos son:\n");
    hierarchy(argv[1], &info);
    printf("Existen %lu archivos regulares con permiso x para grupo y otros.\nEl tamaño total ocupado por dichos archivos es %lu bytes\n", info.n_files, info.ino_files);
  }
  else if(argc == 1){
    printf("Los i-nodos son:\n");
    hierarchy(".", &info);
    printf("Existen %lu archivos regulares con permiso x para grupo y otros.\nEl tamaño total ocupado por dichos archivos es %lu bytes\n", info.n_files, info.ino_files);
  }
  else
    printf("\nNo puedes introducir tantos argumentos.");

return 0;
}
