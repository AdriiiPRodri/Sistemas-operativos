#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define tamano 1024

void bloqueodesbloqueo(int dbloqueo, int orden) {

    struct flock cerrojo;

    cerrojo.l_type = orden;         // Elegimos el tipo de bloqueo en este caso de escritura
    cerrojo.l_whence = SEEK_SET;    // Inicio del archivo
    cerrojo.l_start = 0;            // Vamos a bloquear todo el archivo
    cerrojo.l_len = 0;              // Bloquear todo el archivo

    if (fcntl(dbloqueo, F_SETLKW, &cerrojo) == -1) {
            perror ("Proxy: Problema al bloquear");
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]){

    char bufer[tamano];
    FILE* fdtmp;
    size_t bytesleidos;
    int fdbloqueo;
    int nombrefifo = (int)getpid();
    umask(0);

    if ((fdbloqueo = open("bloqueo", O_RDWR)) < 0){
            perror("Error al abrir el archivo de bloqueo\n");
            exit(EXIT_FAILURE);
    }

    //Creacion del archivo temporal
    if ((fdtmp = tmpfile()) == NULL){
            perror("Error en tmpfile: ");
            exit(EXIT_FAILURE);
    }

    while ((bytesleidos = read(STDIN_FILENO, &bufer, tamano)) > 0){             // En el archivo servidor esta la redirección de la entrada estándar al fifo
            if(fwrite(&bufer, 1, bytesleidos, fdtmp) != bytesleidos){
                    perror("Error en fwrite\n");
                    exit(EXIT_FAILURE);
            }
    }

    //Volvemos al inicio del descriptor del archivo
    rewind(fdtmp);

    //Obtener cerrojo para imprimir en pantalla
    bloqueodesbloqueo(fdbloqueo, F_WRLCK);
    while((bytesleidos = fread(&bufer, sizeof(char), strlen(bufer), fdtmp)) > 0){
        printf("%s", bufer);
    }

    //Desbloqueamos
    bloqueodesbloqueo(fdbloqueo, F_UNLCK);
    char eliminar[25];
    sprintf(eliminar, "fifo.%d", getpid());
    unlink(eliminar);

    //La eliminacion del archivo temporal es automatica

    exit(EXIT_SUCCESS);

}
