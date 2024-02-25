/*
	Copy propagation : Implicitly created due to cse

	In this example, copy propagation gets trigerred due
	to Common subexpression Elimination.

	After elimination of common subexpression 'b+c', the IC
	looks like follows.

	(1) _t4 := b + c
	(2) e := _t4
	(3) if a > 10 goto .L0
	(4) goto .L1
	(5) label .L0
	(6) d := _t4
	(7) goto .L2
	(8) label .L1
	(9) d := b - c
	(10) label .L2
	(11) f := d * e

	copy propagation propagates the value of 'e' (which is 
	equal to _t4) to the statement 11 and eliminates the assignment 
	statement 2. The resultant IC is

	(1) _t4 := b + c
	(2) if a > 10 goto .L0
	(3) goto .L1
	(4) label .L0
	(5) d := _t4
	(6) goto .L2
	(7) label .L1
	(8) d := b - c
	(9) label .L2
	(10) f := d * _t4


*/

int func(int a,int b, int c)
{
	int d,e,f;

	e = b + c;

	if(a > 10 ){
		d = b + c;
	}else{
		d = b - c;
	}

	f = d * e ;

	return(f);
}
