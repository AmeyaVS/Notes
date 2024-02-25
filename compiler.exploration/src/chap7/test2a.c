/*
	Common Sub-expression
*/
int a,b,c,d,e,f,g;

void func()
{

	int i,x;

	a = (b +c)*d ;
	e =  f * a ;
	f = (b+c)*e;
	g = d / (b+c);

}
