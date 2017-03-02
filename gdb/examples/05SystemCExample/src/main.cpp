// Include SystemC Header
#include <systemc>
// Include top module.
#include "top.h"

// Entry Point
int sc_main(int argc, char* argv[]) {
  unsigned int n = 1;
  if(argc > 1)
    n = atoi(argv[1]);
  if (n >= 1) {
    // Instantiate top module.
    top t("top", n);

    // Start simulation.
    sc_start(100, sc_core::SC_NS);

    // Stopping simulation.
    sc_core::sc_stop();
  } else {
    std::cerr << "Kindly enter value >= 1" << std::endl;
    return -1;
  }
  return 0;
}
