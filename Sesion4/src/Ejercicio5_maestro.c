#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(int argc, char *argv[]) {

  if(argc > 2){
    int
      fd[2],
      fd1[1],
      nbytes,
      esclavos = 2,
      ext_inf = atoi(argv[1]),      // Para que lo interprete como int
      ext_sup = atoi(argv[2]),
      intermedio = (ext_sup - ext_inf) / 2 + ext_inf,
      n_hijo = -1;
    pid_t PID = getpid();           // Variable donde almacenamoslos PID correspondientes
    char
      ext_inf_char[10],
      ext_sup_char[10],
      ext_dif_char[10],
      ext_dif1_char[10],
      buffer[2];

    pipe(fd); // Llamada al sistema para crear un pipe
    pipe(fd1);

    sprintf(ext_inf_char, "%d", ext_inf);             // Convertimos los argumentos a char para pasarselos a los hijos a traves de execlp
    sprintf(ext_sup_char, "%d", ext_sup);
    sprintf(ext_dif_char, "%d",  intermedio);
    sprintf(ext_dif1_char, "%d", intermedio + 1);

    for(int i = 0; i < esclavos; i++){                // Creamos los dos hijos
      if(PID > 0){
        n_hijo = i;                                   // Cada hijo guarda su variable correspondiente
        if ( (PID= fork())<0) {
          perror("\nError en fork");
          exit(-1);
        }
      }
      else
        break;
    }

    if (n_hijo == 0) {                      // Hijo 1
      close(fd[0]);
      dup2(fd[1],STDOUT_FILENO);
      execlp("./Ejercicio5_esclavo","./Ejercicio5_esclavo",ext_inf_char, ext_dif_char,NULL);    // Llamamos a nuestro programa esclavo con los limites de calculo que deseemos
      return 1;
    }
    else if(n_hijo == 1 && PID == 0){       // Hijo 2
      close(fd1[0]);
      dup2(fd1[1],STDOUT_FILENO);
      execlp("./Ejercicio5_esclavo","./Ejercicio5_esclavo",ext_dif1_char, ext_sup_char,NULL);
      return 1;
    }

    /*******************************/
    //           Padre             //
    /*******************************/

    close(fd[1]);           // Cerramos ambos decriptores de escritura
    close(fd1[1]);

    /*****************************Lectura - Escritura del primer cauce***********************************/

    do{
      memset(buffer,0,strlen(buffer));
      nbytes = read(fd[0],buffer,sizeof(buffer));         // Comenzamos a leer desde nuestro cuace y lo metemos en el buffer
      if(write(STDOUT_FILENO,buffer,strlen(buffer))==-1){
    		perror("Error en la escritura del hijo1.\n");
    		exit(EXIT_FAILURE);
    	}
    }while(nbytes!=0);                                     // Escribimos en este bucle hasta que acabemos el cauce del hijo 1

    /****************************************************************************************************/

    /*****************************Lectura - Escritura del segundo cauce**********************************/

    do{
      memset(buffer,0,strlen(buffer));
      nbytes = read(fd1[0],buffer,sizeof(buffer));         // Comenzamos a leer desde nuestro cuace y lo metemos en el buffer
      if(write(STDOUT_FILENO,buffer,strlen(buffer))==-1){
    		perror("Error en la escritura del hijo1.\n");
    		exit(EXIT_FAILURE);
    	}
    }while(nbytes!=0);                                     // Escribimos en este bucle hasta que acabemos el cauce del hijo 2

    /****************************************************************************************************/

  }
  else{
    printf("Te faltan argumentos.\n");
  }
  return(0);
}
