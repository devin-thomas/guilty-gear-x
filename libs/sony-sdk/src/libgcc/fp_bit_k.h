/* This file is derived from GCC's config/fp-bit.c (2.95 series), whose notice (GCC 2.95.3's) follows. */
/* This is a software floating point library which can be used instead of
   the floating point routines in libgcc1.c for targets without hardware
   floating point. 
 Copyright (C) 1994, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
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
/* fp_bit.c's static inline helpers, kept in a header so the build's
   per-function capsules inline them instead of emitting them. */
/* The single-float number in unpacked form.  FRAC_NBITS is 32, NGARDS is 7,
   so the implicit one sits at bit 30 and the overflow bit at bit 31.  The
   file static shares its name with the double build's global in dp-bit.o. */

static __inline__ int isnan(fp_number_type *x)
{
    return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}

static __inline__ int isinf(fp_number_type *x)
{
    return x->class == CLASS_INFINITY;
}

static __inline__ int iszero(fp_number_type *x)
{
    return x->class == CLASS_ZERO;
}

/* fp-bit.c's nan(): the NaN this build hands back for inf - inf; its static is the
   member's whole .bss, VMA 0x736188, 0x10 B. */
static __inline__ fp_number_type *nan(void)
{
    static fp_number_type thenan;

    return &thenan;
}

static __inline__ fp_number_type *_fpmul_parts(fp_number_type *a, fp_number_type *b,
                                               fp_number_type *tmp)
{
    unsigned long long prod;
    unsigned int high;
    unsigned int low;

    if (isnan(a)) {
        a->sign = a->sign != b->sign;
        return a;
    }
    if (isnan(b)) {
        b->sign = a->sign != b->sign;
        return b;
    }
    if (isinf(a)) {
        if (iszero(b))
            return nan();
        a->sign = a->sign != b->sign;
        return a;
    }
    if (isinf(b)) {
        if (iszero(a))
            return nan();
        b->sign = a->sign != b->sign;
        return b;
    }
    if (iszero(a)) {
        a->sign = a->sign != b->sign;
        return a;
    }
    if (iszero(b)) {
        b->sign = a->sign != b->sign;
        return b;
    }

    tmp->normal_exp = a->normal_exp + b->normal_exp + 2;
    tmp->sign = a->sign != b->sign;

    /* the product of the two fractions is a 64 bit number */
    prod = (unsigned long long)a->fraction * b->fraction;
    high = prod >> 32;
    low = prod;

    while (high >= IMPLICIT_2) {
        tmp->normal_exp++;
        if (high & 1) {
            low >>= 1;
            low |= IMPLICIT_2;
        }
        high >>= 1;
    }

    while (high < IMPLICIT_1) {
        high <<= 1;
        if (low & IMPLICIT_2)
            high |= 1;
        tmp->normal_exp--;
        low <<= 1;
    }

    if ((high & 0x7F) == 0x40) {
        if (high & 0x80) {
            /* exactly half way, so round to even */
            high += 0x40;
        } else if (low) {
            /* not really half way, there are more bits below */
            high += 0x40;
        }
    }

    tmp->fraction = high;
    tmp->class = CLASS_NUMBER;
    return tmp;
}

static __inline__ fp_number_type *_fpdiv_parts(fp_number_type *a, fp_number_type *b)
{
    unsigned int bit;
    unsigned int numerator;
    unsigned int denominator;
    unsigned int quotient;

    if (isnan(a))
        return a;
    if (isnan(b))
        return b;

    a->sign = a->sign ^ b->sign;

    if (isinf(a) || iszero(a)) {
        /* dividing two infinities, or two zeroes, yields a NaN */
        if (a->class == b->class)
            return nan();
        return a;
    }

    if (isinf(b)) {
        a->fraction = 0;
        a->normal_exp = 0;
        return a;
    }
    if (iszero(b)) {
        a->class = CLASS_INFINITY;
        return a;
    }

    a->normal_exp = a->normal_exp - b->normal_exp;
    numerator = a->fraction;
    denominator = b->fraction;

    if (numerator < denominator) {
        /* the quotient would be less than one */
        numerator *= 2;
        a->normal_exp--;
    }

    bit = IMPLICIT_1;
    quotient = 0;
    while (bit) {
        if (numerator >= denominator) {
            quotient |= bit;
            numerator -= denominator;
        }
        bit >>= 1;
        numerator *= 2;
    }

    if ((quotient & 0x7F) == 0x40) {
        if (quotient & 0x80) {
            /* exactly half way, so round to even */
            quotient += 0x40;
        } else if (numerator) {
            /* not really half way, there are more bits below */
            quotient += 0x40;
        }
    }

    a->fraction = quotient;
    return a;
}

