/*
A return statement in 'C' generates a 'return'
in IC followed by a jump to the end of the procedure
if the return statement is the last statement of 
the routine . the IC generated would be as follows

goto .L0
label .L0

It is replaced in peephole optimization as

label .L0

because the flow of control itself takes it to the next
statement in IC


*/


int func(int a,int b,int c)
{
	int v1;

	v1=a+b+c;

	return(v1);
}
