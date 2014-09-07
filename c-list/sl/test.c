#include <stdio.h>
 
 //$ gcc -c -Wall -Werror -fpic test.c
//gcc -shared -o libtest.so test.o
char* test(void)
{
    return "Hello, I'm a shared library";
}
