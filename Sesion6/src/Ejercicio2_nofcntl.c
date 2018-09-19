#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char * argv[])
{
  if(argc < 4){
    printf("Faltan argumentos\n");
    return 0;
  }

  if(strcmp(argv[2], "|")){     // Si el argumento no es | salimos
    printf("Error en el segundo argumento, ha de ser '|'\n");
    return 0;
  }

  close(1);    // Cerramos la salida
  if (fcntl(fd, F_DUPFD, 1) == -1 )     // Redirigimos la salida a nuestro archivo
    printf ("Fallo en fcntl");
  execlp(argv[1], argv[1], NULL);     // Ejecutamos el comando y la salida esta redirigida a nuestro archivo abierto

  close(0);     // Cerramos el decriptor de entrada
  if (fcntl(fd, F_DUPFD, 0) == -1 )    // Redirigimos la entrada estándar a nuestro archivo anteriormente abierto
    printf ("Fallo en fcntl");
  execlp(argv[3], argv[3], NULL);    // Ejecutamos el comando que nos especifican sobre el archivo abierto que es nuestra entrada

}
