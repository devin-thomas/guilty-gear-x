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
/* libgcc.a member _floatdisf.o: gcc 2.95's libgcc2.c L_floatdisf (GPL with the
   libgcc runtime exception), spelled like the _floatdidf.c adapted from
   nathanialf/ico (MIT). */
#include "libgcc2.h"

typedef int SItype;

typedef unsigned int USItype;

typedef long long DItype;

typedef unsigned long long UDItype;

typedef double DFtype;

typedef float SFtype;

#define WORD_SIZE (sizeof(SItype) * 8)
#define HIGH_HALFWORD_COEFF (((UDItype)1) << (WORD_SIZE / 2))
#define HIGH_WORD_COEFF (((UDItype)1) << WORD_SIZE)
#define DI_SIZE (sizeof(DItype) * 8)
#define DF_SIZE 53
#define SF_SIZE 24

SFtype __floatdisf(DItype u)
{
    /* Do the calculation in DFmode so that we don't lose any of the
       precision of the high word while multiplying it. */
    DFtype f;

    /* Protect against double-rounding error. */
    if (DF_SIZE < DI_SIZE && DF_SIZE > (DI_SIZE - DF_SIZE + SF_SIZE)) {
#define REP_BIT ((UDItype)1 << (DI_SIZE - DF_SIZE))
        if (!(-((DItype)1 << DF_SIZE) < u && u < ((DItype)1 << DF_SIZE))) {
            if ((USItype)u & (REP_BIT - 1)) {
                u |= REP_BIT;
            }
        }
    }
    f = (SItype)(u >> WORD_SIZE);
    f *= HIGH_HALFWORD_COEFF;
    f *= HIGH_HALFWORD_COEFF;
    f += (USItype)(u & (HIGH_WORD_COEFF - 1));

    return (SFtype)f;
}