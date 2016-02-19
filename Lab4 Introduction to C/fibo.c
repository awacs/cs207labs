#include <stdint.h>
#include <stdio.h>

  int fib(int n) {
  if (n == 1){
	return 0;
  }
  if (n == 2){
    return 1;
  }
  else{
	return fib(n-1)+fib(n-2);
  }
  
}

int main(int argc, char **argv)
{
int magic_number = 0;

  if( argc>1 ) {
    magic_number = atoi(argv[1]);
  } else {
    printf("Please enter a magic number.\n");
    return -1;
  }
int c;
c=fib(magic_number);
printf("answer=%ld\n", c);
}