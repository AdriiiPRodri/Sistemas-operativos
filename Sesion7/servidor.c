#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#define tamano 1024
#define longnombre 50

static void mimanejador(int senal){       // Con esto evitamos que queden procesos zombies
  int estado;
  wait(&estado);
  return;
}

static void terminador(int senal){       // Con esto eliminamos los archivos
  unlink("bloqueo");
  unlink("nombrefifoe");
  unlink("nombrefifos");
  exit(0);
}

int main (int argc, char **argv) {

/****** Variables ******/
  int fifo_conocido_e, fifo_conocido_s, fifo_proxy;
  int buffer;
  char nombrefifoe[longnombre], nombrefifos[longnombre];
  pid_t proxyPID;
/***********************/

  if (signal(SIGCHLD, mimanejador) == SIG_ERR)      // Envio de la señal al manejador
    perror ("Servidor: Error configurando senal");

  if (signal(SIGINT, terminador) == SIG_ERR)      // Envio de la señal al terminador
    perror ("Servidor: Error configurando senal");

  umask(0);
/****** Creamos los cauces ******/
  if(mkfifo("nombrefifoe", S_IRWXU) < 0){
    perror ("Servidor: Error configurando senal");
    exit(EXIT_FAILURE);
  }
  if(mkfifo("nombrefifos", S_IRWXU) < 0){
    perror ("Servidor: Error configurando senal");
    exit(EXIT_FAILURE);
  }
/********************************/

/****** Abrimos los cauces ******/
  if ((fifo_conocido_e = open("nombrefifoe", O_RDWR)) < 0) {      // Abrimos el cauce FIFO conocido para lectura
    perror ("Servidor: Problema con el archivo fifo de entrada del servidor");
    exit(-1);
  }
  if ((fifo_conocido_s = open ("nombrefifos", O_RDWR)) < 0) {            // Abrimos el cauce FIFO conocido para escritura
    perror ("Servidor: Problema con el archivo fifo de salida del servidor");
    exit(1);
  }
/********************************/

/****** Crea archivo bloque ******/
if (open("bloqueo", O_CREAT, 0666) == -1 ){    // Creamos nuestro archivo bloqueo
  perror("Open fallo");
}
/*********************************/

/****** Mientras no fin de lectura ******/
  while(read(fifo_conocido_e, &buffer, sizeof(int)) > 0){
    if ((proxyPID = fork()) == -1){                                    // Por cada lectura crea un hijo
      fprintf(stderr, "No se puede crear el hijo.");
      exit(-1);
    }
    if(!proxyPID){                                                      // Entran los hijos
      int mypid = getpid();
      char nombre_fifo[25];
      int fdf;
      sprintf(nombre_fifo, "fifo.%d", mypid);
      mkfifo(nombre_fifo, 0666);       // Crea el fifo con el nombre del PID del proxy
      if(write(fifo_conocido_s, &mypid, sizeof(int))  == -1) {      // Escribimos el PID del proxyPID
        perror("\nError al escribir en el FIFO");
        exit(-1);
      }

      if ((fdf = open(nombre_fifo, O_RDONLY)) < 0) {      // Abrimos el cauce FIFO del proxy para lectura
        perror ("Servidor: Problema con el archivo fifo de entrada del servidor");
        exit(-1);
      }

      /****** Duplica el fifo anterior ******/
      dup2(fdf, STDIN_FILENO);

      execlp("./proxy", "proxy", 0, NULL);
    }
  }
}
