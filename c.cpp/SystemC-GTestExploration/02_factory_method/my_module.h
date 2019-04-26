#ifndef MY_MODULE_H
#define MY_MODULE_H

#include <systemc>

struct my_module : public sc_core::sc_module {
  my_module(const sc_core::sc_module_name& name): sc_core::sc_module(name) {
    // TODO: Incomplete implementation
    SC_HAS_PROCESS(my_module);
    SC_METHOD(flip_flop_impl);
    sensitive << aclk.pos()
              << areset_n.neg();
    dont_initialize();
  }

  void flip_flop_impl() {
    if(areset_n.read()) {
      out.write(in.read());
    } else {
      out.write(false);
    }
  }

  sc_core::sc_in<bool> aclk{"aclk"};
  sc_core::sc_in<bool> areset_n{"areset_n"};
  sc_core::sc_in<bool> in{"in"};
  sc_core::sc_out<bool> out{"out"};
}; //< my_module

#endif /* MY_MODULE_H */
