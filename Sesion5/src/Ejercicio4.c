// tarea4.c

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int signal_recibida = 0;

static void manejador (int sig)     // Creamos nuestro manejador el cual tan solo nos servirá como comprobante
{
          signal_recibida = 1;
}

int main (int argc, char *argv[])
{
    sigset_t conjunto_mascaras;   // Declaramos nuestra mascara a manipular
    sigset_t conj_mascaras_original;  // Aquí guardaremos la máscara original
    struct sigaction act;         // Creamos nuestra estructura sigaction

    //Iniciamos a 0 todos los elementos de la estructura act
    memset (&act, 0, sizeof(act));

    act.sa_handler = manejador;   // Le indicamos el handler que hemos escogido, en este caso nuestra función manejador

    if (sigaction(SIGTERM, &act, 0)) {  // Cuando recibamos una señal SIGTERM, usaremos nuestra estrucura sigaction la cual tiene como handler nuestra función manejador a la cual se llamara al ejecutar la señal especificada
        perror ("sigaction");
        return 1;
    }

    //Iniciamos un nuevo conjunto de mascaras, con sigemptyset lo unicializamos sin ninguna señal, 0
    sigemptyset (&conjunto_mascaras);
    //A�adimos SIGTERM al conjunto de mascaras
    sigaddset (&conjunto_mascaras, SIGTERM);

    //Bloqueamos SIGTERM y guardamos los flags anteriores en &conj_mascaras_original y los nuevos con el añadido en &conj_mascaras
    if (sigprocmask(SIG_BLOCK, &conjunto_mascaras, &conj_mascaras_original) < 0) {
       perror ("primer sigprocmask");
       return 1;
    }

    sleep (10);

    //Restauramos la señal SIGTERM y la desbloquemas, esto lo hacemos porque restauramos la mascara inicial, también podriamos haber usado sigdelset
    if (sigprocmask(SIG_SETMASK, &conj_mascaras_original, NULL) < 0) {
       perror ("segundo sigprocmask");
       return 1;
       }

    sleep (1);

   if (signal_recibida)     // Comprobamos si nos hemos introducido en nuestro manejador, en caso afirmativo tenemos que signal_ricibida es 1 y por tanto hemos tenido exito
       printf ("\nSenal recibida\n");
   return 0;
}
