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

void add10(int* x){
  int expected=*x;
   while (__sync_bool_compare_and_swap_4(x, expected, expected + 10)
            != expected) {
        expected = *x;
        }
}

int main(int argc, char** argv) {
  int32_t data, expected, desired;
  int a = __sync_bool_compare_and_swap_4(&data, expected, desired);

  add10(5);
}
