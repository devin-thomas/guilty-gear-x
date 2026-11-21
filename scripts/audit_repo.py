#!/usr/bin/env python3
"""Audit a tree for things the public repo must never contain.

Checks every file (git-tracked and untracked-but-not-ignored, or every file of
a plain directory) for:

- binaries: NUL bytes, or the magic of an ELF, ISO 9660 image, archive, image,
  audio or video file;
- assets, disc images and build products, by extension (images, audio, video,
  fonts, archives, .iso/.bin/.cue/.chd, .elf/.irx/.o/.a, ...);
- logos and artwork: any image or icon file, and files named logo/banner/boxart.
  Game screenshots are the one exception: PNG, JPEG or WebP files of at most
  2 MB under screenshots/ or docs/screenshots/ (a file there named like art is
  still a finding);
- assembly: .s/.S/.asm/.inc files, `.incbin`, INCLUDE_ASM/INCLUDE_RODATA, and
  text with 10 or more lines that look like a disassembly listing (spimdisasm's
  `/* offset vram word */` comments, objdump's `addr: word  insn`, `glabel`);
- disc data in source: a C initializer with more than 1,024 numeric literals,
  or a run of 2,000+ base64/hex characters;
- compiler dumps (gcc -da RTL, .i preprocessor output): not game data, but
  never wanted in the repository either;
- ggx lineage (LINEAGE.md): any of the nine headers of WistfulHopes/ggx, an
  #include of one, ggx's type aliases and macros (s32, u16, UNK_TYPE, ...) in
  src/ or include/ outside include/generated/, and ggx-only names
  (switchdataD_*, REG_*) in us/symbol_addrs.txt.

Writes a Markdown report. Exit status 1 if there is any finding.

  audit_repo.py [--root DIR] [--out AUDIT.md]

In the public repo, run it with no arguments.
"""
import argparse
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent

SCREENSHOT_DIRS = ('screenshots/', 'docs/screenshots/')
SCREENSHOT_EXT = {'.png', '.jpg', '.jpeg', '.webp'}
SCREENSHOT_MAX = 2 * 1024 * 1024
ELF_SIZES = {4648276, 7844099, 5035748}   # the sizes of the Guilty Gear X and X Plus executables
ASSET_EXT = {
    'image': {'.png', '.jpg', '.jpeg', '.gif', '.bmp', '.tga', '.tif', '.tiff', '.webp', '.svg', '.ico',
              '.icns', '.psd', '.tim', '.tm2', '.tex', '.dds', '.xcf', '.kra'},
    'audio': {'.wav', '.mp3', '.ogg', '.flac', '.aiff', '.vag', '.vb', '.vh', '.adpcm', '.at3', '.sq', '.hd', '.bd',
              '.mid', '.midi'},
    'video': {'.mp4', '.mov', '.avi', '.mkv', '.webm', '.pss', '.m2v', '.ipu'},
    'font': {'.ttf', '.otf', '.fnt', '.woff', '.woff2'},
    'disc image': {'.iso', '.bin', '.cue', '.chd', '.img', '.mdf', '.mds', '.nrg', '.cso', '.zso', '.ecm'},
    'executable or object': {'.elf', '.irx', '.erx', '.o', '.a', '.so', '.dll', '.exe', '.dylib', '.lib', '.obj',
                             '.pyc'},
    'archive': {'.zip', '.7z', '.rar', '.tar', '.gz', '.tgz', '.xz', '.bz2', '.zst', '.lz4'},
    'assembly': {'.s', '.asm', '.inc', '.mips'},
    'compiler dump': {'.i', '.rtl', '.jump', '.cse', '.gcse', '.loop', '.cse2', '.bp', '.flow', '.combine', '.regmove',
                      '.sched', '.lreg', '.greg', '.sched2', '.flow2', '.jump2', '.mach', '.dbr', '.addressof', '.range',
                      '.blob', '.orig', '.both', '.endlive', '.recount'},
}
MAGIC = [(0, b'\x7fELF', 'ELF binary'), (0x8001, b'CD001', 'ISO 9660 image'), (0, b'\x89PNG', 'PNG image'),
         (0, b'\xff\xd8\xff', 'JPEG image'), (0, b'GIF8', 'GIF image'), (0, b'TIM2', 'PS2 TIM2 image'),
         (0, b'RIFF', 'RIFF audio/video'), (0, b'VAGp', 'PS2 VAG audio'), (0, b'\x00\x00\x01\xba', 'MPEG/PSS video'),
         (0, b'PK\x03\x04', 'zip archive'), (0, b'\x1f\x8b', 'gzip archive'), (0, b'\xfd7zXZ', 'xz archive'),
         (0, b'7z\xbc\xaf', '7z archive'), (0, b'OggS', 'Ogg audio'), (0, b'ID3', 'MP3 audio')]
