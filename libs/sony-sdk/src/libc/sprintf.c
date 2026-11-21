/* This file is derived from newlib 1.8.2 libc/stdio/sprintf.c, whose notice follows. */
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
/* Adapted from nathanialf/ico (MIT; e0881204, sce/libc/stdio/sprintf.c): ee-gcc 2.9's newlib/libgcc as linked by Guilty Gear X. */
/* libc.a member sprintf.o */
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "reent.h"

int _sprintf_r(void *ptr, int str, int fmt, ...)
{
    char buf[0x60];
    char *va = (char *)__builtin_next_arg(fmt) - 40;
    int n;
    *(int *)(buf + 0x0) = str;
    *(int *)(buf + 0x8) = 0x7FFFFFFF;
    *(short *)(buf + 0xC) = 0x208;
    *(int *)(buf + 0x10) = str;
    *(int *)(buf + 0x14) = 0x7FFFFFFF;
    *(int *)(buf + 0x54) = (int)ptr;
    n = vfprintf(buf, fmt, va);
    *(char *)(*(int *)(buf + 0x0)) = 0;
    return n;
}

int sprintf(char *str, const char *fmt, ...)
{
    char buf[0x60];
    char *va = (char *)__builtin_next_arg(fmt) - 48;
    int n;
    *(int *)(buf + 0x0) = (int)str;
    *(int *)(buf + 0x8) = 0x7FFFFFFF;
    *(short *)(buf + 0xC) = 0x208;
    *(int *)(buf + 0x10) = (int)str;
    *(int *)(buf + 0x14) = 0x7FFFFFFF;
    *(int *)(buf + 0x54) = (int)_impure_ptr;
    n = vfprintf(buf, fmt, va);
    *(char *)(*(int *)(buf + 0x0)) = 0;
    return n;
}