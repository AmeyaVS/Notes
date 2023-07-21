`include "1.vh"
 
module top;
top2 t();
 
`ifdef A
    initial begin
        $display("Yarr! (1)\n");
    end
`endif
 
endmodule