NAMED_ART = re.compile(r'(logo|banner|boxart|box-art|splash|artwork|cover-?art|key-?art)', re.I)
LISTING = re.compile(r'/\*\s*[0-9A-Fa-f]{4,8}\s+[0-9A-Fa-f]{8}\s+[0-9A-Fa-f]{8}\s*\*/'
                     r'|^\s*[0-9a-fA-F]{6,8}:\s+[0-9a-fA-F]{8}\s+\w'
                     r'|^\s*glabel\s', re.M)
ASM_MACRO = re.compile(r'\bINCLUDE_(ASM|RODATA)\s*\(|^\s*\.incbin\b', re.M)
BLOB = re.compile(r'[A-Za-z0-9+/=]{2000,}|(?:\\x[0-9a-fA-F]{2}){1000,}')
INIT = re.compile(r'=\s*\{')
NUM = re.compile(r'\b(?:0x[0-9a-fA-F]+|\d+)\b')
MAX_LITERALS = 1024
# what LINEAGE.md removed from WistfulHopes/ggx, which has no licence
GGX_HEADERS = ('common.h', 'include_asm.h', 'ggxeuc/act.h', 'ggxeuc/actsub.h', 'ggxeuc/effect.h',
               'ggxeuc/gcl.h', 'ggxeuc/js_act.h', 'ggxeuc/objwork.h', 'ggxeuc/player.h')
GGX_INCLUDE = re.compile(r'^\s*#\s*include\s*[<"](' + '|'.join(map(re.escape, GGX_HEADERS)) + r')[>"]', re.M)
GGX_ALIAS = re.compile(r'\b(s8|s16|s32|s64|u8|u16|u32|u64|f32|UNK_TYPE|UNK_PTR|UNK_RET|UNK_FUN_ARG|UNK_FUN_PTR'
                       r'|UNK_ARGS|ARRAYSIZEU?|CLEAR|INCLUDE_ASM|INCLUDE_RODATA)\b')
GGX_NAME = re.compile(r'^\s*(?:switchdataD_|REG_)\w*\s*=', re.M)
COMMENT = re.compile(r'/\*.*?\*/|//[^\n]*', re.S)


def list_files(root):
    try:
        r = subprocess.run(['git', 'ls-files', '-z', '--cached', '--others', '--exclude-standard', '.'],
                           cwd=root, capture_output=True, check=True)
        if not (root / '.git').exists():
            raise FileNotFoundError        # a plain directory inside some other checkout
        files = sorted({p for p in r.stdout.decode().split('\0') if p and (root / p).is_file()})
        return files, 'git (tracked and untracked, not ignored)'
    except (subprocess.CalledProcessError, FileNotFoundError):
        return sorted(str(p.relative_to(root)).replace('\\', '/') for p in root.rglob('*') if p.is_file()
                      and '.git' not in p.relative_to(root).parts), 'directory walk'


def largest_initializer(text):
    best = 0
    for m in INIT.finditer(text):
        i, depth = m.end(), 1
        while depth and i < len(text):
            depth += {'{': 1, '}': -1}.get(text[i], 0)
            i += 1
        best = max(best, len(NUM.findall(text[m.end():i])))
    return best


def screenshot(rel, data):
    """True for an allowed game screenshot: PNG, JPEG or WebP, at most 2 MB, in a screenshot folder."""
    if not rel.startswith(SCREENSHOT_DIRS) or Path(rel).suffix.lower() not in SCREENSHOT_EXT:
        return False
    if len(data) > SCREENSHOT_MAX or NAMED_ART.search(Path(rel).name) or len(data) in ELF_SIZES:
        return False
    magic = {'.png': b'\x89PNG\r\n\x1a\n', '.jpg': b'\xff\xd8\xff', '.jpeg': b'\xff\xd8\xff', '.webp': b'RIFF'}
    ext = Path(rel).suffix.lower()
    return data.startswith(magic[ext]) and (ext != '.webp' or data[8:12] == b'WEBP')


