#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define stride (256)
#define TOP_N (10)
uint8_t array[256 * stride];
volatile uint8_t temp;
unsigned char secret = 18;
/* cache hit time threshold assumed*/
#define CACHE_HIT_THRESHOLD (130)
#define DELTA 64
int rank[10];
int ascii_hit_counter[256];

typedef struct
{
  int value;
  int index;
} IndexedValue;

// Function to swap two IndexedValue elements
void swap(IndexedValue *a, IndexedValue *b) {
    IndexedValue t = *a;
    *a = *b;
    *b = t;
}

// Partition function for quicksort
int partition(IndexedValue arr[], int low, int high) {
    int pivot = arr[high].value; // pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is larger than or equal to pivot
        if (arr[j].value >= pivot) {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// The main function that implements QuickSort
void quickSort(IndexedValue arr[], int low, int high) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

static inline uint64_t read_pmccntr(void)
{
  uint64_t val;
  asm volatile("mrs %0, pmccntr_el0" : "=r"(val));
  return val;
}
void victim()
{
  temp = array[secret * stride + DELTA];
}
void flushSideChannel()
{
  int i;
  // flush the values of the array from cache
  for (i = 0; i < 256; i++)
  {
    asm volatile("dc civac, %0" : : "r"(&array[i * stride + DELTA]) : "memory");
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

    temp = array[mix_i * stride + DELTA];
    asm volatile("dsb ish"); // Data synchronization barrier after volatile access
    asm volatile("isb");

    time2 = read_pmccntr();
    asm volatile("isb"); // Serialize after reading the counter
    time_diff = time2 - time1;

    // printf("~~~~~At mix_i = %d~~~~~\ntime_diff: %d\n\n", mix_i, time_diff);
    if (time_diff <= CACHE_HIT_THRESHOLD)
    {
      // printf("array[%d*%d + %d] is in cache.\n", mix_i, stride, DELTA);
      // printf("The Secret = %d.\n", mix_i);
      // printf("hit time is %d\n", time_diff);
      // printf("\n");
      ascii_hit_counter[mix_i] += 1;
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
  int i = 0;
  IndexedValue pairs[256];
  for (i = 0; i < 256 * stride; i++)
  {
    array[i] = i;
    if((i < 256)){
      ascii_hit_counter[i] = 0;
    }

  }

  for (i = 0; i < 1; i++)
  {
    flushSideChannel();
    victim();
    reloadSideChannel();
  }
  // Fill the pairs array
  for (i = 0; i < 256; i++)
  {
    pairs[i].value = ascii_hit_counter[i];
    pairs[i].index = i;
  }
  // Sort the pairs by their value in descending order
  quickSort(pairs, 0, 256 - 1);

  // Print the top TOP_N elements
  printf("Top TOP_N elements with the highest values:\n");
  for (int i = 0; i < TOP_N; i++)
  {
    printf("Index: %d, Value: %d\n", pairs[i].index, pairs[i].value);
  }

  return (0);
}
