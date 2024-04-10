#include <stdio.h>
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

#define stride 256
unsigned int location;
#define DELTA 64
int TOP_N = 10;
int CACHE_HIT_THRESHOLD = 94;
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160];
uint8_t unused2[64];
uint8_t array2[256 * stride + DELTA];
char *secret = "LBWNB";
volatile uint8_t temp = 0; /* Used so compiler won’t optimize out victim_function() */

void victim_function(size_t x)
{
    if (x < array1_size)
    {
        temp &= array2[array1[x] * stride + DELTA];
    }
}

static inline uint64_t read_pmccntr(void)
{
    uint64_t val;
	asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
	return val;
}


int main() {

    int i;
    register uint64_t time1, time2, time3, time4, time_diff;
    int temp;
    for(i = 0;i < array1_size * 10; i++){
        array1[i] = i;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~hit time measure~~~~~~~~~~~~~~~~~
    asm volatile("isb"); // Serialize before reading the counter
    time1 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    temp = array1[array1_size * 10];
    asm volatile("isb"); // Serialize before reading the counter
    time2 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    time_diff = time2 - time1;
    printf("hit time is %ld \n",time_diff);

    //flush array1 from cache
    for(i = 0;i < array1_size * 10; i++){
        asm volatile("dc civac, %0" : : "r"(&array1[i]) : "memory");
        asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
        asm volatile("isb"); // Insert isb for serialization after cache flush
    }

    for(i = 0;i < 256 * stride + DELTA; i++){
        array2[i] = i;
    }

    //flush array2 from cache
    for(i = 0;i < 256 * stride + DELTA; i++){
        asm volatile("dc civac, %0" : : "r"(&array2[i]) : "memory");
        asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
        asm volatile("isb"); // Insert isb for serialization after cache flush
    }
    //~~~~~~~~~~~~~~~~~~~~~~~miss time measure~~~~~~~~~~~~~~~~~
    asm volatile("isb"); // Serialize before reading the counter
    time1 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    temp = array2[1];
    asm volatile("isb"); // Serialize before reading the counter
    time2 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    time_diff = time2 - time1;
    printf("miss time is %ld \n",time_diff);



    int a = 1;
    int b = 2;
    int c = 3;
    asm volatile("dc civac, %0" : : "r"(&a) : "memory");
    asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
    asm volatile("isb"); // Insert isb for serialization after cache flush
    asm volatile("dc civac, %0" : : "r"(&b) : "memory");
    asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
    asm volatile("isb"); // Insert isb for serialization after cache flush
    asm volatile("dc civac, %0" : : "r"(&c) : "memory");
    asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
    asm volatile("isb"); // Insert isb for serialization after cache flush

    if(a + b == c){
        asm volatile("isb"); // Serialize before reading the counter
        time1 = read_pmccntr();
        asm volatile("isb"); // Serialize after reading the counter
        temp = array1[array1_size * 10];
        asm volatile("isb"); // Serialize before reading the counter
        time2 = read_pmccntr();
        asm volatile("isb"); // Serialize after reading the counter
    }
    temp = array1[array1_size * 10];

    asm volatile("isb"); // Serialize before reading the counter
    time3 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    temp = array1[array1_size];
    asm volatile("isb"); // Serialize before reading the counter
    time4 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter


    printf("Time 2 - Time 1 is %ld \n", time2 - time1);
    printf("Time 4 - Time 3 is %ld \n", time4 - time3);


}