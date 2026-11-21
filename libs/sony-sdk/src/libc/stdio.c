/* This file is derived from newlib 1.8.2 libc/stdio/stdio.c, whose notice follows. */
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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/stdio.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member stdio.o */
#include "stdlib.h"
#include "string.h"
#include "reent.h"
#include "libc_internal.h"

#define __SAPP 0x0100
#define __SOFF 0x1000
#define SEEK_END 2

int __sread(void *cookie, char *buf, int n)
{
    Fil *fp = (Fil *)cookie;
    int ret;

    ret = _read_r(fp->data, fp->file, buf, n);
    if (ret >= 0) {
        fp->offset += ret;
    } else {
        fp->flags &= ~__SOFF;
    }
    return ret;
}

int __swrite(void *cookie, const char *buf, int n)
{
    Fil *fp = (Fil *)cookie;

    if (fp->flags & __SAPP) {
        (void)_lseek_r(fp->data, fp->file, 0, SEEK_END);
    }
    fp->flags &= ~__SOFF;
    return _write_r(fp->data, fp->file, buf, n);
}

long __sseek(void *cookie, long offset, int whence)
{
    Fil *fp = (Fil *)cookie;
    long ret;

    ret = _lseek_r(fp->data, fp->file, offset, whence);
    if (ret == -1L) {
        fp->flags &= ~__SOFF;
    } else {
        fp->flags |= __SOFF;
        fp->offset = ret;
    }
    return ret;
}

int __sclose(void *cookie)
{
    Fil *fp = (Fil *)cookie;

    return _close_r(fp->data, fp->file);
}