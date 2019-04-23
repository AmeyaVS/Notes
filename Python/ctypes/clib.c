#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int simple_function(void) {
    static int counter = 0;
    counter++;
    return counter;
}

void add_one_to_string(char *input) {
    size_t ii = 0;
    for (; ii < strlen(input); ii++) {
        input[ii]++;
    }
}

char * alloc_C_string(void) {
    char* phrase = strdup("I was written in C");
    printf("C just allocated %p(%ld):  %s\n",
           phrase, (long int)phrase, phrase);
    return phrase;
}

void free_C_string(char* ptr) {
    printf("About to free %p(%ld):  %s\n",
           ptr, (long int)ptr, ptr);
    free(ptr);
}