def audit_file(path, rel):
    found = []
    ext = Path(rel).suffix.lower()
    data = path.read_bytes()
    if screenshot(rel, data):
        return found
    for kind, exts in ASSET_EXT.items():
        if ext in exts:
            found.append((kind, f'extension {ext}'))
    if NAMED_ART.search(Path(rel).name):
        found.append(('logo or artwork', 'file name'))
    for off, magic, what in MAGIC:
        if data[off:off + len(magic)] == magic:
            found.append(('binary', what))
    if b'\0' in data[:65536]:
        found.append(('binary', 'NUL bytes'))
    if any(k == 'binary' for k, _ in found):
        return found
    text = data.decode('utf-8', errors='replace')
    n = len(LISTING.findall(text))
    if n >= 10:
        found.append(('assembly', f'{n} disassembly-listing lines'))
    if ASM_MACRO.search(text) and ext in ('.c', '.h', '.s', '.inc'):
        found.append(('assembly', 'INCLUDE_ASM/INCLUDE_RODATA/.incbin'))
    if BLOB.search(text):
        found.append(('disc data', 'long base64/hex run'))
    if ext in ('.c', '.h'):
        n = largest_initializer(text)
        if n > MAX_LITERALS:
            found.append(('disc data', f'initializer with {n} literals'))
    return found


def lineage_findings(root, files):
    """(path, why) for anything LINEAGE.md removed from ggx that is back in the tree."""
    found = [(f'include/{h}', 'ggx header') for h in GGX_HEADERS if (root / 'include' / h).exists()]
    for rel in files:
        if not rel.startswith(('src/', 'include/')) or rel.startswith('include/generated/')                 or not rel.endswith(('.c', '.h')):
            continue
        text = (root / rel).read_text(encoding='utf-8', errors='replace')
        if GGX_INCLUDE.search(text):
            found.append((rel, 'includes a ggx header'))
        for m in sorted(set(GGX_ALIAS.findall(COMMENT.sub(' ', text)))):
            found.append((rel, f'ggx alias or macro {m}'))
    sym = root / 'us/symbol_addrs.txt'
    names = GGX_NAME.findall(sym.read_text(encoding='utf-8')) if sym.exists() else []
    if names:
        found.append(('us/symbol_addrs.txt', f'{len(names)} ggx-only names'))
    return found


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', default=str(HERE))
    ap.add_argument('--out', help='Markdown report')
    a = ap.parse_args()

    root = Path(a.root).resolve()
    findings, counts = defaultdict(list), defaultdict(int)
    files, method = list_files(root)
    for rel in files:
        counts[Path(rel).suffix.lower() or '(none)'] += 1
        for kind, why in audit_file(root / rel, rel):
            findings[kind].append((rel, why))
    for rel, why in lineage_findings(root, files):
        findings['ggx lineage'].append((rel, why))

    lines = ['# Repo audit', '',
             f'Generated by `scripts/audit_repo.py`. Scanned {len(files):,} files ({method}).', '',
             '| Check | Findings |', '| --- | ---: |']
    kinds = ['binary', 'image', 'audio', 'video', 'font', 'disc image', 'executable or object', 'archive',
             'logo or artwork', 'assembly', 'disc data', 'compiler dump', 'ggx lineage']
    found = sum(len(findings.get(k, [])) for k in kinds)
    for k in kinds:
        lines.append(f'| {k} | {len(findings.get(k, []))} |')
    lines += ['', '**Result: ' + ('clean.' if not found else f'{found} findings.') + '**', '']
    for k in kinds:
        if findings.get(k):
            lines += [f'## {k}', '']
            for rel, why in findings[k][:200]:
                lines.append(f'- `{rel}`: {why}')
            if len(findings[k]) > 200:
                lines.append(f'- ... and {len(findings[k]) - 200} more')
            lines.append('')
    lines += ['## File types scanned', '',
              ', '.join(f'{e} {n:,}' for e, n in sorted(counts.items(), key=lambda x: -x[1])), '']
    report = '\n'.join(lines)
    if a.out:
        Path(a.out).write_text(report, encoding='utf-8', newline='\n')
    print(report)
    return 1 if found else 0


if __name__ == '__main__':
    sys.exit(main())
