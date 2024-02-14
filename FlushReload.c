#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

uint8_t array[256 * 128];
int temp;
unsigned char secret = 94;
/* cache hit time threshold assumed*/
#define CACHE_HIT_THRESHOLD (300)
#define DELTA 32

static inline uint64_t read_pmccntr(void)
{
  uint64_t val;
  asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
  return val;
}
void victim()
{
  temp = array[secret * 128 + DELTA];
}
void flushSideChannel()
{
  int i;
  // Write to array to bring it to RAM to prevent Copy-on-write
  for (i = 0; i < 256; i++)
    array[i * 128 + DELTA] = 1;
  // flush the values of the array from cache
  for (i = 0; i < 256; i++)
  {
    asm volatile("dc civac, %0" : : "r"(&array[i * 128 + DELTA]) : "memory");
    asm volatile("isb");
  }
}

void reloadSideChannel()
{
  int junk = 0;
  register uint64_t time1, time2, time_diff;
  volatile uint8_t *addr;
  int i;
  for (i = 0; i < 256; i++)
  {
    // if (i == 31)
    // {
    //   for (int j = 0; j < 256; j++)
    //   {
    //     asm volatile("dc civac, %0" : : "r"(&array[j * 128 + DELTA]) : "memory");
    //     asm volatile("isb");
    //   }
    // }
    
    addr = &array[i * 128 + DELTA];
    asm volatile("isb");
    time1 = read_pmccntr();
    junk = *addr;
    time2 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    time_diff = time2 - time1;
    printf("~~~~~~~~~\n");
    printf("%d\n", i);
    printf("~~~~~~~~~\n");
    printf("time_diff is %" PRId64 "\n", time_diff);
    if (time_diff <= CACHE_HIT_THRESHOLD)
    {
      printf("array[%d*128 + %d] is in cache.\n", i, DELTA);
      printf("The Secret = %d.\n", i);
    }
  }
}

int main(int argc, const char **argv)
{
  flushSideChannel();
  victim();
  reloadSideChannel();
  return (0);
}
