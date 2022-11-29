#include <stdio.h>
#include <time.h>

int x = 0;
int main() {
  printf("%d ", (x = 3) + (x = 4));
  printf("%d\n", x);
  return 0;
}