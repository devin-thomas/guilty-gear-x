/* This file is derived from newlib 1.8.2 libc/stdio/fwalk.c, whose notice follows. */
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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/fwalk.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member fwalk.o */
#include "reent.h"
#include "libc_internal.h"

int _fwalk(Reent *ptr, int (*function)())
{
    Fil *fp;
    int n, ret = 0;
    Glue *g;

    for (g = &ptr->glue; g != 0; g = g->next)
        for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
            if (fp->flags != 0)
                ret |= function(fp);

    return ret;
}