#include<sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int nprocs = 20;
  pid_t childpid;

  for (int i=1; i < nprocs; i++) {
    if ((childpid= fork()) == -1) {
      fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
      exit(-1);
    }

    if (!childpid)        // En este caso cada hijo acaba el proceso llegando al exit(0) porque entran en el if y el proceso padre inicial es el que sigue iterando en el bucle. Hay un padre que tiene nprocs - 1 hijos que llegan al exit(0)
      break;

    /*if (childpid)      // En este otro caso, es el padre el que acaba por lo que cada hijo a su vez se convierte en padre en la siguiente iteración y vuelve a crear otro hijo, entonces el padre muere y así hasta acabar el bucle, la cuestión es que en este bucle tienen una iteración como ciclo de vida.
      break;
    */
  }

exit(0);
}
