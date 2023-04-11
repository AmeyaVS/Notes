PROGRAM sample(input,output);
VAR a,b: integer;
 
	PROCEDURE P1();
	VAR a,b : integer;
	VAR c : integer;
		PROCEDURE P2();
		VAR a,b : integer;
		BEGIN
				a:=10; (* This should resolve to 'a' defined in P2 *)
				b:=20; (* This should resolve to 'b' defined in P2 *)
				c:=25; (* This should resolve to 'c' defined in P1 *)
		END;
	BEGIN
		c :=40;
		writeln('Value of c is ',c); (* c is 40 here *)
		P2();
		writeln('Value of c is ',c); (* c is 25 here *)
	END;
BEGIN
	P1();
END.


