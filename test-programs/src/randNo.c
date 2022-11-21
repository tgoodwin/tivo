#include<stdio.h>
#include<stdlib.h>

unsigned int MyRand(unsigned int start_range,unsigned int end_range)
{
    static unsigned int rand = 0xACE1U; 

    if(start_range == end_range) {
        return start_range;
    }

    rand += 0x3AD;
    rand %= end_range;
    
    while(rand < start_range){
        rand = rand + end_range - start_range;
    }

    return rand;
}

int main(void)
{
    int i;
    for (i = 0; i < 0xFF; i++)
    {
        printf("%u\t",MyRand(10,20));
    }
    return 0;
}