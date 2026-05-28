#include <stdio.h>

void foo()
{
    int x = 0;
}

int bar(int a)
{
    return a * 2;
}

int main()
{
    foo();
    int r = bar(5);
    printf("%d\n", r);
    return 0;
}
