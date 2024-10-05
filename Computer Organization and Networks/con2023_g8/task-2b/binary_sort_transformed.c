// 

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// RISC-V Register set
const size_t zero = 0;
size_t a0, a1;                     // fn args or return args
size_t a2, a3, a4, a5, a6, a7;     // fn args
size_t t0, t1, t2, t3, t4, t5, t6; // temporaries
// Callee saved registers, must be stacked befor using it in a function!
size_t s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
//-----------------------------------------------------------------------------

// int binary_search(int* A, int low, int high, int x)
void binary_search()
{
 
  size_t stacked_s1 = s1;
  size_t stacked_s2 = s2;
  size_t stacked_s3 = s3;
  size_t stacked_s4 = s4;
  size_t stacked_s5 = s5;
  size_t stacked_s6 = s6;

  s1 = 0;  // m
  s2 = a0; // array base pointer to array
  s3 = s2; // pointer to current element
  s4 = a2; // low
  s5 = a3; // high
  s6 = a4; // x
	// printf("low:%d high:%d\n",s4,s5);
  if ((int)s4 > (int)s5) goto first_else;

	// else printf("no\n");

  s1 = (s4 + s5);
  s1 = s1 / 2;
	// printf("x:%d  A:%u\n",s6, *((int *)s2 + s1));
  if ((ssize_t)s6 == *((ssize_t *)s2 + s1))
    goto returnm;

  if ((ssize_t)s6 < *((ssize_t *)s2 + s1))
    goto recursion1;

  a2 = s1 + 1;
  a3 = s5;
	a4 = s6;
  binary_search();
 

recursion1:
	// printf("high -1 recursion\n");
  a2 = s4;
  a3 = s1 -1;
	a4 = s6;
  binary_search();



first_else:
  if ((ssize_t)s6 > *((ssize_t *)s2 + s4))
    goto return_lowplus1;

  a1 = s4;
  s1 = stacked_s1;
  s2 = stacked_s2;
  s3 = stacked_s3;
  s4 = stacked_s4;
  s5 = stacked_s5;
  s6 = stacked_s6;
  return;
return_lowplus1:

  a1 = s4 + 1;
  s1 = stacked_s1;
  s2 = stacked_s2;
  s3 = stacked_s3;
  s4 = stacked_s4;
  s5 = stacked_s5;
  s6 = stacked_s6;

  return;

returnm:
  a1 = s1;
  s1 = stacked_s1;
  s2 = stacked_s2;
  s3 = stacked_s3;
  s4 = stacked_s4;
  s5 = stacked_s5;
  s6 = stacked_s6;
  return;


}

void insertion_sort()
 {
	size_t stacked_s1 = s1; //i
	size_t stacked_s2 = s2; //x
	size_t stacked_s3 = s3; //idx
	size_t stacked_s4 = s4; //j 
	size_t stacked_s5 = s5; //running pointer
	size_t stacked_s6 = s6; //current array element
  size_t stacked_s7 = s7;

	s1 = 0;
	s2 = 0;
	s3 = 0;
	s4 = 1;
	s5 = a0;
	s6 = s5;
  s7 = a1;//size

	s6 = s6 + 4;//offset 4 because j starts at 1

	goto insertion_for_test;
insertion_for_body: 

	s2 = *(int*)s6;
	a2 = 0;
	a3 = s4;
	a4 = s2;
	binary_search();
	s3 = a1;
	s1 = s4;

	goto insertion_while_test;
insertion_while_body:
	t0 = s1 -1;
	t1 = *((int*)s5 + t0);
	*((int*)s5 + s1) = t1;

	s1 = s1 - 1;


insertion_while_test: if((ssize_t)s1 >(ssize_t) s3) goto insertion_while_body;


	// s2 = //A[j]
	s6 = s6 + 4;
	s4 = s4 + 1;
	*((int*)s5 + s3) = s2;			//A[idx] = x;

insertion_for_test: if((ssize_t)s4 < (ssize_t)s7) goto insertion_for_body; 
	//stack stuff
	s1 = stacked_s1;
	s2 = stacked_s2;
	s3 = stacked_s3;
	s4 = stacked_s4;
	s5 = stacked_s5;
	s6 = stacked_s6;
  s7 = stacked_s7;
  return;
 }   


void input(void) {
  // Read size
  t0 = a0; // Save a0
  a0 = fscanf(stdin, "%08x\n", (int *)&t1);
  t4 = 1;
  if (a0 == t4)
    goto input_continue;
  // Early exit
  a0 = 0;
  return;

input_continue:
  t4 = 1;
  t5 = 10;
input_loop_begin:
  if (t5 == 0)
    goto after_input_loop;
  a0 = fscanf(stdin, "%08x\n", (int *)&t2);
  if (a0 == t4)
    goto continue_read;
  // Exit, because read was not successful
  a0 = t1;
  return;
continue_read:
  *(int *)t0 = t2;
  // Pointer increment for next iteration
  t0 = t0 + 4;
  // Loop counter decrement
  t5 = t5 - 1;
  goto input_loop_begin;

after_input_loop:
  a0 = t1;
  return;
}

void output(void) {
before_output_loop:
  if (a0 == 0)
    goto after_output_loop;

  fprintf(stdout, "%08x\n", (unsigned int)*(int *)a1);

  // Pointer increment for next iteration
  a1 = a1 + 4;
  // Decrement loop counter
  a0 = a0 - 1;
  goto before_output_loop;

after_output_loop:
  return;
}

int main(void) {
  int A[10];
  int size;

  a0 = (size_t)A;
  input();
  size = a0;

  a0 = (size_t)A;
  a1 = size;
  // void insertion_sort(int* A, int size)
  insertion_sort();

  a0 = size;
  a1 = (size_t)A;
  output();

  return 0;
}