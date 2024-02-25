/* 
	access to the first field in the above 
	structure generates IC statements of the form

	_t1 := _t2 + 0

	It is replaced by peephole optimization
	with

	_t1 := _t2

	This is an example of algebric Identity being
	used in peephole optimization.

*/
struct my_struct
{
	int f1[20];
	int f2;
} xyz;

int func(int index)
{
	xyz.f1[index]=34;
}
