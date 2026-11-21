/* This file is derived from GCC's libgcc2.c (2.95 series), whose notice (GCC 2.95.3's) follows. */
/* Copyright (C) 1989, 92-98, 1999 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */
/* GCC libgcc source: GNU GPL version 2 or later, with the libgcc exception
   (THIRD_PARTY_NOTICES.md; the licence text is libs/sony-sdk/COPYING.GPL-2.0). */
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libgcc/_umoddi3.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libgcc.a member _umoddi3.o */
#include "libgcc2.h"

typedef int SItype;

typedef unsigned int USItype;

typedef long long DItype;

typedef unsigned long long UDItype;

struct DIstruct {
    SItype low, high;
};

typedef union {
    struct DIstruct s;
    DItype ll;
} DIunion;

#define SI_TYPE_SIZE 32
#define __BITS4 (SI_TYPE_SIZE / 4)
#define __ll_B (1L << (SI_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((USItype)(t) % __ll_B)
#define __ll_highpart(t) ((USItype)(t) / __ll_B)

#include "_umoddi3_k.h"


/* count_leading_zeros is longlong.h's generic C form, kept whole, the
 * branch for word sizes above 32 included. */
#define count_leading_zeros(count, x)                                                              \
    do {                                                                                           \
        USItype __xr = (x);                                                                        \
        USItype __a;                                                                               \
                                                                                                   \
        if (SI_TYPE_SIZE <= 32) {                                                                  \
            __a = __xr < ((USItype)1 << 2 * __BITS4)                                               \
                      ? (__xr < ((USItype)1 << __BITS4) ? 0 : __BITS4)                             \
                      : (__xr < ((USItype)1 << 3 * __BITS4) ? 2 * __BITS4 : 3 * __BITS4);          \
        } else {                                                                                   \
            for (__a = SI_TYPE_SIZE - 8; __a > 0; __a -= 8)                                        \
                if (((__xr >> __a) & 0xff) != 0)                                                   \
                    break;                                                                         \
        }                                                                                          \
                                                                                                   \
        (count) = SI_TYPE_SIZE - (__clz_tab[__xr >> __a] + __a);                                   \
    } while (0)
#define umul_ppmm(w1, w0, u, v)                                                                    \
    __asm__("multu %2,%3"                                                                          \
            : "=l"((USItype)(w0)), "=h"((USItype)(w1))                                             \
            : "d"((USItype)(u)), "d"((USItype)(v)))
#define sub_ddmmss(sh, sl, ah, al, bh, bl)                                                         \
    do {                                                                                           \
        USItype __x;                                                                               \
        __x = (al) - (bl);                                                                         \
        (sh) = (ah) - (bh) - (__x > (al));                                                         \
        (sl) = __x;                                                                                \
    } while (0)
#define udiv_qrnnd(q, r, n1, n0, d)                                                                \
    do {                                                                                           \
        USItype __d1, __d0, __q1, __q0;                                                            \
        USItype __r1, __r0, __m;                                                                   \
        __d1 = __ll_highpart(d);                                                                   \
        __d0 = __ll_lowpart(d);                                                                    \
                                                                                                   \
        __r1 = (n1) % __d1;                                                                        \
        __q1 = (n1) / __d1;                                                                        \
        __m = (USItype)__q1 * __d0;                                                                \
        __r1 = __r1 * __ll_B | __ll_highpart(n0);                                                  \
        if (__r1 < __m) {                                                                          \
            __q1--, __r1 += (d);                                                                   \
            if (__r1 >= (d))                                                                       \
                if (__r1 < __m)                                                                    \
                    __q1--, __r1 += (d);                                                           \
        }                                                                                          \
        __r1 -= __m;                                                                               \
                                                                                                   \
        __r0 = __r1 % __d1;                                                                        \
        __q0 = __r1 / __d1;                                                                        \
        __m = (USItype)__q0 * __d0;                                                                \
        __r0 = __r0 * __ll_B | __ll_lowpart(n0);                                                   \
        if (__r0 < __m) {                                                                          \
            __q0--, __r0 += (d);                                                                   \
            if (__r0 >= (d))                                                                       \
                if (__r0 < __m)                                                                    \
                    __q0--, __r0 += (d);                                                           \
        }                                                                                          \
        __r0 -= __m;                                                                               \
                                                                                                   \
        (q) = (USItype)__q1 * __ll_B | __q0;                                                       \
        (r) = __r0;                                                                                \
    } while (0)

#include "_umoddi3_i.h"


UDItype __umoddi3(UDItype u, UDItype v)
{
    UDItype w;

    (void)__udivmoddi4(u, v, &w);

    return w;
}