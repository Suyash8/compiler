#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    printf("Line one\n");
    printf("Line two\n");
    int x = add(2, 3);
    printf("x=%d\n", x);
    return 0;
}
