#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char * argv[])
{
  int esperar;    // Con esta variable esperamos para poder ver la situación de interbloqueo
	struct flock cerrojo;    // Creamos el cerrojo que vamos a colocar
	int fd;    // Creamos el decriptor de archivo
	while (--argc > 0 ) {    //Abrimos nuestro argumentos dentro de este bucle
		if ((fd=open(*++argv, O_RDWR)) == -1 ){    // Abrimos nuestro archivo con lectura y escritura
			perror("open fallo");
			continue;
		}
	cerrojo.l_type=F_WRLCK;    // Elegimos el tipo de bloqueo en este caso de escritura
	cerrojo.l_whence=SEEK_SET;    // Inicio del archivo
	cerrojo.l_start=0;     // Vamos a bloquear todo el archivo
	cerrojo.l_len=0;       // Bloquear todo el archivo

	while (fcntl (fd, F_SETLK, &cerrojo) == -1) {
		/// Si el cerrojo falla, vemos que proceso lo esta bloqueando
		while (fcntl (fd, F_SETLK, &cerrojo) == -1 && cerrojo.l_type != F_UNLCK) {//Damos vueltas mientras no este desbloqueado
			printf ("%s bloqueado por %d desde %d hasta %d para %c\n", *argv, cerrojo.l_pid, cerrojo.l_start, cerrojo.l_len, cerrojo.l_type==F_WRLCK ? 'w':'r');
		if (!cerrojo.l_len)   // Si la longitud de bloqueo es 0 salimos de esta bucle while
      break;
		cerrojo.l_start += cerrojo.l_len;   // Avanzamos el inicio del bloqueo
		cerrojo.l_len = 0;    // Colocamos la longitud del boqueo a 0
		} // Proseguimos mientras existan cerrojos de otros procesos
	} // Seguimos mientras el bloqueo no tenga exito

	// Ahora el bloqueo tiene éxito y podemos procesar el archivo
	printf ("Procesando el archivo, abre otro proceso para comprobar interbloqueo.\n");
  scanf("%d\n", esperar);     // Esperamos antes de desbloquear para ver la situación de interbloqueo
	// Una vez finalizado el trabajo, desbloqueamos el archivo entero
	cerrojo.l_type=F_UNLCK;     // Desbloqueo y para todo el archivo
	cerrojo.l_whence=SEEK_SET;
	cerrojo.l_start=0;
	cerrojo.l_len=0;
	if (fcntl (fd, F_SETLKW, &cerrojo) == -1)    // Desbloqueamos
    perror ("Desbloqueo");
	}
	return 0;
}
