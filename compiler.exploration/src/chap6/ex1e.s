
.data
x:
.int 10
y:
.int 20
z:
.int 30

str1 :
.ascii "The numbers before compare x=%d y=%d z=%d \n\0"

str2 :
.ascii "The numbers after  compare x=%d y=%d z=%d \n\0"

.text

 .globl _main
_main :

  # printf("The numbers before compare x=%d y=%d z=%d \n",x,y,z);
  pushl z                # push the Last argument
  pushl y                # push the Last but one argument
  pushl x                # push the next argument in the reverse order
  pushl $str1            # Pushing the first argument
  call _printf           # Calling the C Library Function - printf
  addl $16,%esp          # Reset the SP to 'remove' the pushed arguments

  movl    x,%eax         # Moving x into reg eax
  cmpl    %eax,y         # Comparing y with x
  jl      lbl1           # jump to lbl1, if y is less than x

  movl    $50,z          # move 50 into z, if y is >= x
  jmp     lbl3           # jumping to lbl3

lbl1:
   movl    $100,z         # move 100 into z, if x is < y

lbl3:

  # printf("The numbers after compare x=%d y=%d z=%d \n",x,y,z);
  pushl z                # push the Last argument
  pushl y                # push the Last but one argument
  pushl x                # push the next argument in the reverse order
  pushl $str2            # Pushing the first argument
  call _printf           # Calling the C Library Function - printf
  addl $16,%esp          # Reset the SP to 'remove' the pushed arguments


  mov $0,%eax            # returning 0 to keep the OS happy

  ret
  
.end
