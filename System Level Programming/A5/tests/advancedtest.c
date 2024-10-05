#include "stdlib.h"
#define ARRAY_SIZE0 5
#define ARRAY_SIZE1 20
#define ARRAY_SIZE2 40
#define ARRAY_SIZE3 100
#define ARRAY_SIZE4 500

#define SOME_CONSTANT 0x9999999
#define ANOTHER_CONSTANT 0x666

int main()
{
  // Be creative ;)
  // int *data[5];


  int *data = (int *)malloc(ARRAY_SIZE0 * sizeof(int));
  int * data1 = (int *)malloc(ARRAY_SIZE1 * sizeof(int));
  int * data2 = (int *)malloc(ARRAY_SIZE1 * sizeof(int));
  int * data3 = (int *)malloc(ARRAY_SIZE1 * sizeof(int));
  int * data4 = (int *)malloc(ARRAY_SIZE1 * sizeof(int));

  free(data1);
  free(data1);
  free(data2);
  free(data3);
  free(data4);

  size_t return_val = free_block_info(0);



  return 0;

  // Check all the requirements from our assignment description:
  // https://www.iaik.tugraz.at/teaching/materials/slp/assignments/a5/
  // Try to test as many as you can!
  // If you don't know how to test for something, you can ask for help
  // via discord or mail.

  // You can add new files in /tests/, c and c++ both work.
  return -1;
}
