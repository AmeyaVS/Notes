struct my_data
{ 
   int student_id;
   int age;
}d1;  
   
   
/* Function */
 int main()
{
   struct my_data *ptr; 
 
   ptr = &d1;
 
   /* Access by '.' operator */
   d1.age=20;
 
   /* Access by '->' operator */
   ptr->age=20;
   
}
