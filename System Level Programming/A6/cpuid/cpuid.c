#include <stdio.h>
#include <stdint.h>

int main()
{
    /* TODO: Find the processor's manufacturer ID string,
     *       the processor brand string, and the
     *       availability bits of the given features from the wiki
     *       using the cpuid instruction and inline assembly.
     *       Do not use builtin functions.
     *       Print the values to stdout in the given formats.
     *       Feel free to modify this file wherever you want.
     */
    char vendor[13];
    size_t first, second, third;
    // char str[20];


    asm volatile (
    "cpuid\n\t"
    : "=b" (first), "=d" (second), "=c" (third) // Outputs: store the four bytes of the vendor string
    : "a" (0x00000000)
    : "rdi", "rsi", "r8", "r9", "r10", "r11", "cc", "memory"
);

    vendor[0] = first;
    // char secondByte = (char)((first >> 8) & 0xFF);
    vendor[1] = (char)((first >> 8) & 0xFF);
    vendor[2] = (char)((first >> 16) & 0xFF);
    vendor[3] = (char)((first >> 24) & 0xFF);

    vendor[4] = second;
    vendor[5] = (char)((second >> 8) & 0xFF);
    vendor[6] = (char)((second >> 16) & 0xFF);
    vendor[7] = (char)((second >> 24) & 0xFF);

    vendor[8] = third;
    vendor[9] = (char)((third >> 8) & 0xFF);
    vendor[10] = (char)((third >> 16) & 0xFF);
    vendor[11] = (char)((third >> 24) & 0xFF);
    vendor [12] = '\0';

    size_t brand[12];
    char brand_str[49];

    asm volatile(
        "cpuid\n\t"
        : "=a" (brand[0]), "=b" (brand[1]), "=c" (brand[2]), "=d" (brand[3])
        : "a" (0x80000002)
        : "rdi", "rsi", "r8", "r9", "r10", "r11", "cc", "memory"
    );
    
    brand_str[0] = (char) brand[0];
    brand_str[1] = (char)((brand[0] >> 8) & 0xFF);
    brand_str[2] = (char)((brand[0] >> 16) & 0xFF);
    brand_str[3] = (char)((brand[0] >> 24) & 0xFF);
    
    brand_str[4] = (char) brand[1];
    brand_str[5] = (char)((brand[1] >> 8) & 0xFF);
    brand_str[6] = (char)((brand[1] >> 16) & 0xFF);
    brand_str[7] = (char)((brand[1] >> 24) & 0xFF);
    
    brand_str[8] = (char) brand[2];
    brand_str[9] = (char)((brand[2] >> 8) & 0xFF);
    brand_str[10] = (char)((brand[2] >> 16) & 0xFF);
    brand_str[11] = (char)((brand[2] >> 24) & 0xFF);

    brand_str[12] = (char) brand[3];
    brand_str[13] = (char)((brand[3] >> 8) & 0xFF);
    brand_str[14] = (char)((brand[3] >> 16) & 0xFF);
    brand_str[15] = (char)((brand[3] >> 24) & 0xFF);

    asm volatile(
        "cpuid\n\t"
        : "=a" (brand[0]), "=b" (brand[1]), "=c" (brand[2]), "=d" (brand[3])
        : "a" (0x80000003)
        : "rdi", "rsi", "r8", "r9", "r10", "r11", "cc", "memory"
    );

    brand_str[16] = (char) brand[0];
    brand_str[17] = (char)((brand[0] >> 8) & 0xFF);
    brand_str[18] = (char)((brand[0] >> 16) & 0xFF);
    brand_str[19] = (char)((brand[0] >> 24) & 0xFF);
    
    brand_str[20] = (char) brand[1];
    brand_str[21] = (char)((brand[1] >> 8) & 0xFF);
    brand_str[22] = (char)((brand[1] >> 16) & 0xFF);
    brand_str[23] = (char)((brand[1] >> 24) & 0xFF);
    
    brand_str[24] = (char) brand[2];
    brand_str[25] = (char)((brand[2] >> 8) & 0xFF);
    brand_str[26] = (char)((brand[2] >> 16) & 0xFF);
    brand_str[27] = (char)((brand[2] >> 24) & 0xFF);

    brand_str[28] = (char) brand[3];
    brand_str[29] = (char)((brand[3] >> 8) & 0xFF);
    brand_str[30] = (char)((brand[3] >> 16) & 0xFF);
    brand_str[31] = (char)((brand[3] >> 24) & 0xFF);

    asm volatile(
        "cpuid\n\t"
        : "=a" (brand[0]), "=b" (brand[1]), "=c" (brand[2]), "=d" (brand[3])
        : "a" (0x80000004)
        : "rdi", "rsi", "r8", "r9", "r10", "r11", "cc", "memory"
    );

    brand_str[32] = (char) brand[0];
    brand_str[33] = (char)((brand[0] >> 8) & 0xFF);
    brand_str[34] = (char)((brand[0] >> 16) & 0xFF);
    brand_str[35] = (char)((brand[0] >> 24) & 0xFF);
    
    brand_str[36] = (char) brand[1];
    brand_str[37] = (char)((brand[1] >> 8) & 0xFF);
    brand_str[38] = (char)((brand[1] >> 16) & 0xFF);
    brand_str[39] = (char)((brand[1] >> 24) & 0xFF);
    
    brand_str[40] = (char) brand[2];
    brand_str[41] = (char)((brand[2] >> 8) & 0xFF);
    brand_str[42] = (char)((brand[2] >> 16) & 0xFF);
    brand_str[43] = (char)((brand[2] >> 24) & 0xFF);

    brand_str[44] = (char) brand[3];
    brand_str[45] = (char)((brand[3] >> 8) & 0xFF);
    brand_str[46] = (char)((brand[3] >> 16) & 0xFF);
    brand_str[47] = (char)((brand[3] >> 24) & 0xFF);

    brand_str[48] = '\0';

    size_t features;
    size_t features2;

    asm volatile(
        "cpuid\n\t"
        : "=d" (features), "=c"(features2)
        : "a" (1)
        : "rdi", "rsi", "r8", "r9", "r10", "r11", "cc", "memory"
    );

    size_t DE_bit_mask = 1 << 2;  
    size_t PAE_bit_mask = 1 << 6;
    size_t HTT_bit_mask = 1 << 28;
    size_t TM_bit_mask = 1 << 29;

    int DE = (features & DE_bit_mask) > 0;
    int PAE = (features & PAE_bit_mask) > 0;
    int HTT = (features & HTT_bit_mask) > 0;
    int TM = (features & TM_bit_mask) > 0;

    size_t EST_mask = 1 << 7;
    int EST = (features2 & EST_mask) > 0;

    size_t features3;
    asm volatile(
        "cpuid\n\t"
        : "=b" (features3)
        : "a" (7), "c"(0)
        : "rdi", "rsi", "r8", "r9", "r10", "r11", "cc", "memory"
    );

    size_t AVX2_mask = 1 << 5;
    size_t SMEP_mask = 1 << 7;
    size_t RDSEED_mask = 1 << 18;

    int AVX2 = (features3 & AVX2_mask) > 0;
    int SMEP = (features3 & SMEP_mask) > 0;
    int RDSEED = (features3 * RDSEED_mask) > 0;


    printf("Manufacturer id: %s\n", vendor);

    printf("Processor brand string: %s\n", brand_str);

    // printf("de: %u, pae: %u, htt: %u, tm: %u, est: %u, avx2: %u, smep: %u, rdseed: %u\n"
    // ,DE, PAE, HTT, TM, EST, AVX2, SMEP, RDSEED);


    printf("de: %u, pae: %u, htt: %u, tm: %u, est: %u, avx2: %u, smep: %u, rdseed: %u\n",
               DE, PAE, HTT, TM, EST, AVX2, SMEP, RDSEED);
}