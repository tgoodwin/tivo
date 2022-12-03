#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>


volatile int counter = 0;

void counter_inc()
{
	int observed;
	do {
		observed = counter;
	} while(!__sync_bool_compare_and_swap_4(&counter, observed, observed + 1));
}

void* thread_main(void* p)
{
	int i;
	for(i = 0; i < 100000000; i ++)
		if(i % 2 == 0) counter_inc();
	return NULL;
}
#define N 5

int main()
{
	pthread_t t[N];
	int i;
	for(i = 0; i < N; i ++)
		pthread_create(&t[i], NULL, thread_main, NULL);

	for(i = 0; i < N; i ++)
		pthread_join(t[i], NULL);

	printf("%d\n", counter);

	return 0;
}