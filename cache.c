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
    

    uint8_t array[256 * 4096];
    volatile uint8_t *volatile_array = array; // Declare a volatile pointer to the array
    int temp, temp2, i;
    int64_t fast_diff1, fast_diff2;
    int64_t slow_diff1, slow_diff2;

    // Initialize the array
    for (i = 0; i < 256; i++) {
        volatile_array[i * 4096] = 1;
        asm volatile ("dsb ish"); // Data synchronization barrier ensure write has completed
    }

    // Timestamp and array access with serialization and data barriers
    asm volatile ("isb"); // Serialize before reading the counter
    fast_diff1 = read_pmccntr();
    //asm volatile ("MRS %0, cntvct_el0" : "=r" (fast_diff1));
    asm volatile ("isb"); // Serialize after reading the counter
    temp2 = volatile_array[7 * 4096];
    asm volatile ("dsb ish"); // Data synchronization barrier after volatile access
    asm volatile ("isb"); // Serialize before reading the counter again
    // asm volatile ("MRS %0, cntvct_el0" : "=r" (fast_diff2));
    fast_diff2 = read_pmccntr();
    asm volatile ("isb"); // Serialize after reading the counter
    int64_t first_diff = (fast_diff2 - fast_diff1);




    // FLUSH the array from the CPU cache

    for (i = 0; i < 256; i++) {
        asm volatile ("dc civac, %0" : : "r" (&volatile_array[i * 4096]) : "memory");
        //asm volatile ("dsb ish"); // Data synchronization barrier ensure invalidation has completed
        asm volatile ("isb"); // Insert isb for serialization after cache flush
    }

 

    // Similar pattern for the second measurement
    asm volatile ("isb");
    // asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    slow_diff1 = read_pmccntr();
    asm volatile ("isb");
    temp = volatile_array[7 * 4096];
    asm volatile ("dsb ish");
    asm volatile ("isb");
    //asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    slow_diff2 = read_pmccntr();
    asm volatile ("isb");
    int64_t second_diff = (slow_diff2 - slow_diff1);

    temp = volatile_array[10 * 4096];
    printf("fast diff %" PRId64 "\n",first_diff);
    printf("slow diff %" PRId64 "\n",second_diff);

    // Similar pattern for the second measurement
    asm volatile ("isb");
    //asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    slow_diff1 = read_pmccntr();
    asm volatile ("isb");
    temp = volatile_array[80 * 4096];
    asm volatile ("dsb ish");
    asm volatile ("isb");
    //asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    slow_diff2 = read_pmccntr();
    asm volatile ("isb");
    printf("what about access 80: %" PRId64 "\n",slow_diff2 - slow_diff1);

    // Similar pattern for the second measurement
    asm volatile ("isb");
    asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    slow_diff1 = read_pmccntr();
    asm volatile ("isb");
    temp = volatile_array[119 * 4096];
    asm volatile ("dsb ish");
    asm volatile ("isb");
    // asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    slow_diff2 = read_pmccntr();
    asm volatile ("isb");
    printf("what about access 119: %" PRId64 "\n",slow_diff2 - slow_diff1);

    asm volatile ("isb");
    // asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    slow_diff1 = read_pmccntr();
    asm volatile ("isb");
    temp = volatile_array[10 * 4096];
    asm volatile ("dsb ish");
    asm volatile ("isb");
    // asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    slow_diff2 = read_pmccntr();
    asm volatile ("isb");
    printf("what about access 10, which has beed accessed before: %" PRId64 "\n",slow_diff2 - slow_diff1);

    asm volatile ("dc civac, %0" : : "r" (&volatile_array[10 * 4096]) : "memory");
    //asm volatile ("dsb ish"); // Data synchronization barrier ensure invalidation has completed
    asm volatile ("isb"); // Insert isb for serialization after cache flush

    asm volatile ("isb");
    asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff1));
    slow_diff1 = read_pmccntr();
    asm volatile ("isb");
    temp = volatile_array[10 * 4096];
    asm volatile ("dsb ish");
    asm volatile ("isb");
    // asm volatile ("MRS %0, cntvct_el0" : "=r" (slow_diff2));
    slow_diff2 = read_pmccntr();
    asm volatile ("isb");
    printf("what about access 10 again after accurately flush element 10: %" PRId64 "\n",slow_diff2 - slow_diff1);


    // asm volatile("MRS %0, cntvct_el0 " : "=r" (temp_disp2));
    // asm volatile ("isb"); // Serialize after reading the counter
    // diff = temp_disp2 - temp_disp1;
    // printf("time spent to run the whole program: %d\n",diff);
    read_pmccntr();

   return 0;
}
