#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


int main (void) {
  printf("--------------------------------------------------\n");
  printf("Size of int: %lu\n", sizeof(int)); // %lu = unsigned long int
  printf("Size of int*: %lu\n", sizeof(int*));

  int my_arr[5] = {9,2,7,1,4};
  int* arr_pointer = my_arr;

  printf("Arr pointer: %p\n", arr_pointer);
  printf("Value of arr pointer: %d\n", *arr_pointer);

  int* arr_pointer_incremented = arr_pointer + 1;
  printf("Arr pointer incremented: %p\n", arr_pointer_incremented);
  printf("Value of arr pointer incremented: %d\n", *arr_pointer_incremented);
  
  printf("--------------------------------------------------\n");
  return 0;
}