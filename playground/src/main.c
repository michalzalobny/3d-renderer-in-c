#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


int main (void) {
  printf("--------------------------------------------------\n");
  printf("Size of int: %lu\n", sizeof(int)); // %lu = unsigned long int
  printf("Size of int*: %lu\n", sizeof(int*));

  char my_arr[5] = {'c','d','v','g','k'};
  char* arr_pointer = my_arr;

  printf("Arr pointer: %p\n", arr_pointer);
  printf("Value of arr pointer: %c\n", *arr_pointer);

  char* arr_pointer_incremented = (char*)arr_pointer + 2;
  printf("Arr pointer incremented: %p\n", arr_pointer_incremented);
  printf("Value of arr pointer incremented: %c\n", *arr_pointer_incremented);

  printf("--------------------------------------------------\n");
  return 0;
}