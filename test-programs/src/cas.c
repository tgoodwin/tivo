#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


bool __tivo_sync_bool_compare_and_swap_4(volatile int32_t* data,
                                         volatile int32_t expected,
                                         volatile int32_t desired) {
  bool result = __sync_bool_compare_and_swap_4(data, expected, desired);
  printf("TODO: do record or replay\n");
  return result;
}


int atomic_add(int* ptr, int value)
{
    while(true) {
      int original_value = *ptr;
      int new_value = original_value + value;
      int old = __sync_bool_compare_and_swap_4(ptr, original_value, new_value);
          if (old == original_value) {
          return old; 
    }
  }
}


int main(int argc, char** argv) {
  int32_t data, expected, desired;
  int a = __sync_bool_compare_and_swap_4(&data, expected, desired);

  int variable;
  int *ptr=&variable;
  *ptr=20;
  atomic_add(ptr, 30);

}
