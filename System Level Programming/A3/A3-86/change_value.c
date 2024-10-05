#include <stdio.h>
#include <stdlib.h>




int main()
{
  const int i = 10;

  int *ptr = (int*)&i;
  *ptr = 11; 
  printf("%d \n",i);
}