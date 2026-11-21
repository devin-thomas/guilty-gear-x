/* Guilty Gear X US (SLUS-202.87): 2 functions ported from the X Plus
   decomp (ggxeuc/random.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/random.h"

void RandomInit(int num) {
    RandomSeed = num;
}

int Random(void) {
    RandomSeed = RandomSeed * 0x1579 + 0x7FA9;
    return RandomSeed & 0xFFFF;
}