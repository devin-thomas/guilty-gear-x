#!/usr/bin/env python3
"""Fetch the PS2 toolchain the matching build needs. It is never vendored.

Downloads, unpacks and checks, from decomp.me's compiler archive
(github.com/decompme/compilers, release `compilers`):
- ee-gcc2.9-991111-01: cc1 and cpp 2.9-ee-991111-01, the game's compiler;
- ee-gcc2.9-991111-01-dtls13010: its GAS ("2.9-ee-991111", a later build than the
  991111-01 package's own), the assembler for every game file except two;
- ee-gcc2.96: GAS 2.10-ee-001003-1, for the object and cp_obj game files and the
  Sony SDK files;
- ee-gcc2.9-991111: cc1, cpp and GAS 2.9-ee-991111, for the Sony SDK and C runtime;
and decompals' binutils for mips-ps2 (github.com/decompals/binutils-mips-ps2-decompals), v0.10.

Every archive is checked by SHA-256 (GitHub's published digest for binutils; for EE-GCC,
which has none, the digest of the archives the published verification build used). Each
EE-GCC package is then checked by the SHA-256 of the files the build runs. A mismatch stops
the script and leaves nothing in place.

EE-GCC is a Linux i386 program. On x86-64 Linux with the 32-bit C library it runs
directly (GGX_QEMU=''); elsewhere it runs under qemu-i386 (set GGX_QEMU to that command).
The dtls13010 assembler brings its own loader and C library and needs no prefix.

  fetch_toolchain.py [--dest tools] [--host linux-x86-64|macos-arm64|windows-x86-64] [--dry-run]
"""
import argparse
import hashlib
import io
import platform
import shutil
import sys
import tarfile
import tempfile
import urllib.request
import zipfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
COMPILERS = 'https://github.com/decompme/compilers/releases/download/compilers/'
BINUTILS = 'https://github.com/decompals/binutils-mips-ps2-decompals/releases/download/v0.10/'

# package dir -> (archive, archive sha256, {path in package: sha256})
EE_GCC = {
    'ee-gcc2.9-991111-01': ('ee-gcc2.9-991111-01.tar.xz',
                            'ed684fd98f89d36b0121caab311052089103e3b36241fcef4338cc9ea41c75b8', {
        'lib/gcc-lib/ee/2.9-ee-991111-01/cc1': 'b9aef69f93efb949f15ea58189e8eef4a002b9fe4de5d3fcf89c34e1244ec026',
        'lib/gcc-lib/ee/2.9-ee-991111-01/cpp': 'f85a54d241e019993fa7a06285d3677856b3b431e10318a587ab029373c603d7',
    }),
    'ee-gcc2.9-991111-01-dtls13010': ('ee-gcc2.9-991111-01-dtls13010.tar.gz',
                                      '3ab52a7ea962100f2a7df3e04d3e7a758aad669eb365251682e769b755d9d586', {
        'ee/bin/as': '8690a5090a0c8f2ccb8e6ebe9d60131ed1f2214ab22962758a90fc4abcbeebbf',
        'ee/bin/as.bin': 'cb909465e826106e8ad11f11bb201476a0b64a5012829cd2a440ca3eaf732a03',
    }),
    'ee-gcc2.96': ('ee-gcc2.96.tar.xz',
                   '0590d2ca9da8f5903889d66761220d14b47a8d14ba987ca53db84a1650a1fd0a', {
        'ee/bin/as': 'b8cfdb6ecb6931642914020f92a62e653378f57230eed4895f69afa050d47b5e',
    }),
    'ee-gcc2.9-991111': ('ee-gcc2.9-991111.tar.xz',
                         '99cb5db603cd985b50fc6851157020ad69489f2227309ccf825bd43ac7bc19dd', {
        'lib/gcc-lib/ee/2.9-ee-991111/cc1': '548c962776e6d01ea0e83a08dc8ea99b87eb7b77bcfd2350d5bce7dc9395398d',
        'lib/gcc-lib/ee/2.9-ee-991111/cpp': '260a2ae5d55d223539be9f4a09a9890a1bc12bcdccf50e015d95e581089df499',
        'ee/bin/as': '82e98546816d6d97ede5cda29deff86bb0c93b9fa456dd6a50520475bee39ff3',
    }),
}
BINUTILS_ARCHIVES = {
    'linux-x86-64': ('binutils-mips-ps2-decompals-linux-x86-64.tar.gz',
                     '9fe31ea3ee1a37536f9f0e2e12c668e7c3cb99e59f5154bd2f1fc4762473094c'),
    'macos-arm64': ('binutils-mips-ps2-decompals-macos-arm64.tar.gz',
                    '948cc736e01b2c135168caabfb616f6ed8efe89f1dad1ed8ef1ca74acb1b742c'),
    'windows-x86-64': ('binutils-mips-ps2-decompals-windows-x86-64.zip',
                       '2bb5904ee174e04f1600cc70ffdb3dc36b0efa4d1892ea446be588ba8dc6198c'),
}


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def default_host():
    m = platform.machine().lower()
    if sys.platform == 'darwin':
        return 'macos-arm64'
    if sys.platform == 'win32':
        return 'windows-x86-64'
    return 'linux-x86-64' if m in ('x86_64', 'amd64') else None


