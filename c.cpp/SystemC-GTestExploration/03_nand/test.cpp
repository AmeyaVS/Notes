#include <gtest/gtest.h>
#include <systemc>
#include <mutex>
#include "nand.h"
#include "testbench.h"

std::mutex g_singleThread;

void *test_driver_p = nullptr;
//template<typename TInt, typename TDut>
void *get_test_driver() {
  assert(test_driver_p);
  return test_driver_p;
}
class nand_test: public ::testing::Test {
  protected:
    //bool lock = false;
    test_driver<bool, nand<bool> > & tb;
    //nand_test(): tb(factory::get<nand<bool> >()) {
    nand_test(): tb(*static_cast<test_driver<bool, nand<bool> > *>(get_test_driver())) {
      //reset_dut();
    }
    
    ~nand_test() override {};

    void SetUp() override {
      //while(lock == true);
      g_singleThread.lock();
      reset_dut();
      std::cout << "SetUp\n";
      //lock = true;
    }
    void reset_dut() {
      tb.in[0].write(false);
      tb.in[1].write(false);
      //wait(sc_core::SC_ZERO_TIME);
      sc_core::wait();
    }

    void TearDown() override {
      //lock = false;
      g_singleThread.unlock();
      std::cout << "TearDown\n";
    }
}; //< nand_test

TEST_F(nand_test, nand00) {
  tb.in[0].write(false);
  tb.in[1].write(false);
  sc_core::wait();
  EXPECT_EQ(tb.out.read(), true);
}

TEST_F(nand_test, nand01) {
  tb.in[0].write(false);
  tb.in[1].write(true);
  //wait(sc_core::SC_ZERO_TIME);
  sc_core::wait();
  EXPECT_EQ(tb.out.read(), true);
}

TEST_F(nand_test, nand10) {
  tb.in[0].write(true);
  tb.in[1].write(false);
  //wait(sc_core::SC_ZERO_TIME);
  sc_core::wait();
  EXPECT_EQ(tb.out.read(), true);
}

TEST_F(nand_test, nand11) {
  tb.in[0].write(true);
  tb.in[1].write(true);
  //wait(sc_core::SC_ZERO_TIME);
  sc_core::wait();
  EXPECT_EQ(tb.out.read(), false);
}
