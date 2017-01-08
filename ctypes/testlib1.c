#include <stdio.h>

/*
gcc -shared -Wl,-soname,testlib -o testlib1.so -fPIC testlib1.c
python test1.py
*/

void myprint(void);
int add(int x, int y);
void set(char* x, int size);

void myprint()
{
    printf("hello world\n");
}

int add(int x, int y)
{
    printf("%d\n", x+y);
    return x+y;
}

void set(char* x, int size)
{
    x[0] = 'c';
    printf("%d\n", size);
}

