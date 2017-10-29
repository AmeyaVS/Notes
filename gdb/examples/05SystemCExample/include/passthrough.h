#ifndef PASSTHROUGH_H_
#define PASSTHROUGH_H_

#include <systemc>

SC_MODULE(passthrough) {
  public:
    sc_core::sc_in<unsigned int> in; //< Input port
    sc_core::sc_out<unsigned int> out; //< Output port

  private:
    void do_work(void) {
      unsigned int val = 0;
      while(1) {
        wait();
        val = in.read();
        std::cout << this->name()
                  << ": Buffering @ "
                  << sc_core::sc_time_stamp()
                  << ": "
                  << std::hex << val
                  << std::endl;
        out.write(val);
      }
    }

  public:
    SC_CTOR(passthrough):
      in("in")
      , out("out")
  {
    SC_THREAD(do_work);
    sensitive << in;
  }
}; // passthrough

#endif // PASSTHROUGH_H_
