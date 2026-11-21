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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libgcc/_muldi3.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libgcc.a member _muldi3.o */
#include "libgcc2.h"

typedef int SItype;

typedef unsigned int USItype;

typedef long long DItype;

struct DIstruct {
    SItype low, high;
};

typedef union {
    struct DIstruct s;
    DItype ll;
} DIunion;

#define umul_ppmm(w1, w0, u, v)                                                                    \
    __asm__("multu %2,%3"                                                                          \
            : "=l"((USItype)(w0)), "=h"((USItype)(w1))                                             \
            : "d"((USItype)(u)), "d"((USItype)(v)))
#define __umulsidi3(u, v)                                                                          \
    ({                                                                                             \
        DIunion __w;                                                                               \
        umul_ppmm(__w.s.high, __w.s.low, u, v);                                                    \
        __w.ll;                                                                                    \
    })

DItype __muldi3(DItype u, DItype v)
{
    DIunion w;
    DIunion uu, vv;

    uu.ll = u;
    vv.ll = v;

    w.ll = __umulsidi3(uu.s.low, vv.s.low);
    w.s.high += ((USItype)uu.s.low * (USItype)vv.s.high + (USItype)uu.s.high * (USItype)vv.s.low);

    return w.ll;
}