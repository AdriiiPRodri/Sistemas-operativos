#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<stdbool.h>
#include<math.h>

bool EsPrimo(int n){
  int raiz = sqrt(n);
  for(int i = 2; i <= raiz; i++){
    if(n % i == 0)
      return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  for(int i = atoi(argv[1]); i <= atoi(argv[2]); i++)
    if(EsPrimo(i))
      printf("%d\n", i);

  return(0);
}
