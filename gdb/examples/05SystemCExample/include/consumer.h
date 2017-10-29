#ifndef CONSUMER_H_
#define CONSUMER_H_

#include <systemc>

SC_MODULE(consumer) {
  public:
    sc_core::sc_in<unsigned int> in; //< Input Port.

  private:
    void blackhole(void) {
      unsigned int val = in.read();
      std::cout << this->name()
                << ": Reading @ "
                << sc_core::sc_time_stamp()
                << ": "
                << std::hex << val
                << std::endl;
    }

  public:
  SC_CTOR(consumer):
    in("in")
  {
    SC_METHOD(blackhole);
    sensitive << in;
    dont_initialize();
  }

}; // consumer

#endif // CONSUMER_H_
