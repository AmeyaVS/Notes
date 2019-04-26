#include "nand.h"
#include "testbench.h"
//#include "factory.h"

int sc_main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  //factory::add<test_driver<bool, nand<bool> > > td;//("td");

  test_driver<bool, nand<bool> > td("td");
  sc_core::sc_start();

  return 0;
}
