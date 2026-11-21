/* This file is derived from newlib 1.8.2 libc/stdio/fvwrite.c, whose notice follows. */
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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/fvwrite.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member fvwrite.o */
#include "string.h"
#include "reent.h"
#include "libc_internal.h"
#include "stdio.h"

struct __siov {
    const char *iov_base; /* 0x0 */
    int iov_len;          /* 0x4 */
};

struct __suio {
    struct __siov *uio_iov; /* 0x0 */
    int uio_iovcnt;         /* 0x4 */
    int uio_resid;          /* 0x8 */
};

#define __SLBF 0x0001
#define __SNBF 0x0002
#define __SWR 0x0008
#define __SSTR 0x0200
#define __SERR 0x0040
#define EOF (-1)
#define BUFSIZ 1024
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define COPY(n) (void)memmove((void *)fp->p, (void *)p, (int)(n))
#include "fvwrite_k.h"

int __sfvwrite(Fil *fp, struct __suio *uio)
{
    unsigned int len;
    const char *p;
    struct __siov *iov;
    int w, s;
    char *nl;
    int nlknown, nldist;

    if ((len = uio->uio_resid) == 0) {
        return 0;
    }
    if (((fp->flags & __SWR) == 0 || fp->bf.base == 0) && __swsetup(fp)) {
        return EOF;
    }

    iov = uio->uio_iov;
    len = 0;
    w = 0;
    if (fp->flags & __SNBF) {
        do {
            GETIOV(;);
            w = (*fp->write)(fp->cookie, p, MIN(len, BUFSIZ));
            if (w <= 0) {
                goto err;
            }
            p += w;
            len -= w;
        } while ((uio->uio_resid -= w) != 0);
    } else if ((fp->flags & __SLBF) == 0) {
        do {
            GETIOV(;);
            w = fp->w;
            if (fp->flags & __SSTR) {
                if (len < w) {
                    w = len;
                }
                COPY(w);
                fp->w -= w;
                fp->p += w;
                w = len;
            } else if (fp->p > fp->bf.base && len > w) {
                COPY(w);
                fp->p += w;
                if (fflush(fp)) {
                    goto err;
                }
            } else if (len >= (w = fp->bf.size)) {
                w = (*fp->write)(fp->cookie, p, w);
                if (w <= 0) {
                    goto err;
                }
            } else {
                w = len;
                COPY(w);
                fp->w -= w;
                fp->p += w;
            }
            p += w;
            len -= w;
        } while ((uio->uio_resid -= w) != 0);
    } else {
        nlknown = 0;
        do {
            GETIOV(nlknown = 0);
            if (!nlknown) {
                nl = memchr((void *)p, '\n', len);
                nldist = nl ? nl + 1 - p : len + 1;
                nlknown = 1;
            }
            s = MIN(len, nldist);
            w = fp->w + fp->bf.size;
            if (fp->p > fp->bf.base && s > w) {
                COPY(w);
                fp->p += w;
                if (fflush(fp)) {
                    goto err;
                }
            } else if (s >= (w = fp->bf.size)) {
                w = (*fp->write)(fp->cookie, p, w);
                if (w <= 0) {
                    goto err;
                }
            } else {
                w = s;
                COPY(w);
                fp->w -= w;
                fp->p += w;
            }
            if ((nldist -= w) == 0) {
                if (fflush(fp)) {
                    goto err;
                }
                nlknown = 0;
            }
            p += w;
            len -= w;
        } while ((uio->uio_resid -= w) != 0);
    }
    return 0;

err:
    fp->flags |= __SERR;
    return EOF;
}