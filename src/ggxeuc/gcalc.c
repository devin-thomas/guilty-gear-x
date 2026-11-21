/* Guilty Gear X US (SLUS-202.87): 5 functions ported from the X Plus
   decomp (ggxeuc/gcalc.c), byte-checked against GGX by
   the build gate. Declarations are X Plus's; GGX layouts live in include/. */
#include "generated/gcalc.h"

int g_roundmax(int val, int max) {
    if (max < val) {
        return max;
    }
    return val;
}

int g_roundmin(int val, int min) {
    if (val < min) {
        return min;
    }
    return val;
}

int g_round(int val, int max, int min) {
    if (max < val) {
        return max;
    }
    if (val < min) {
        return min;
    }
    return val;
}

void f2int(float *f) {
    *f = (int)*f;
}

float nlSqrt(float src);

float G_GetDist(float x, float y) {
    return nlSqrt(x * x + y * y);
}