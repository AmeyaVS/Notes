/* Prototype */
int printf();

struct my_data
{
	int age;
	int student_id;
}d1;

int main()
{
	/* Initialization */
	d1.age=60;
	d1.student_id=1234;

	printf("Before : age=%d student_id=%d \n",d1.age,d1.student_id);

	d1.student_id=4567;
	d1.age=20;

	printf("After  : age=%d student_id=%d \n",d1.age,d1.student_id);

	return(0);
	
}
