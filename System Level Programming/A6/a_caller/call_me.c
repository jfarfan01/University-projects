#include <stdio.h>
#include <stdint.h>
#include <math.h>
// There is nothing to do in this file.
// Changes in this file will not be considered in the test system.

// This function has to be called.
/**
 * @brief calculates the average of the numbers in the provided array within lower and upper
 *
 * @param number_array the array to search
 * @param array_len the length of the array to search
 * @param lower the lower bound
 * @param upper the upper bound
 * @param sum reference, where the sum of matches will be stored
 * @return double, average of the matches
 */
double calculateAverageInRange(const double *number_array, uint32_t array_len, double lower, double upper, double *sum)
{
    printf("Function called with parameters: number_array=%p, array_len=%u, lower=%.3f, upper=%.3f, sum=%p\n",
           number_array, array_len, lower, upper, sum);
    *sum = 0;
    printf("survived dereference\n");
    double avg = 0;
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < array_len; ++i)
    {
        double n = number_array[i];
        printf("survived another derefernce\n");
        if (lower <= n && n <= upper)
        {
            *sum += n;
            avg += n;
            cnt++;
        }
        printf("looped\n");
    }
    return cnt > 0 ? (*sum / cnt) : NAN;
}