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
	int h,j,k;

	/* Initialization */
	d1.age=20;
	d1.student_id=4567;
	d1.student_dob.day=17;
	d1.student_dob.mon=11;
	d1.student_dob.year=1967;
}
