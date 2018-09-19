/*
Ejercicio 4. Define una macro en lenguaje C que tenga la misma funcionalidad que la macro
S_ISREG(mode) usando para ello los flags definidos en <sys/stat.h> para el campo st_mode de
la struct stat, y comprueba que funciona en un programa simple. Consulta en un libro de C o
en internet cómo se especifica una macro con argumento en C.
#define S_ISREG2(mode) ...
*/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>                                               
#include<string.h>
								// Lo que hacemos en la macro es hacer un AND a nivel de bits
#define S_ISREG2(mode) ((mode & S_IFMT)==S_IFREG)		// usando la mascara S_IFMT la cual es 170000 en octal,
								// lo que conseguimos con esto es que nos aparezca el tipo de 
								// archivo obviando los permisos del mismo ya que ahora mismo no nos interesa
								// Para comprobarlo basta con pasar de octal a binario y hacer el AND con 170000 en binario
int main(int argc, char *argv[]){
    int i;
    struct stat atributos;
    char tipoArchivo[30];

    if(argc<2) {
        printf("\nSintaxis de ejecucion: tarea2 [<nombre_archivo>]+\n\n");
        exit(-1);
    }
    for(i=1;i<argc;i++) {
        printf("%s: ", argv[i]);
	    
        if(lstat(argv[i],&atributos) < 0) {
            printf("\nError al intentar acceder a los atributos de %s",argv[i]);
            perror("\nError en lstat");
        }
        else {
            if(S_ISREG2(atributos.st_mode)) strcpy(tipoArchivo,"Regular");
            else if(S_ISDIR(atributos.st_mode)) strcpy(tipoArchivo,"Directorio");
            else if(S_ISCHR(atributos.st_mode)) strcpy(tipoArchivo,"Especial de caracteres");
            else if(S_ISBLK(atributos.st_mode)) strcpy(tipoArchivo,"Especial de bloques");
            else if(S_ISFIFO(atributos.st_mode)) strcpy(tipoArchivo,"Tuber�a con nombre (FIFO)");
            else if(S_ISLNK(atributos.st_mode)) strcpy(tipoArchivo,"Enlace relativo (soft)");
            else if(S_ISSOCK(atributos.st_mode)) strcpy(tipoArchivo,"Socket");
            else strcpy(tipoArchivo,"Tipo de archivo desconocido");
            printf("%s\n",tipoArchivo);
        }
    }

    return 0;
}
