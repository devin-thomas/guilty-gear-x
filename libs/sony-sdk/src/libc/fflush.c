/* This file is derived from newlib 1.8.2 libc/stdio/fflush.c, whose notice follows. */
/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/fflush.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member fflush.o */
#include "reent.h"
#include "libc_internal.h"
#include "stdio.h"

#define __SLBF 0x0001
#define __SNBF 0x0002
#define __SWR 0x0008
#define __SERR 0x0040
#define EOF (-1)

int fflush(Fil *fp)
{
    register unsigned char *p;
    register int n, t;

    if (fp == 0) {
        return _fwalk(_impure_ptr, fflush);
    }
    /* newlib's CHECK_INIT(fp), a do-while-zero macro wrapper */
    do {
        if (fp->data == 0)
            fp->data = _impure_ptr;
        if (fp->data->sdidinit == 0)
            __sinit(fp->data);
    } while (0);
    /* newlib spells the flags word and the write count with one variable t */
    t = fp->flags;
    if ((t & __SWR) == 0) {
        return 0;
    }
    if ((p = fp->bf.base) == 0) {
        return 0;
    }
    n = fp->p - p;
    fp->p = p;
    fp->w = (t & (__SLBF | __SNBF)) ? 0 : fp->bf.size;
    for (; n > 0; n -= t, p += t) {
        if ((t = (*fp->write)(fp->cookie, (char *)p, n)) <= 0) {
            fp->flags |= __SERR;
            return EOF;
        }
    }
    return 0;
}