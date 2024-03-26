#include <stdio.h>

int a = 0;

int print(char* str) {
    return printf("%s: %d\n", str, a++);
}	
