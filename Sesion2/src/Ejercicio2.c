/*
Ejercicio2.c
Realiza un programa en C utilizando las llamadas al sistema necesarias que acepte
como entrada:
• Un argumento que representa el 'pathname' de un directorio.
• Otro argumento que es un número octal de 4 dígitos (similar al que se puede utilizar
para cambiar los permisos en la llamada al sistema chmod). Para convertir este argumento
tipo cadena a un tipo numérico puedes utilizar la función strtol. Consulta el manual en
línea para conocer sus argumentos.
El programa tiene que usar el número octal indicado en el segundo argumento para cambiar los
permisos de todos los archivos que se encuentren en el directorio indicado en el primer
argumento.
El programa debe proporcionar en la salida estándar una línea para cada archivo del directorio
que esté formada por:
<nombre_de_archivo> : <permisos_antiguos> <permisos_nuevos>
Si no se pueden cambiar los permisos de un determinado archivo se debe especificar la siguiente
información en la línea de salida:
<nombre_de_archivo> : <errno> <permisos_antiguos>
*/

//La estructura struct dirent conforme a POSIX 2.1 es la siguiente:
#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include "dirent.h"  //cabecera necesaria para el tratamiento de directorios

int main(int argc, char *argv[]) {

  if(argc >2){
    char* path_name = argv[1];        // Guardamos el nombre del directorio en un array
    chdir(path_name);                 // Para que funcione en cualquier path_name que nos pasen, debemos de cambiar el directorio de trabajo con chdir
    DIR* dir = opendir(path_name);    // Abrimos el directorio
    int new_chmod = strtol(argv[2], NULL, 8);   // Transformamos los permisos introducidos en la entrada estandar como segundo argumento a octal
    struct dirent* DIR_info;          // Creamos la estructura dirent* para guardar las propiedades del directorio

    if(dir==NULL){                    // Primera comprobación de errores
      perror("\nError al abrir el directorio.\n");
      exit(-1);
    }

    DIR_info = readdir(dir);          // Con esto leemos las propiedades del directorio
    if(DIR_info == NULL){             // Comprobamos que todo va bien
      perror("\nError al leer el directorio.\n");
      exit(-1);
    }

    do{
      struct stat atributos;              // Declaramos la estructura stat para ver los permisos del archivo
      char* actual = DIR_info->d_name;    // Guardamos el nombre del directorio gracias a la estructura dirent*

      if(stat(actual, &atributos) < 0){   // Aquí leemos los atributos de nuestro directorio actual
        printf("\nFallo al leer los atributos de %s: ", actual);
        exit(-1);
      }
      if(S_ISREG(atributos.st_mode)){     // Comprobamos que es un archivo regular y en caso afirmativo seguimos
        int old_chmod = atributos.st_mode & 0777;    // Guardamos los permisos antiguos para mostrarlos por pantalla
        if(chmod(actual, new_chmod) < 0){            // Cambiamos los permisos
          printf("\nNombre: %s\t", actual);          // Caso de error
          perror("Error al cambiar los permisos.");
          printf("\t%o\n", old_chmod);
        }else
          printf("\nNombre: %s\t%o\t%o\n",actual, old_chmod, new_chmod);    // Mostramos los  permisos nuevos y antiguos
      }
      DIR_info = readdir(dir);        // Volvemos a leer para pasar al siguiente archivo
    }while(DIR_info != NULL);         // Fin del bucle cuando no hay más directorios o archivos
  }
  else
    printf("%s", "\n    Debes especificar la ruta del directorio y los permisos que deseas colocarle.\n\n");    // Falta de argumentos

return 0;
}
