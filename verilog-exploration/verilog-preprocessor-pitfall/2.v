module top2;
 
`ifdef A
    initial begin
        $display("Yarr! (2)\n");
    end
`endif
 
//This will warn us if we're redefining a macro
`define A B
 
endmodule
