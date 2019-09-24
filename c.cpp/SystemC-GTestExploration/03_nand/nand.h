#ifndef NAND_H_
#define NAND_H_

#include <systemc>

template<typename T = bool>
SC_MODULE(nand) {
  sc_core::sc_vector<sc_core::sc_in<T> > in;//{"in", 2};
  sc_core::sc_out<T> out;//{"out"};

  SC_CTOR(nand):
    in("in", 2),
    out("out")
  {
    SC_METHOD(do_nand);
    //for(auto i : in) {
      sensitive << in[0] << in[1];
    //}
    dont_initialize();
  }

  void do_nand() {
    out = ~(in[0] & in[1]);
  }

 }; //< nand

#endif // NAND_H_

template<>
inline void nand<>::do_nand() {
	out = !(in[0].read() && in[1].read());
}
