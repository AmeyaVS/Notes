#include "my_module.h"
#include "factory.h"

#include <gtest/gtest.h>
#include <systemc>

class dut {
public:
    sc_core::sc_clock aclk{"aclk"};
    sc_core::sc_signal<bool> areset_n{"areset_n"};
    sc_core::sc_signal<bool> in{"in"};
    sc_core::sc_signal<bool> out{"out"};

    dut() {
        m_dut.aclk(aclk);
        m_dut.areset_n(areset_n);
        m_dut.in(in);
        m_dut.out(out);
    }
private:
    my_module m_dut{"my_module"};
};

static factory::add<dut> g;

TEST(my_module, simple) {
    auto test = factory::get<dut>();

    test->areset_n = 0;
    test->in = 0;
    sc_start(3, sc_core::SC_NS);

    test->areset_n = 1;
    test->in = 1;
    sc_start(3, sc_core::SC_NS);

    EXPECT_TRUE(test->out.read());
}

