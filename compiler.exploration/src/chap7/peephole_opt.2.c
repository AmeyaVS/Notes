/* access to elements in the array of type 'char'
   generates IC statements of the form

	_t0 := index * 1

	It is replaced by peephole optimization
	with

	_t1 := index

	This is an example of algebric Identity being
	used in peephole optimization.
*/

char arr[10];
int func(int index,char c)
{

	arr[index]=c;
}
