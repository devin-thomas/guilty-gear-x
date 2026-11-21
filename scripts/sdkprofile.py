"""Compile profiles for the SDK and C-runtime TUs (src/sdk/...).

The game is EE-GCC 2.9-ee-991111 -O2 -G8. The libraries linked after it were
built differently, as nathanialf/ico (MIT) established for the same compiler
generation and these byte checks confirm on both GGX discs:

- Sony's SDK archives (sdk/sce/...: libgraph, libdma, libdev, libpad, libcdvd,
  libmc, libkernl): the same cc1 at -O2 -G0 -mips3, no -g, builtins on, but
  assembled by a later GAS that fills reorder-mode delay slots. SCE's
  2.10-ee-001003-1 assembler, shipped in the ee-gcc 2.96 package, reproduces it.
- The compiler install's newlib libc/libm and libgcc (sdk/newlib/...,
  sdk/libm/..., sdk/gcc/...): -O2 -G0 -mips3 -fno-builtin. These archives were
  prebuilt and are byte-identical on both discs; the 991111 package's GAS
  reproduces them on both (KR's game assembler is not involved).

profile() returns the defaults unchanged for every other TU.
"""

import os
import re

TOOLS = os.environ.get('GGX_TOOLS', '/tools')

SCE_AS = f'{TOOLS}/ee-gcc2.96'
RUNTIME_AS = f'{TOOLS}/ee-gcc2.9-991111'


# GGX US: game TUs assembled by the SCE 2.10 GAS (ee-gcc2.96 package) instead of the game's
# dtls13010 GAS (verify.US_AS_DIR). Earlier builds needed it for 19 TUs and kept others on the 991111-01
# GAS; the dtls13010 GAS reproduces both groups (it fills delay slots and keeps FPU hazard nops), so
# only object and cp_obj remain: ObjectExecute and CpObjExecute match only with SCE 2.10.
# Every other game function was compiled with both and passes only with the dtls13010 GAS.
GAME_AS210_TUS = {'ggxeuc/object', 'ggxeuc/cp_obj'}


def profile(tu, flags, as_dir):
    """(cc1 flags, assembler package) for TU `tu` (a splat segment name)."""
    if tu in GAME_AS210_TUS and as_dir.endswith(('991111-01', '991111-01-dtls13010')):
        return flags, SCE_AS
    if tu.startswith('sdk/sce/'):
        return '-O2 -G0 -mips3', SCE_AS
    if tu.startswith(('sdk/newlib/', 'sdk/libm/', 'sdk/gcc/')):
        return '-O2 -G0 -mips3 -fno-builtin', RUNTIME_AS
    return flags, as_dir


_COND = re.compile(r'\s*#\s*(ifdef|ifndef|if|else|elif|endif)\b\s*(\w*)')


def select_version(text, target):
    """Resolve #ifdef/#ifndef VERSION_JP / VERSION_KR blocks (and their #else)
    for `target`, so a function that exists or differs on one disc only is a
    plain definition after splitting; other preprocessor lines pass through."""
    out, stack = [], []          # stack entries: None (other #if) or [keep_now, taken]
    live = lambda: all(s is None or s[0] for s in stack)
    for line in text.splitlines(keepends=True):
        m = _COND.match(line)
        kind, arg = (m.group(1), m.group(2)) if m else (None, None)
        if kind in ('ifdef', 'ifndef') and arg in ('VERSION_JP', 'VERSION_KR'):
            hit = (arg == f'VERSION_{target.upper()}') == (kind == 'ifdef')
            stack.append([hit, hit])
            out.append('\n')
            continue
        if kind in ('if', 'ifdef', 'ifndef'):
            stack.append(None)
        elif kind == 'else' and stack and stack[-1] is not None:
            stack[-1][0] = not stack[-1][1]
            out.append('\n')
            continue
        elif kind == 'endif' and stack:
            if stack.pop() is not None:
                out.append('\n')
                continue
        out.append(line if live() else '\n')
    return ''.join(out)


def source(tu, text, target):
    """The C text build.py splits for TU `tu`: SDK TUs get their version blocks resolved."""
    return select_version(text, target) if tu.startswith('sdk/') else text
