#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

unsigned long ulrand(void)
{
    unsigned long ret;
    ret  = (((unsigned long)rand() << 24) & 0xFF000000ul);
    ret |= (((unsigned long)rand() << 12) & 0x00FFF000ul);
    ret |= (((unsigned long)rand()      ) & 0x00000FFFul);

    return ret;
}

int main(int argc, char *argv[])
{
    unsigned long ul;
    srand(time(NULL));
    for (int i = 0; i < 256; i++)
    {
        ul = ulrand();
        printf("%lu\t", ul);
    }
    return 0;
}

