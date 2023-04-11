with Ada.Text_IO; use Ada.Text_IO;
package body mytst is
	procedure myproc(   a:in integer; b:in out integer ) is
	begin   
		b := a + 20 ;
	end myproc;
end mytst;
