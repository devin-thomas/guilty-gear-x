/* This file is derived from newlib 1.8.2 libc/stdio/makebuf.c, whose notice follows. */
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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/makebuf.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member makebuf.o */
#include "reent.h"
#include "stat.h"
#include "libc_internal.h"
#include "stdlib.h"
#include "unistd.h"

/* Only st_mode is reached here, at 0x04; the record is 0x70 bytes, the size
   of the stat buffer in this member's frame.  The trailing bytes are
   padding, not a known field layout. */
#define __SLBF 0x0001
#define __SNBF 0x0002
#define __SOPT 0x0400
#define __SNPT 0x0800
#define __SMOD 0x0080
#define BUFSIZ 1024

void __smakebuf(Fil *fp)
{
    unsigned int size;
    int couldbetty;
    void *p;
    struct stat st;

    if (fp->flags & __SNBF) {
        fp->bf.base = fp->p = fp->nbuf;
        fp->bf.size = 1;
        return;
    }
    if (fp->file < 0 || _fstat_r(fp->data, fp->file, &st) < 0) {
        couldbetty = 0;
        size = BUFSIZ;
        fp->flags |= __SNPT;
    } else {
        couldbetty = (st.st_mode & S_IFMT) == S_IFCHR;
        size = BUFSIZ;
        if ((st.st_mode & S_IFMT) == S_IFREG && fp->seek == __sseek) {
            fp->blksize = size;
            fp->flags |= __SOPT;
        } else {
            fp->flags |= __SNPT;
        }
    }
    if ((p = _malloc_r(fp->data, size)) == 0) {
        fp->flags |= __SNBF;
        fp->bf.base = fp->p = fp->nbuf;
        fp->bf.size = 1;
        return;
    }
    fp->data->cleanup = (void *)_cleanup_r;
    fp->flags |= __SMOD;
    fp->bf.base = fp->p = (unsigned char *)p;
    fp->bf.size = size;
    if (couldbetty && isatty(fp->file)) {
        fp->flags |= __SLBF;
    }
}