
.data
x:
.int 10
y:
.int 20
z:
.int 0

str2 :
.ascii "The Sum of two numbers x=%d y=%d is z=%d \n"

.text

# The function 'my_add' adds the values in the global variables x and y
# and stores the result in another global z.

my_add :

   # Function Body Begins
   pushl   %ebp           # Saving the current BP
   movl    %esp,%ebp      # Moving the SP to BP so that 
                          # function arguments can be accesed using
                          # indirect addressing via BP

   movl    8(%ebp),%ecx   # Moving the First Argument to %ecx
   addl    12(%ebp),%ecx  # Adding the second argument to the 
                          # content of %ecx and store it in %ecx


   movl    %ecx,%eax      # The return value has to be stored in %eax

   movl    %ebp,%esp      # Restoring the SP to what it was at the 
                          # entry of function 
   popl    %ebp           # Restoring the value of BP

   # Function Body ends
  
   ret

 .globl _main
_main :

  # Pushing the Two arguments for my_add(x,y)
  pushl y
  pushl x
  call my_add
  movl %eax,z
  addl $8,%esp

  # Displaying the return value by using the equivalant of
  # printf("The Sum of two numbers x=%d y=%d is z=%d \n",x,y,z);
  
  pushl z                # push the Last argument first
  pushl y                # push the Last but one argument next
  pushl x                # push the next argument in the reverse order
  pushl $str2            # Pushing the Last but one (first) argument
  call _printf           # Calling the C Library Function - printf
  addl $16,%esp          # Reset the SP to 'remove' the pushed arguments

  mov $0,%eax            # returning 0 to keep the OS happy


  ret
  
.end
