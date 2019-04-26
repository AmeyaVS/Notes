#include "nand.h"

template<>
void nand<bool>::do_nand() {
  out = !(in[0].read() && in[1].read());
}
