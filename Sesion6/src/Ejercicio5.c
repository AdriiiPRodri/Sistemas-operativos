#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main (int argc, char *argv[])
{
  struct stat sb;
  off_t len;
  char *p, *q;
  int fd ,fd1;

  if (argc < 3) {
    printf("Faltan argumentos.\n");
    return 1;
  }

  fd = open (argv[1], O_RDONLY, S_IRWXU);       // Abrimos el archivo que queremos copiar
  if (fd == -1) {
    printf("Error al abrir archivo\n");
    return 1;
  }

  if (fstat (fd, &sb) == -1) {                  // Le aplicamos la estructura stat para acceder a sus datos
    printf("Error al hacer stat\n");
    return 1;
  }

  int tama = sb.st_size;          // Guardamos el tamaño del archivo

  if (!S_ISREG (sb.st_mode)) {    // Comprobamos que el archivo 1 es un archivo regular
    printf ("%s : no es un archivo regular\n", argv[1]);
    return 1;
  }

  p = (char *)mmap (0, tama, PROT_READ, MAP_SHARED, fd, 0);       // Proyectamos en memoria el archivo 1 completo
  if (p == MAP_FAILED) {
    printf("Fallo el mapeo\n");
    return 1;
  }

  if (close (fd) == -1) {     // Cerramos el archivo 1 ya que esta en memoria
    printf("Error al cerrar el archivo\n");
    return 1;
  }

  fd1 = open (argv[2], O_WRONLY|O_CREAT, S_IRWXU);    // Abrimos - creamos el fichero 2 el cual se puede escribir solo
  ftruncate(fd1, tama);       // truncamos al tamaño del archivo1 el archivo2

  q = (char *)mmap (0, tama, PROT_WRITE, MAP_SHARED, fd1, 0); // Proyectamos en memoria el archivo 2 completo
  if (q == MAP_FAILED) {
    printf("Fallo el mapeo del archivo 2\n");
    return 1;
  }

  memcpy(q, p, tama);       // Copiamos la sección de memoria p en q, copiamos el tamaño especificado en tama, es decir el archivo 1 completo

  if (munmap (p, tama) == -1) { // Cerrar la proyeccion 1.
    printf("Error al cerrar la proyeccion \n");
    return 1;
  }
  if (munmap (q, tama) == -1) { // Cerrar la proyeccion 2.
    printf("Error al cerrar la proyeccion 2\n");
    return 1;
  }

  return 0;
}
