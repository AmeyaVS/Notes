
.data
year :
.int 2005

str1 :
.ascii "Hello World \n\0"

str2 :
.ascii "Hello World %d \n\0"

.text

  .globl _main

_main:

    # Equivalant of doing printf("Hello World \n");

    pushl $str1     # Push the address of the str1 as argument
    call _printf    # Calling the C Library Function - printf
    addl $4,%esp    # Reset the SP to 'remove' the pushed items

    # Equivalant of doing printf("Hello World %d \n",year);

    pushl year      # push the Last argument first
    pushl $str2     # Pushing the Last but one (first) argument
    call _printf    # Calling the C Library Function - printf
    addl $8,%esp    # Reset the SP to 'remove' the pushed items

    mov $0,%eax     # Return 0 to keep the OS Happy

    ret

.end
