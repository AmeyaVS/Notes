
.data
x:
.int 10
y:
.int 20

str1 :
.ascii "The two numbers before swap x=%d y=%d \n\0"

str2 :
.ascii "The two numbers after swap  x=%d y=%d \n\0"

.text

# The function 'swap' swaps the values in the global variables x and y
# by using a temporary local variable - tmp on the stack

swap :

   # Function Prolog begins
   pushl   %ebp           # Saving the current BP
   movl    %esp,%ebp      # Moving the SP to BP so that 
                          # function arguments can be accesed using 
                          # indirect addressing via BP
                           
   # Creating local storage space
   subl    $4,%esp        # Creating 4 bytes of Local storage

   # Function Prolog ends

   # Function Body Begins

   # tmp = x ;
   movl    x,%eax         # Moving x into register eax
   movl    %eax,-4(%ebp)  # Moving it into local variable (tmp)

   # x = y ;
   movl    y,%eax         # Moving y into register x
   movl    %eax,x         # Moving it into x

   # y = tmp ;
   movl    -4(%ebp),%eax  # Moving local variable (tmp) to reg x
   movl    %eax,y         # Moving reg x into y

   # Function Body ends


   # Function Epilog begins
   movl    %ebp,%esp      # Restoring the SP to what it was at the 
                          # entry of function 
   popl    %ebp           # Restoring the value of BP

   # Function Epilog ends
  
   ret

 .globl _main
_main :

  # printf("The two numbers before swap x=%d y=%d \n",x,y);
  pushl y                # push the Last argument
  pushl x                # push the next argument in the reverse order
  pushl $str1            # Pushing the Last but one (first) argument
  call _printf           # Calling the C Library Function - printf
  addl $12,%esp          # Reset the SP to 'remove' the pushed arguments

  call swap              # Calling the swap function


  # printf("The two numbers after swap x=%d y=%d \n",x,y);
  pushl y                # push the Last argument
  pushl x                # push the next argument in the reverse order
  pushl $str2            # Pushing the Last but one (first) argument
  call _printf           # Calling the C Library Function - printf
  addl $12,%esp          # Reset the SP to 'remove' the pushed arguments


  mov $0,%eax            # returning 0 to keep the OS happy

  ret
  
.end
