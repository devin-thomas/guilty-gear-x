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
/* fvwrite.c's GETIOV macro, in a header: the build's C splitter would take a
   preprocessor chunk ending in '}' for a function. */
#define GETIOV(extra_work)                                                                         \
    while (len == 0) {                                                                             \
        extra_work;                                                                                \
        p = iov->iov_base;                                                                         \
        len = iov->iov_len;                                                                        \
        iov++;                                                                                     \
    }
