/*
 contador.c
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

//  Dependiendo del tipo de señal que nos llegue el contador si ira incrementando y mostrando por pantalla

static void sig_USR_hdlr(int sigNum)
  {
    static int senales[65] = {0};
    senales[sigNum]++;
    printf("\nRecibida señal %d, %d veces\n", sigNum, senales[sigNum]);
   }

int main(int argc, char *argv[])
   {
    struct sigaction sig_USR_nact;
    if(setvbuf(stdout,NULL,_IONBF,0))
       {
    perror("\nError en setvbuf");
       }

//Inicializar la estructura sig_USR_na para especificar la nueva acción para la señal.

sig_USR_nact.sa_handler= sig_USR_hdlr;

//'sigemptyset' inicia el conjunto de se�ales dado al conjunto vacío.

sigemptyset (&sig_USR_nact.sa_mask);
sig_USR_nact.sa_flags = 0;

//Establecer mi manejador particular de se�al para SIGUSR1
printf("Mi PID %d", getpid());

// Recorremos todas las señales y le aplicamos la acción que han de hacer al recibirse con sigaction

for(int i = 0; i < 65; i++)
  sigaction (i, &sig_USR_nact, NULL);

  printf("\nNo puedo manejar la señal 9\n");
  printf("No puedo manejar la señal 19\n");
  printf("Esperando el envío de señales..\n");

for(;;)
   {
   }
}
