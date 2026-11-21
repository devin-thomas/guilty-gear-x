"""Per-object compile profiles for the Sony SDK tree.

Generalises the Guilty Gear X Plus build's scripts/sdkprofile.py from one
game to any build: a profile is a compiler package, cc1 flags, an assembler
package and the include groups, and the choice depends on the library and on
what the game build links.

Two families, as nathanialf/ico (MIT) established and the X Plus gate confirms:

- Sony's prebuilt SDK archives (libkernl, libgraph, libdma, libdev, libpad,
  libcdvd, libmc, ...): Sony's own build, not the game's. For SDK 2.2-2.5 that
  is cc1 2.9-ee-991111 at -O2 -G0 -mips3, no -g, builtins on, assembled by
  SCE GAS 2.10-ee-001003-1 (the as in the ee-gcc 2.96 package), which fills
  reorder-mode delay slots. Later SDKs are tried with the same profile first
  and with the 2.96 compiler second; the matcher records which one matches.
  The game's compiler does not matter here: a game built with EE-GCC 3.2
  and one built with 2.96 can link the same prebuilt archives.
- The compiler install's runtime (newlib libc/libm, libgcc): these ship with
  the compiler package, so they follow the game's toolchain. -O2 -G0 -mips3
  -fno-builtin. The 991111 runtime is assembled by its package's GAS; the 2.96
  package's runtime also has unfilled delay slots, so it takes the 991111 GAS
  too (the 2.10 GAS fills them).

A build's SCE_SDK value is the library's own version tag (PsIIlibNNNN), so
"#if SCE_SDK >= 0x2500" tests the archive actually linked; libraries of one
disc can differ (X Plus JP: libmc 2320, the rest 2300).
"""

QEMU = ('/tools/linux-runtime/usr/bin/qemu-i386-static /tools/linux-runtime/lib32/ld-linux.so.2 '
        '--library-path /tools/linux-runtime/lib32')

# compiler packages under the toolchain directory
CC = {
    '991111': dict(pkg='ee-gcc2.9-991111', ver='2.9-ee-991111', cpp='cpp', minor=9),
    '991111-01': dict(pkg='ee-gcc2.9-991111-01', ver='2.9-ee-991111-01', cpp=None, minor=9),
    '2.96': dict(pkg='ee-gcc2.96', ver='2.96-ee-001003-1', cpp='cpp0', minor=96),
    '3.2': dict(pkg='ee-gcc3.2-030926', ver='3.2-ee-030926', cpp='cpp0', minor=2, major=3),
}
AS = {
    'sce-2.10': 'ee-gcc2.96',          # GNU assembler 2.10-ee-001003-1
    '991111': 'ee-gcc2.9-991111',      # GNU assembler 2.9-ee-991111
    '2.96': 'ee-gcc2.96',
    '3.2': 'ee-gcc3.2-030926',
}

PROFILES = {
    # Sony SDK archives
    'sce-991111': dict(cc='991111', flags='-O2 -G0 -mips3', as_='sce-2.10'),
    'sce-2.96': dict(cc='2.96', flags='-O2 -G0 -mips3', as_='sce-2.10'),
    # compiler-package runtime
    'rt-991111': dict(cc='991111', flags='-O2 -G0 -mips3 -fno-builtin', as_='991111'),
    # the 2.96 package's prebuilt runtime leaves reorder-mode delay slots unfilled
    # (fabs: move v0,a0; jr ra; nop), as the 991111 GAS does, not as its own 2.10 GAS
    'rt-2.96': dict(cc='2.96', flags='-O2 -G0 -mips3 -fno-builtin', as_='991111'),
    'rt-2.96-sceas': dict(cc='2.96', flags='-O2 -G0 -mips3 -fno-builtin', as_='2.96'),
    'rt-3.2': dict(cc='3.2', flags='-O2 -G0 -fno-builtin', as_='3.2'),
}

RUNTIME = {'libc', 'libm', 'libgcc'}
INCLUDE = {'libc': ['newlib'], 'libm': ['libm', 'newlib'], 'libgcc': ['libgcc']}
# runtime variants: src/<lib>-<variant>/ holds the runtime of one compiler package
# when its sources differ wholesale from the 991111 package's (src/<lib>/)
VARIANT_INCLUDE = {
    'ee296': {'libc': ['newlib-1.9.0', 'gcc'], 'libm': ['newlib-1.9.0-libm', 'newlib-1.9.0', 'gcc'],
              'libgcc': ['libgcc']},
    'ee32': {'libc': ['newlib-1.10.0', 'gcc'], 'libm': ['newlib-1.10.0-libm', 'newlib-1.10.0', 'gcc'],
             'libgcc': ['libgcc']},
}


