with mytst;
with Ada.Text_IO, Ada.Integer_Text_IO;
use Ada;
procedure ex9 is
x: integer ;
y: integer ;
begin
	x :=10;
	y :=20;
	Text_IO.Put ("Value of 'y' before the procedure call:");
	Integer_Text_IO.Put (y);
	mytst.myproc(x,y);
	Text_IO.New_Line;
	Text_IO.Put ("Value of 'y'  after the procedure call:");
	Integer_Text_IO.Put (y);
end ex9;
