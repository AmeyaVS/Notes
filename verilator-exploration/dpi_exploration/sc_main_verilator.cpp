// Entry point for the top verilated module
#include "Vour.h"

// SystemC Entry point
int sc_main(int argc, char **argv)
{
    // Parse Verilator command line arguments
    Verilated::commandArgs(argc, argv);
    // Simulation clock for verilated module.
    sc_clock clk("clk", 10, SC_NS, 0.5, 3, SC_NS, true);
    Vour *top;
    top = new Vour("top");
    // Bind clk to top module.
    top->clk(clk);
    while (!Verilated::gotFinish()) //< wait till $finish
    {
        // Step simulation.
        sc_start(1, SC_NS);
    }
    delete top;
    exit(0);
}