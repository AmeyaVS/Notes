.data

ret_val :
.int 0

str1 :
.ascii "Hello World\0"

str2 :
.ascii "Return value of strlen is %d \n\0"

.text
  .globl _main

.align 4
_main:

    # Equivalant of doing ret_val = strlen(str1);

    pushl $str1            # Push the address of the str1 as argument
    call _strlen           # Calling the C Library Function - strlen
    movl  %eax,ret_val     # Moving the return value into variable ret_val
    addl $4,%esp           # Reset the SP to 'remove' the pushed arguments

    # Displaying the return value by using the equivalant of 
    # printf("Return value of strlen is %d \n",ret_val);

    pushl ret_val          # push the Last argument first
    pushl $str2            # Pushing the Last but one (first) argument
    call _printf           # Calling the C Library Function - printf
    addl $8,%esp           # Reset the SP to 'remove' the pushed arguments

    mov $0,%eax            # Returning 0 to keep the OS happy

    ret

.end
