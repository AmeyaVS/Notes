PROGRAM sample(input,output);
VAR v0 : integer;

	PROCEDURE P1(param1:integer);
	VAR v1 : integer;

		PROCEDURE P2();
		VAR v2: integer;
		BEGIN
			v2:=20; 
			v1:=25; (* 'v1' defined in P1 *)
		END;

		PROCEDURE Peer_Of_P2();
		BEGIN
			P2();
		END;


		PROCEDURE Another_Peer_Of_P2();
			PROCEDURE P3();
			BEGIN
				P2();
			END;
		BEGIN
		END;

	BEGIN
		if (param1 > 100 ) 
		then 
			P2() 
		else if( param1 > 50 )
		then
			Peer_Of_P2() 
		else 
			Another_Peer_Of_P2() ;

	END;

BEGIN
	write('Give Input :');
	readln(v0);
	P1(v0);
END.

			
		

