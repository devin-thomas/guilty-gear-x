# Third-party notices

This project's own code is under [CC0 1.0](LICENSE). This file lists what comes from elsewhere: code in `libs/sony-sdk/` that keeps its upstream terms, tools the build fetches, and credits.

## Code in `libs/sony-sdk/` under its own terms

`libs/sony-sdk/` rebuilds, in C, the Sony SDK libraries and the C runtime that `SLUS_202.87` links. Most of it was written from the discs and is CC0. The files below say where they come from in their first comment, and they keep their upstream terms.

### nathanialf/ico (MIT)

Files whose header reads "Adapted from nathanialf/ico" (in `libs/sony-sdk/src/`, from commit `e0881204`) are adapted from the [ICO decompilation](https://github.com/nathanialf/ico), under this licence (also in `libs/sony-sdk/LICENSE-ico`):

```
MIT License

Copyright (c) 2026 the ico decompilation contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

### newlib 1.8.2 and fdlibm

Files whose header names "newlib 1.8.2" reproduce [newlib](https://sourceware.org/newlib/) 1.8.2, the C library the EE-GCC 2.9 toolchain linked. The `libs/sony-sdk/src/libm/` files (and their `*_k.h` constant headers) come from its fdlibm-based libm, under Sun Microsystems' notice, which each file from fdlibm keeps:

```
Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.

Developed at SunPro, a Sun Microsystems, Inc. business.
Permission to use, copy, modify, and distribute this
software is freely granted, provided that this notice
is preserved.
```

The C-library files adapted from ico (`libs/sony-sdk/src/libc/`) are newlib 1.8.2 code too. Where the upstream file carries a notice, the file here begins with it, word for word:

- `fflush.c`, `findfp.c`, `fvwrite.c`, `fwalk.c`, `makebuf.c`, `sprintf.c`, `stdio.c`, `vfprintf.c`, `wsetup.c` and `fvwrite_k.h` (a macro from `fvwrite.c`) come from newlib's BSD stdio and keep the University of California's notice (Copyright (c) 1990 The Regents of the University of California). As `vfprintf.c`'s licence asks: this product includes software developed by the University of California, Berkeley and its contributors.
- `dtoa.c` and `mprec.c` keep David M. Gay's notice (Copyright (c) 1991 by AT&T).
- The others (`locale.c`, the reentrant wrappers `closer.c`, `fstatr.c`, `lseekr.c` and `sbrkr.c`, `printf.c`, the allocator files from Doug Lea's public-domain `mallocr.c`, `mbtowc_r.c`, `rand.c`) have no notice upstream.

Other newlib files keep the notice of their own upstream file.

### GCC's libgcc

`libs/sony-sdk/src/libgcc/` holds the libgcc members the disc links: `libgcc2.c` routines (`_divdi3`, `_muldi3`, ...) and the `fp-bit.c`/`dp-bit.c` soft-float code of GCC 2.9-ee. Upstream, these are GCC source files under the GNU GPL version 2 or later, with the libgcc exception that lets their compiled code be linked into any program without restriction. Those terms still apply to these files: each one begins with the Free Software Foundation's notice and the exception from its upstream file (`libgcc2.c` or `config/fp-bit.c`, as in GCC 2.95.3), and the GPL's text is in [`libs/sony-sdk/COPYING.GPL-2.0`](libs/sony-sdk/COPYING.GPL-2.0).

## Tools fetched at build time

Nothing below is stored in this repository.

| Tool | Licence | Source |
| --- | --- | --- |
| EE-GCC 2.9-ee-991111-01, its dtls13010 assembler build, 2.9-ee-991111 and 2.96 (cc1, cpp, GAS) | GNU GPL | [decompme/compilers](https://github.com/decompme/compilers), via `scripts/fetch_toolchain.py` |
| binutils for mips-ps2 | GNU GPL | [decompals/binutils-mips-ps2-decompals](https://github.com/decompals/binutils-mips-ps2-decompals) v0.10 |
| splat64, spimdisasm, rabbitizer | MIT | PyPI ([ethteck/splat](https://github.com/ethteck/splat), [Decompollaborate](https://github.com/Decompollaborate)) |
| pyelftools | Public domain | PyPI |
| PyYAML | MIT | PyPI |
| qemu-user (to run the i386 compiler on other hosts) | GNU GPL | the host's packages |

## Credits

- **Guilty Gear X Plus decompilation** ([devin-thomas/guilty-gear-x-plus](https://github.com/devin-thomas/guilty-gear-x-plus)). Guilty Gear X shares its engine with X Plus, and this project began by porting that C, its headers and its build to the US disc of X.
- **[WistfulHopes/ggx](https://github.com/WistfulHopes/ggx)**, the first decompilation project for *Guilty Gear X Plus* and the starting point of the X Plus work this project grew from. That repository has no licence, so nothing derived from its code is included here. Thank you to WistfulHopes all the same.
- **[decomp.me](https://decomp.me)** and **decompals**, for the compiler archive and the toolchain this work depends on.
- **[chaoticgd/ccc](https://github.com/chaoticgd/ccc)**, whose `stdump` read the X Plus Korean disc's STABS debug data, the source of the names and types used here.
- **[nathanialf/ico](https://github.com/nathanialf/ico)**, for the SDK and C-runtime reconstructions credited above.
- **[splat](https://github.com/ethteck/splat)** and **[spimdisasm](https://github.com/Decompollaborate/spimdisasm)**, which split your executable at build time.
