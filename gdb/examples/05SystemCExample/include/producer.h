#ifndef PRODUCER_H_
#define PRODUCER_H_

#include <systemc>
#include <ctime>

SC_MODULE(producer) {
  public:
    sc_core::sc_in<bool> clk; //< Clock input port.
    sc_core::sc_out<unsigned int> out; //< Output data port.

  private:
    void do_work(void) {
      unsigned int val = (rand() % 256);
      std::cout << this->name()
                << ": Driving @ "
                << sc_core::sc_time_stamp()
                << ": "
                << std::hex << val
                << std::endl;
      out.write(val);
    }

  public:
  SC_CTOR(producer):
    clk("clk")
    , out("out")
  {
    srand(time(NULL));
    SC_METHOD(do_work);
    sensitive << clk.pos();
    dont_initialize();
  }
}; // producer

#endif // PRODUCER_H_