def runtime_variant(game_compiler):
    """The runtime source variant a game's compiler package ships (None: src/<lib>/)."""
    if '3.2' in game_compiler:
        return 'ee32'
    if '2.96' in game_compiler:
        return 'ee296'
    return None

# runtime profile by the game's compiler
def runtime_profiles(game_compiler):
    if '3.2' in game_compiler:
        return ['rt-3.2', 'rt-2.96']
    if '2.96' in game_compiler:
        return ['rt-2.96', 'rt-2.96-sceas', 'rt-991111']
    return ['rt-991111', 'rt-2.96']


def candidates(lib, lib_version, game_compiler):
    """Profiles to try for an object of `lib`, most likely first."""
    if lib in RUNTIME:
        return runtime_profiles(game_compiler)
    v = int(lib_version or '0')
    return ['sce-991111', 'sce-2.96']


def include_groups(lib, variant=None):
    if variant:
        return VARIANT_INCLUDE.get(variant, {}).get(lib, INCLUDE.get(lib, ['sce']))
    return INCLUDE.get(lib, ['sce'])


def defines(lib, lib_version, sdk_release):
    """SCE_SDK: the library's own tag, or for untagged SCE libraries (libdev,
    and libvu0 before 2.8) the build's libkernl tag."""
    d = {}
    if lib not in RUNTIME:
        d['SCE_SDK'] = '0x%s' % (lib_version or sdk_release)
    return d


def predefines(profile):
    """The ee-gcc driver's cpp predefines for this profile (CPP_PREDEFINES and the
    -mips3 / -mabi=eabi / long_max specs, from the driver binaries' strings)."""
    p = PROFILES[profile]
    cc = CC[p['cc']]
    d = ['-D__GNUC__=%d' % cc.get('major', 2), '-D__GNUC_MINOR__=%d' % cc['minor']]
    for m in ('mips', 'MIPSEL', 'R5900', '_mips', '_MIPSEL', '_R5900') + (('__ee__',) if cc['minor'] != 9 else ()):
        d.append('-D' + m)
        if not m.startswith('_'):
            d += ['-D__%s__' % m, '-D__%s' % m]
    d += ['-D__ELF__', '-D__LANGUAGE_C', '-D_LANGUAGE_C', '-DLANGUAGE_C', '-D__mips_eabi',
          '-D__LONG_MAX__=9223372036854775807L']
    if '-mips3' in p['flags']:
        d += ['-U__mips', '-D__mips=3', '-D__mips64']
    return ' '.join(d)


def cpp_flavour(profile):
    """The preprocessor a profile uses and its predefines, as a cache-key part."""
    cc = CC[PROFILES[profile]['cc']]
    return '%s/%s %s' % (cc['pkg'], cc['cpp'] or '991111-cpp', predefines(profile))


def preprocess_command(profile, src, out, incdirs, defs):
    """Shell command (run in the container) that preprocesses src to out (.i)."""
    p = PROFILES[profile]
    cc = CC[p['cc']]
    bin_ = '/tools/%s/lib/gcc-lib/ee/%s' % (cc['pkg'], cc['ver'])
    cpp = '%s/%s' % (bin_, cc['cpp']) if cc['cpp'] else '/tools/ee-gcc2.9-991111/lib/gcc-lib/ee/2.9-ee-991111/cpp'
    d = ' '.join('-D%s=%s' % kv for kv in sorted(defs.items()))
    inc = ' '.join('-I%s' % i for i in incdirs)
    return '%s %s -lang-c %s %s %s %s -o %s' % (QEMU, cpp, predefines(profile), d, inc, src, out)


def compile_command(profile, src_i, out):
    """Shell command (run in the container) that compiles a .i to the object out."""
    p = PROFILES[profile]
    cc = CC[p['cc']]
    bin_ = '/tools/%s/lib/gcc-lib/ee/%s' % (cc['pkg'], cc['ver'])
    gas = '/tools/%s/ee/bin/as -EL -mabi=eabi' % AS[p['as_']]
    return ('{q} {bin}/cc1 -quiet {flags} {i} -o {out}.s && {q} {gas} {out}.s -o {out}').format(
        q=QEMU, bin=bin_, flags=p['flags'], i=src_i, out=out, gas=gas)
