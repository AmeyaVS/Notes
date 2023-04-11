PROGRAM sample(input,output);
VAR p1,p2,p3 : integer;

	FUNCTION my_func(f1,f2:integer): integer;
	BEGIN
		if (f1 > f2 ) 
		then 
			my_func := f1
		else 
			my_func := f2;

		f1 := 100 ;{ Changing the Value of Formal Parameter }
		f2 := 120 ;{ Changing the Value of Formal Parameter }

	END;

BEGIN

	p1 := 4;
	p2 := 30;

	writeln('Before the function call p1=',p1,' p2=',p2);

	p3 := my_func(p1,p2);

	writeln('After  the function call p1=',p1,' p2=',p2);

END.