def download(url):
    print(f'  GET {url}')
    with urllib.request.urlopen(url) as r:
        return r.read()


def unpack(data, name, dest):
    """Unpack an archive into dest, refusing paths outside it. A symbolic link is allowed only
    as a relative link to a file in its own directory (the dtls13010 package's libc.so.6)."""
    dest.mkdir(parents=True)
    if name.endswith('.zip'):
        with zipfile.ZipFile(io.BytesIO(data)) as z:
            for m in z.infolist():
                target = (dest / m.filename).resolve()
                if not target.is_relative_to(dest.resolve()):
                    raise SystemExit(f'{name}: unsafe path {m.filename}')
            z.extractall(dest)
        return
    with tarfile.open(fileobj=io.BytesIO(data)) as t:
        members = t.getmembers()
        for m in members:
            if not (dest / m.name).resolve().is_relative_to(dest.resolve()) or m.islnk():
                raise SystemExit(f'{name}: unsafe member {m.name}')
            if m.issym() and ('/' in m.linkname or m.linkname in ('', '.', '..')):
                raise SystemExit(f'{name}: unsafe link {m.name} -> {m.linkname}')
        t.extractall(dest, members=[m for m in members if not m.issym()], filter='data')
        for m in members:
            if m.issym():
                link = dest / m.name
                target = link.parent / m.linkname
                if not target.is_file():
                    raise SystemExit(f'{name}: link {m.name} -> {m.linkname} has no target')
                shutil.copy2(target, link)      # a copy, so the tree holds no links


def package_root(tmp, expect):
    """The directory that holds `expect`: the unpack dir itself or its single subdirectory."""
    if (tmp / expect).exists():
        return tmp
    subs = [p for p in tmp.iterdir() if p.is_dir()]
    if len(subs) == 1 and (subs[0] / expect).exists():
        return subs[0]
    raise SystemExit(f'{tmp.name}: {expect} not found in the archive')


def check_files(root, files, label):
    for rel, want in files.items():
        got = sha256((root / rel).read_bytes())
        if got != want:
            raise SystemExit(f'{label}: {rel} has SHA-256 {got}, expected {want}')


def executable(root):
    """tar extraction keeps modes; make sure the programs the build runs can run."""
    for p in [*root.glob('ee/bin/*'), *root.glob('lib/gcc-lib/ee/*/*'), *root.glob('lib/ld-*.so')]:
        if p.is_file():
            p.chmod(p.stat().st_mode | 0o755)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--dest', default=str(HERE.parent / 'tools'), help='install directory (default: ./tools)')
    ap.add_argument('--host', choices=sorted(BINUTILS_ARCHIVES), default=default_host(),
                    help='binutils build to fetch')
    ap.add_argument('--no-binutils', action='store_true')
    ap.add_argument('--dry-run', action='store_true', help='print what would be fetched, download nothing')
    a = ap.parse_args()
    dest = Path(a.dest).resolve()

    plan = [(pkg, COMPILERS + archive, digest, files) for pkg, (archive, digest, files) in EE_GCC.items()]
    if not a.no_binutils:
        if not a.host:
            raise SystemExit('no binutils build for this host; pass --host or --no-binutils')
        archive, digest = BINUTILS_ARCHIVES[a.host]
        plan.append(('binutils', BINUTILS + archive, digest, {}))

    for pkg, url, digest, _ in plan:
        state = 'present' if (dest / pkg).exists() else 'fetch'
        print(f'{pkg:32} {state:8} {url}')
    if a.dry_run:
        return 0

    dest.mkdir(parents=True, exist_ok=True)
    for pkg, url, digest, files in plan:
        final = dest / pkg
        if final.exists():
            if files:
                check_files(final, files, pkg)
            print(f'{pkg}: present')
            continue
        data = download(url)
        if digest and sha256(data) != digest:
            raise SystemExit(f'{pkg}: archive SHA-256 {sha256(data)}, expected {digest}')
        with tempfile.TemporaryDirectory(dir=dest) as tmp:
            tmp = Path(tmp) / 'unpack'
            unpack(data, url.rsplit('/', 1)[1], tmp)
            root = package_root(tmp, next(iter(files), '.'))
            check_files(root, files, pkg)
            executable(root)
            shutil.move(str(root), final)
        print(f'{pkg}: ok (archive SHA-256 {sha256(data)})')
    print(f'toolchain ready in {dest}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
