#include <stdio.h>

typedef unsigned int uint32_t;

typedef struct _REG {
	typedef union _bitFields {
		typedef struct _bits {
			uint32_t TBUF_EMPTY : 5;
			uint32_t TBUF_FULL : 5;
			uint32_t OFF_CNT_THRS : 6;
			uint32_t ON_CNT_THRS : 6;
			uint32_t Reserved : 10;
		} bits;

		bits REG0_bits;

		uint32_t AllBits;
	} bitFields;
	bitFields REG0_U;

	static const uint32_t WriteMask;
	static const uint32_t ReadMask;
} REG;

const uint32_t REG::WriteMask = 0xFF;
const uint32_t REG::ReadMask = 0xFF;


int main() {
	REG a;
	printf("size: %d\n",  sizeof(a));
  a.REG0_U.REG0_bits.TBUF_EMPTY = 0x5;
  return 0;
}
