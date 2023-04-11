struct dob{
	int day;
	int mon;
	int year;
}ab;

struct my_data
{
	int age;
	int student_id;
	struct dob student_dob;
}d1;


/* Function */
int main()
{
	/* Local Variables */
	struct my_data *ptr;

	/* Initialization */
	ptr=&d1;

	ptr->age=20;
	ptr->student_id=4567;

	ptr->student_dob.mon=11;
}
