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
static inline uint64_t read_pmccntr(void)
{
	uint64_t val;
	asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
	return val;
}
int main() {
    

    uint8_t array[256 * 64];
    volatile uint8_t *volatile_array = array; // Declare a volatile pointer to the array
    int temp, temp2, i;
    int64_t fast_diff1, fast_diff2;
    int64_t slow_diff1, slow_diff2;

    // Initialize the array
    for (i = 0; i < 256; i++) {
        volatile_array[i * 64] = 1;
        asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
        asm volatile ("ISB");
    }

    // Timestamp and array access with serialization and data barriers
    asm volatile ("isb"); // Serialize before reading the counter
    // fast_diff1 = read_pmccntr();
    asm volatile ("MRS %0, cntvct_el0" : "=r" (fast_diff1));
    asm volatile ("isb"); // Serialize after reading the counter

    temp2 = volatile_array[7 * 64];
    asm volatile ("dsb ish"); // Data synchronization barrier after volatile access
    
    asm volatile ("isb"); // Serialize before reading the counter again
    asm volatile ("MRS %0, cntvct_el0" : "=r" (fast_diff2));
    //fast_diff2 = read_pmccntr();
    asm volatile ("isb"); // Serialize after reading the counter
    int64_t first_diff = (fast_diff2 - fast_diff1);


    // FLUSH the array from the CPU cache

    for (i = 0; i < 64; i++) {
        asm volatile ("dc civac, %0" : : "r" (&volatile_array[i * 64]) : "memory");
        asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
        asm volatile ("ISB");
    }

    //Flush the element

    // asm volatile ("dc civac, %0" : : "r" (&volatile_array[7 * 256]) : "memory");
    // asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
    // asm volatile ("ISB");

    // Similar pattern for the second measurement
    asm volatile ("isb");
    asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    // slow_diff1 = read_pmccntr();
    asm volatile ("isb");

    temp = volatile_array[7 * 256];
    asm volatile ("dsb ish");

    asm volatile ("isb");
    asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    // slow_diff2 = read_pmccntr();
    asm volatile ("isb");
    int64_t second_diff = (slow_diff2 - slow_diff1);

    printf("fast diff %" PRId64 "\n",first_diff);
    printf("slow diff %" PRId64 "\n",second_diff);

   return 0;
}
