/* Constants from fdlibm, via newlib 1.8.2 libm/math, under this notice:
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* e_sqrt.c's constants. __ieee754_sqrt never loads them (gcc folds
   one-tiny and friends), so they are spelled as the values: a static const
   the code does not reference would be data the build cannot place. */
#define one 1.0
#define tiny 1.0e-300
