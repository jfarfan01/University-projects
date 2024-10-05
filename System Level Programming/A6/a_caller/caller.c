#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
// Implement the function call in assembly in the TODO section below.
// Do not change the functionality or the signature of the function to be called.
// Do not modify this file outside the TODO section.

// Function to be called:
double calculateAverageInRange(const double *number_array, uint32_t array_len, double lower, double upper, double *sum);

int main(int argc, char **argv)
{
    setbuf(stdout, NULL); // Disable buffering

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <number1,number2,..> <lower> <upper>\n", argv[0]);
        return 1;
    }

    char *numbers_string = argv[1];

    uint32_t array_len = 0;
    for (size_t i = 0; i < strlen(numbers_string); i++)
    {
        if (numbers_string[i] == ',')
        {
            array_len++;
        }
    }
    array_len++;

    if (array_len == 0)
    {
        fprintf(stderr, "Error: length of numbers == 0\n");
        return 1;
    }

    double numbers[array_len];
    char *number_str;
    for (uint32_t i = 0; (number_str = strtok(numbers_string, ",")); i++, numbers_string = NULL)
    {
        numbers[i] = atof(number_str);
    }

    double lower = atof(argv[2]);
    double upper = atof(argv[3]);

    double avg = INFINITY;
    double sum = INFINITY;

    printf("Calling function with parameters: number_array=%p, array_len=%u, lower=%.3f, upper=%.3f, sum=%p\n",
           numbers, array_len, lower, upper, &sum);

    // Use (only!) GCC inline assembly to call `calculateAverageInRange`. Always consider the expected data types.
    // Parameters: numbers, array_len, lower, upper, &sum
    // Store the return value in nr_matches

    // ########## TODO START ##########
    // System V AMD64 ABI implementation (64-bit)
     __asm__ volatile (
    "leaq %1, %%rdi\n\t"      
    "movq %2, %%rsi\n\t"      
    "movsd %3, %%xmm0\n\t"    
    "movsd %4, %%xmm1\n\t"
    "leaq %5, %%rdx\n\t"
    "call calculateAverageInRange\n\t"
    "movsd %%xmm0, %0\n\t"    
    : "=m" (avg)          
    : "m" (numbers), "m" (array_len), "m" (lower), "m" (upper), "m" (sum) 
    : "%rdi", "%esi","rdx", "%xmm0","%xmm1","%rsi", "r8", "r9", "r10", "r11", "cc", "memory", "%rax",
        "%rbx", "%r12", "%r13", "%r14", "%r15"
    );

    // ########## TODO END ##########

    printf("Function returned: average: %.3lf\n", avg);
    printf("Sum of matches: %.3lf\n", sum);

    return 0;
}
