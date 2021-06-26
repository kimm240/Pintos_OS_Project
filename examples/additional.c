#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc, char **argv)
{
  int num1 = atoi(argv[1]);
  int num2 = atoi(argv[2]);
  int num3 = atoi(argv[3]);
  int num4 = atoi(argv[4]);
  printf("%d ", fibonacci(num1));
  printf("%d\n", max_of_four_integers(num1, num2, num3, num4));
  return EXIT_SUCCESS;
}
