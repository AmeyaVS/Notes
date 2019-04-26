#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#include <systemc>
#include <gtest/gtest.h>

template<typename TInt, typename TDut>
SC_MODULE(test_driver) {

  sc_core::sc_vector<sc_core::sc_buffer<TInt> > in;//{"in", 2};
  sc_core::sc_buffer<TInt> out;//{"out"};

  SC_CTOR(test_driver):
    in("in", 2),
    out("out")
  {
    dut.in[0](in[0]);
    dut.in[1](in[1]);
    dut.out(out);

    SC_THREAD(test_thread);
    sensitive << clk.posedge_event();

    register_test_driver(this);
  }

  private:

    void test_thread() {
      if(RUN_ALL_TESTS()) {
        SC_REPORT_ERROR("GTest", "Some Test FAILED!");
      } else {
        SC_REPORT_INFO("GTest", "All test PASSED");
      }
      sc_core::sc_stop();
    }
    TDut dut{"dut"};
    sc_core::sc_clock clk{"clk", 10, sc_core::SC_NS};
}; //< test_driver


extern void* test_driver_p;

template<typename TInt, typename TDut>
void register_test_driver(test_driver<TInt, TDut> *tb) {
  test_driver_p = tb;
}



#endif // TESTBENCH_H_
