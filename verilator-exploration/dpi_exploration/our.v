// Sample/Example Code from Verilator webpage
import "DPI-C" function int add (input int a, input int b);

module our(clk);
    input clk;
    always @ (posedge clk)
        begin
            $display("Hello World");
            $display("%x + %x = %x", 1, 2, add(1,2));
            $finish;
        end

endmodule
