/*Ejercicio 4. Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se
identificará en la salida estándar, mostrando un mensaje del tipo Soy el hijo PID. El proceso
padre simplemente tendrá que esperar la finalización de todos sus hijos y cada vez que detecte
la finalización de uno de sus hijos escribirá en la salida estándar un mensaje del tipo:
Acaba de finalizar mi hijo con <PID>
Sólo me quedan <NUM_HIJOS> hijos vivos
*/

#include<sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){           // Cuidado con el buffer del printf al crear hijos, puede crear confusión y que los hjos impriman aunque no deban hacerlo
  int nprocs = 5;
  int status = 0;
  pid_t childpid = 1;
  pid_t pid_guardado[5];
  int contador = 5;

  for (int i = 0; i < 5; i++) {      // Creamos los cinco procesos hijos
    if(childpid)
      if ((childpid= fork()) == -1) {
        fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
        exit(-1);
      }
  }

  if(!childpid)                   // Entran los hijos
    printf("Soy el hijo %d\n", getpid());

  while(wait(&status) != -1){
    printf("Acaba de finalizar mi hijo con %d\n", status>>8);     // Vemos el PID del hijo moviendo los bits a la derecha
    contador--;
    printf("Sólo me quedan %d hijos vivos.\n", contador);
  }

exit(getpid());           // Devolvemos el PID de los hijos
}
