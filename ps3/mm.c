#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

uint32_t enigmatic_computation(uint64_t value);

uint32_t enigmatic_computation(uint64_t value)
{
    int sum = 0;
    while (value)
    {
        sum += value % 10;
        value /= 10;
    }
    return (sum > 9) ? enigmatic_computation(sum) : sum;
}

int main()
{
    uint64_t total = 0;
    while (1)
    {
        int ch = getchar();
        if (ch == EOF || ch == '\n' || ch == '\r')
        {
            break;
        }
        if ('0' <= ch && ch <= '9')
        {
            total += ch - '0';
        }
    }
    printf("%u\n", enigmatic_computation(total));
}
