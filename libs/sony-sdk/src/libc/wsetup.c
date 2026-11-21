/* This file is derived from newlib 1.8.2 libc/stdio/wsetup.c, whose notice follows. */
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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/wsetup.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member wsetup.o */
#include "reent.h"
#include "stdlib.h"
#include "libc_internal.h"

int __swsetup(Fil *fp)
{
    do {
        if (fp->data == 0) {
            fp->data = _impure_ptr;
        }
        if (!fp->data->sdidinit) {
            __sinit(fp->data);
        }
    } while (0);

    if ((fp->flags & 8) == 0) {
        if ((fp->flags & 0x10) == 0) {
            return -1;
        }
        if (fp->flags & 4) {
            if (fp->ub.base != 0) {
                if (fp->ub.base != fp->ubuf) {
                    _free_r(fp->data, fp->ub.base);
                }
                fp->ub.base = 0;
            }
            fp->flags &= ~0x24;
            fp->r = 0;
            fp->p = fp->bf.base;
        }
        fp->flags |= 8;
    }

    if (fp->bf.base == 0) {
        __smakebuf(fp);
    }
    if (fp->flags & 1) {
        fp->w = 0;
        fp->lbfsize = -fp->bf.size;
    } else {
        fp->w = (fp->flags & 2) ? 0 : fp->bf.size;
    }
    return 0;
}