.data
x:
.int 10
y:
.int 20
z:
.int 0

.text

# The function 'my_add' adds the values in the global 
# variables x and y and stores the result in global z.

my_add :

   # Function Body Begins
   movl   x,%eax
   addl   y,%eax
   movl   %eax,z
   # Function Body ends
  
   ret

 .globl _main
_main :
   call my_add

   mov $0,%eax  # returning 0 to keep the OS happy

   ret
   
.end
