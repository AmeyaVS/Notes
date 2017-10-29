#ifndef TOP_H_
#define TOP_H_

#include "producer.h"
#include "consumer.h"
#include "passthrough.h"

SC_MODULE(top) {
  private:
    producer p;
    consumer c;
    sc_core::sc_vector<passthrough> buffer;

    sc_core::sc_vector<sc_core::sc_signal<unsigned int> > channel;
    sc_core::sc_clock clk;

  public:
  top(sc_core::sc_module_name name_, unsigned int n = 1):
    p("p")
    , c("c")
    , buffer("buffer", n)
    , channel("channel", n + 1)
    , clk("clk", 5, sc_core::SC_NS, 0.5)
  {
    // Binding clk signal to clk input port of producer.
    p.clk(clk);
    // Binding producer output port to channel 
    p.out(channel[0]);

    // Binding passthrough
    for(size_t i = 0; i < n; ++i) {
      // Binding passthrough buffer input port.
      buffer[i].in(channel[i]);
      // Binding passthrough buffer output port.
      buffer[i].out(channel[i + 1]);
    }

    // Binding consumer port to channel.
    c.in(channel[n]);
  }
}; // top

#endif // TOP_H_
