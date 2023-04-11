/* 
	Constant Folding 
	Access of array elements using constant valued 
	indexes like arr[0],arr[1] in the input source
	e.g.

	arr[0]=3;
	arr[1]=4;
	
	generates IC statements as follows
	(1) _t0 := 0 * 4
	(2) _t1 := &arr1
	(3) _t1[_t0] := 3
	(4) _t2 := 1 * 4
	(5) _t3 := &arr1
	(6) _t3[_t2] := 4

	Constant folding can replace the IC statements 
	(1), (4) with

	(1) _t0 := 0
	(4) _t2 := 4

	Later, copy propagation of the same would yield

	(1) _t1 := &arr1
	(2) _t1[0] := 3
	(3) _t1[4] := 4


*/

int arr[45];

int main()
{

	arr[0]=3;
	arr[1]=4;

}
