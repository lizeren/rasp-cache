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
int main() {
    // arm_v8_timing_init();
    // uint64_t result_pmccntr = arm_v8_get_timing();
    // asm volatile ("isb"); // Serialize after reading the counter
    // printf("pmccntr: %d",result_pmccntr);

    // uint64_t temp_disp1,temp_disp2,diff,freq;
    // asm volatile("MRS %0, cntfrq_el0 " : "=r" (freq));
    // asm volatile("MRS %0, cntvct_el0 " : "=r" (temp_disp1));
    // asm volatile ("isb"); // Serialize after reading the counter
    // printf("Freq of cntvct is: %lld\n",freq);
    // printf("cntvct_el0 is: %lld\n",temp_disp1);

    uint8_t array[256 * 8192];
    volatile uint8_t *volatile_array = array; // Declare a volatile pointer to the array
    int temp, temp2, i;
    int64_t fast_diff1, fast_diff2;
    int64_t slow_diff1, slow_diff2;

    // Initialize the array
    for (i = 0; i < 256; i++) {
        volatile_array[i * 8192] = 1;
        asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
    }
    
    // Access some of the array items
    volatile_array[7 * 8192] = 200;


    // Timestamp and array access with serialization and data barriers
    asm volatile ("isb"); // Serialize before reading the counter
    asm volatile ("MRS %0, cntvct_el0" : "=r" (fast_diff1));
    asm volatile ("isb"); // Serialize after reading the counter
    temp2 = volatile_array[7 * 8192];
    asm volatile ("dsb ish"); // Data synchronization barrier after volatile access
    asm volatile ("isb"); // Serialize before reading the counter again
    asm volatile ("MRS %0, cntvct_el0" : "=r" (fast_diff2));
    asm volatile ("isb"); // Serialize after reading the counter
    int64_t first_diff = (fast_diff2 - fast_diff1);




    // FLUSH the array from the CPU cache
    for (i = 0; i < 256; i++) {
        asm volatile ("dc civac, %0" : : "r" (&volatile_array[i * 8192]) : "memory");
        //asm volatile ("dsb ish"); // Data synchronization barrier ensure invalidation has completed
        asm volatile ("isb"); // Insert isb for serialization after cache flush
    }
 

    // Similar pattern for the second measurement
    asm volatile ("isb");
    asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    asm volatile ("isb");
    temp = volatile_array[1 * 8192];
    asm volatile ("dsb ish");
    asm volatile ("isb");
    asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    asm volatile ("isb");
    int64_t second_diff = (slow_diff2 - slow_diff1);

    printf("fast diff %" PRId64 "\n",first_diff);
    printf("slow diff %" PRId64 "\n",second_diff);

    // asm volatile("MRS %0, cntvct_el0 " : "=r" (temp_disp2));
    // asm volatile ("isb"); // Serialize after reading the counter
    // diff = temp_disp2 - temp_disp1;
    // printf("time spent to run the whole program: %d\n",diff);
   return 0;
}
