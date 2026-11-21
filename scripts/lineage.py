#!/usr/bin/env python3
"""Lineage check of the Guilty Gear X US tree against a WistfulHopes/ggx checkout.

ggx has no licence, so the public release may keep nothing derived from its code.
This is the Guilty Gear X Plus decompilation's check, adapted to this
tree. It finds what the tree still shares with a ggx checkout (85b1242 was
audited) and explains each overlap or reports it as unexplained:

1. Headers: none of ggx's include/ paths may exist here.
2. Functions: every C function body in ggx's src/ against the definition of the
   same name here (or its US counterpart, US_NAMES), compared as token sequences
   (whitespace and comments ignored, ggx's s32/u16/... aliases mapped to native
   C). Bodies with at most two tokens (`{ }`) and one-statement bodies whose
   header is the .mdebug signature (include/generated/<tu>.h) are dictated by
   the binary. CLEANROOM functions are the independent rewrites (LINEAGE.md).
   Any other identical or near-identical body is unexplained.
3. Aliases: no ggx type alias or macro (s32, u16, UNK_TYPE, INCLUDE_ASM, ...)
   may appear in src/ or include/ outside the generated tree.
4. Shared text: every tracked text file here against every ggx text file, as
   4-line shingles of normalised non-trivial lines. A shared shingle is
   explained when every line is a symbol-list entry or a splat segment line,
   when it is in the .mdebug output (include/generated/) or is a struct whose
   fields are all .mdebug field names, or when it lies inside a CLEANROOM
   function. Anything else is unexplained.
5. Names (needs --kr-elf): every us/symbol_addrs.txt name is a Korean-disc
   .symtab name, a splat-style auto name (func_/D_/jtbl_), or listed as ours.

Usage:
  lineage.py --upstream <ggx checkout> [--kr-elf SLPM_645.25] [--json out.json]
Exit status 1 if anything is unexplained.
"""
import argparse
import difflib
import json
import os
import re
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

ALIASES = {'s8': 'char', 's16': 'short', 's32': 'int', 's64': 'long',
           'u8': 'unsigned char', 'u16': 'unsigned short', 'u32': 'unsigned int',
           'u64': 'unsigned long', 'f32': 'float'}
GGX_MACROS = ('UNK_TYPE', 'UNK_PTR', 'UNK_RET', 'UNK_FUN_ARG', 'UNK_FUN_PTR', 'UNK_ARGS',
              'ARRAYSIZE', 'ARRAYSIZEU', 'CLEAR', 'INCLUDE_ASM', 'INCLUDE_RODATA')
TOKEN = re.compile(r'0[xX][0-9a-fA-F]+|\d+\.\d*f?|\d+|[A-Za-z_]\w*|->|\+\+|--|<<=|>>=|[<>=!+\-*/%&|^]=|&&|\|\||<<|>>|\S')
TEXT_EXT = {'.c', '.h', '.py', '.txt', '.yaml', '.yml', '.md', '.inc', '.s', '.sh', '.toml', '.json', '.csv', '.ld'}
SKIP_UPSTREAM = {'kappa-db.json'}   # ggx's reference disassembly; never shingled (see LINEAGE.md)
TRIVIAL = re.compile(r'^([{}();,]*|#endif.*|#else|break;|return;|default:|\*/|/\*|//.*|else|else \{|\} else \{'
                     r'|#include .*|import [\w., ]+|from [\w.]+ import [\w., ()*]+)$')
SYMBOL_LINE = re.compile(r'^\S+ = 0x[0-9a-fA-F]+;( // type:\w+.*)?$')
SEGMENT_LINE = re.compile(r'^- \[0x[0-9a-f]+, (c|asm|bin|data|rodata|bss|\.?\w+), (ggxeuc|sdk)/\S+\]$')
MDEBUG_OUTPUT = ('include/generated/',)
# Written independently of ggx (LINEAGE.md). The first six are the X Plus clean-room
# rewrites (2026-10-25), carried to the US build with their verified bodies.
CLEANROOM = {'af_ASTFLAGCONTROL': 'src/ggxeuc/act.c', 'af_BACK_MOTION': 'src/ggxeuc/act.c',
             'af_AFTERIMAGE': 'src/ggxeuc/act.c', 'SetAnimeNo': 'src/ggxeuc/act.c',
             'ObjectCreate_ActNo': 'src/ggxeuc/object.c', 'func_001581D0': 'src/ggxeuc/chase.c'}
# ggx function name -> its US counterpart's name in this tree (the US address has no .symtab name)
US_NAMES = {'ChaseInit': 'func_001581D0'}
AUTO_NAME = re.compile(r'^(func|D|jtbl|jumptable|switchD|L)_[0-9A-Fa-f]{6,8}(_\w+)?$')
STRUCT_OPEN = re.compile(r'^(typedef )?(struct|union) \w* ?\{$')
FIELD = re.compile(r'^[\w ]+?\*? ?(\w+)(\[[^\]]*\])*;( /\* \+0x[0-9a-f]+ \*/)?$')
# Paths of research material outside this repository (none here).
RESEARCH_ONLY = ()


def strip_comments(src):
    src = re.sub(r'/\*.*?\*/', ' ', src, flags=re.S)
    return re.sub(r'//[^\n]*', ' ', src)


def c_functions(path):
    """Return {name: definition text} for every function definition in a C file."""
    src = strip_comments(open(path, encoding='utf-8', errors='replace').read())
    out = {}
    for m in re.finditer(r'^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\(([^;{}]*)\)\s*\{', src, re.M):
        name = m.group(1)
        if name in ('if', 'while', 'for', 'switch', 'return', 'sizeof'):
            continue
        depth, i = 0, m.end() - 1
        while i < len(src):
            if src[i] == '{':
                depth += 1
            elif src[i] == '}':
                depth -= 1
                if depth == 0:
                    break
            i += 1
        out[name] = src[m.start():i + 1]
    return out


def tokens(text):
    out = []
    for t in TOKEN.findall(text):
        out.extend(ALIASES.get(t, t).split())
    return out


def walk(root, sub, ext):
    for d, dirs, files in os.walk(os.path.join(root, sub)):
        dirs[:] = sorted(x for x in dirs if x != '.git')
        for f in sorted(files):
            if f.endswith(ext):
                yield os.path.join(d, f)


def signatures():
    """{name: signature} from the generated per-TU headers (the .mdebug function records)."""
    out = {}
    for p in walk(HERE, 'include/generated', '.h'):
        if os.sep + 'types' + os.sep in p or os.sep + 'forward' + os.sep in p:
            continue
        for line in open(p, encoding='utf-8'):
            m = re.match(r'^([A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;]*\));', line)
            if m:
                out.setdefault(m.group(2), m.group(1))
    return out


def squash(s):
    return re.sub(r'\s+', '', s)


def compare_functions(up):
    ups, ours = {}, {}
    for p in walk(up, 'src', '.c'):
        for name, text in c_functions(p).items():
            ups[name] = (os.path.relpath(p, up).replace('\\', '/'), text)
    for p in walk(HERE, 'src', '.c'):
        for name, text in c_functions(p).items():
            ours.setdefault(name, (os.path.relpath(p, HERE).replace('\\', '/'), text))
    sigs = signatures()
    rows = []
    for name, (upath, utext) in sorted(ups.items(), key=lambda kv: kv[1][0] + kv[0]):
        ut = tokens(utext[utext.index('{'):])
        here = US_NAMES.get(name, name)
        row = {'name': name, 'upstream': upath, 'upstream_tokens': len(ut)}
        if here != name:
            row['us_name'] = here
        if here not in ours:
            row.update(ours=None, status='absent here')
            rows.append(row)
            continue
        opath, otext = ours[here]
        ot = tokens(otext[otext.index('{'):])
        sm = difflib.SequenceMatcher(None, ut, ot, autojunk=False)
        header_is_sig = squash(otext[:otext.index('{')]) == squash(sigs.get(here, '#'))
        row.update(ours=opath, ours_tokens=len(ot), similarity=round(sm.ratio(), 3),
                   longest_shared_run=max((b.size for b in sm.get_matching_blocks()), default=0),
                   header_is_mdebug_signature=header_is_sig)
        if here in CLEANROOM and ut != ot:
            row['status'] = 'independent rewrite'
        elif len(ot) <= 2 and header_is_sig:
            row['status'] = 'dictated: empty body, .mdebug signature'
        elif ot.count(';') == 1 and header_is_sig and len(ot) <= 6:
            row['status'] = 'dictated: one statement, .mdebug signature'
        elif ut == ot:
            row['status'] = 'IDENTICAL TOKENS'
        elif sm.ratio() >= 0.8 or row['longest_shared_run'] >= 20:
            row['status'] = 'SIMILAR'
        else:
            row['status'] = 'independent'
        rows.append(row)
    return rows


def check_headers(up):
    theirs = {os.path.relpath(p, up).replace('\\', '/') for p in walk(up, 'include', '.h')}
    return sorted(h for h in theirs if os.path.exists(os.path.join(HERE, h)))


def check_aliases(files):
    pat = re.compile(r'\b(' + '|'.join(list(ALIASES) + list(GGX_MACROS)) + r')\b')
    hits = []
    for rel in files:
        if not rel.startswith(('src/', 'include/')) or rel.startswith(MDEBUG_OUTPUT) \
                or not rel.endswith(('.c', '.h')):
            continue
        text = strip_comments(open(os.path.join(HERE, rel), encoding='utf-8', errors='replace').read())
        for m in pat.finditer(text):
            hits.append(f'{rel}: {m.group(1)}')
    return hits


def field_names():
    names = set()
    for p in walk(HERE, 'include/generated/types', '.h'):
        for line in open(p, encoding='utf-8'):
            m = FIELD.match(line.strip())
            if m:
                names.add(m.group(1))
    return names


def stabs_struct(lines, fields):
    if not STRUCT_OPEN.match(lines[0]) and not FIELD.match(lines[0]):
        return False
    for l in lines[1:] if STRUCT_OPEN.match(lines[0]) else lines:
        m = FIELD.match(l)
        if not m or not (m.group(1) in fields or m.group(1).startswith('pad')):
            return False
    return True


def raw_function(path, name):
    """The definition of `name` as it stands in the file, comments included."""
    src = open(path, encoding='utf-8', errors='replace').read()
    m = re.search(r'^[A-Za-z_][^;{}\n]*\b' + name + r'\s*\([^;{}]*\)\s*\{', src, re.M)
    depth, i = 0, m.end() - 1
    while True:
        depth += {'{': 1, '}': -1}.get(src[i], 0)
        if depth == 0:
            return src[m.start():i + 1]
        i += 1


def norm_line(l):
    l = re.sub(r'\s+', ' ', l.strip())
    return '' if TRIVIAL.match(l) else l


def shingle_set(lines, k=4):
    norm = [n for n in (norm_line(l) for l in lines) if n]
    return {'\n'.join(norm[i:i + k]) for i in range(len(norm) - k + 1)}


def shingles(path):
    try:
        return shingle_set(open(path, encoding='utf-8', errors='replace').read().splitlines())
    except OSError:
        return set()


def cleanroom_shingles():
    out = set()
    for name, rel in CLEANROOM.items():
        out |= shingle_set(raw_function(os.path.join(HERE, rel), name).splitlines())
    return out


def tracked_files():
    r = subprocess.run(['git', 'ls-files', '-z', '.'], cwd=HERE, capture_output=True, check=True)
    return [p for p in r.stdout.decode().split('\0') if p]


def compare_text(up, files):
    index = {}
    for root, dirs, fs in os.walk(up):
        dirs[:] = [d for d in dirs if d != '.git']
        for f in fs:
            if f in SKIP_UPSTREAM or os.path.splitext(f)[1] not in TEXT_EXT:
                continue
            p = os.path.join(root, f)
            for sh in shingles(p):
                index.setdefault(sh, set()).add(os.path.relpath(p, up).replace('\\', '/'))
    texts = [f for f in files if os.path.splitext(f)[1] in TEXT_EXT]
    mdebug, fields, clean = set(), field_names(), cleanroom_shingles()
    for rel in texts:
        if rel.startswith(MDEBUG_OUTPUT):
            mdebug |= shingles(os.path.join(HERE, rel))
    hits = {}
    for rel in texts:
        for sh in shingles(os.path.join(HERE, rel)):
            for u in index.get(sh, ()):
                lines = sh.split('\n')
                if all(SYMBOL_LINE.match(l) for l in lines):
                    why = 'symbol-list entries (see names)'
                elif all(SEGMENT_LINE.match(l) for l in lines):
                    why = 'segment layout'
                elif sh in mdebug:
                    why = 'also in the .mdebug output'
                elif stabs_struct(lines, fields):
                    why = 'struct fields named by the .mdebug'
                elif sh in clean and rel in CLEANROOM.values():
                    why = 'inside an independent rewrite'
                else:
                    why = 'UNEXPLAINED'
                d = hits.setdefault(rel, {}).setdefault(u, {})
                d.setdefault(why, []).append(sh)
    return {rel: {u: {why: {'count': len(v), 'example': v[0]} for why, v in ws.items()} for u, ws in us.items()}
            for rel, us in sorted(hits.items())}


def elf_symbols(path):
    """{name: {addresses}} from an ELF32 little-endian .symtab (no dependencies)."""
    data = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', data, 0x20)
    shentsize, shnum = struct.unpack_from('<HH', data, 0x2e)
    secs = [struct.unpack_from('<IIIIIIIIII', data, shoff + i * shentsize) for i in range(shnum)]
    out = {}
    for s in secs:
        if s[1] != 2:                               # SHT_SYMTAB
            continue
        strtab = secs[s[6]]
        for off in range(s[4], s[4] + s[5], 16):
            name_off, value, _size, _info, _other, _shndx = struct.unpack_from('<IIIBBH', data, off)
            end = data.index(b'\0', strtab[4] + name_off)
            name = data[strtab[4] + name_off:end].decode('latin-1')
            if name and value:
                out.setdefault(name, set()).add(value)
    return out


def read_symbols(path):
    for line in open(path, encoding='utf-8'):
        m = re.match(r'\s*(\S+)\s*=\s*(0x[0-9a-fA-F]+);', line)
        if m:
            yield m.group(1), int(m.group(2), 16)


def compare_names(up, kr_elf):
    tab = elf_symbols(kr_elf)
    upstream_only = {n for n, a in read_symbols(os.path.join(up, 'config/symbol_addrs.txt'))
                     if n not in tab}
    r = {'Korean .symtab name': 0, 'auto name': 0, 'ggx-only name': [], 'other': []}
    for name, _addr in read_symbols(os.path.join(HERE, 'us/symbol_addrs.txt')):
        if name in tab:
            r['Korean .symtab name'] += 1
        elif AUTO_NAME.match(name):
            r['auto name'] += 1
        elif name in upstream_only:
            r['ggx-only name'].append(name)
        else:
            r['other'].append(name)
    return r


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--upstream', required=True)
    ap.add_argument('--kr-elf')
    ap.add_argument('--json')
    a = ap.parse_args()
    rev = subprocess.run(['git', 'rev-parse', '--short', 'HEAD'], cwd=a.upstream,
                         capture_output=True, text=True).stdout.strip()
    files = tracked_files()
    report = {'upstream_rev': rev, 'headers': check_headers(a.upstream),
              'functions': compare_functions(a.upstream), 'aliases': check_aliases(files),
              'shared_text': compare_text(a.upstream, files)}
    if a.kr_elf:
        report['names'] = compare_names(a.upstream, a.kr_elf)

    bad = 0
    print(f'ggx {rev}')
    print(f'ggx headers present here: {report["headers"] or "none"}')
    bad += len(report['headers'])
    funcs = report['functions']
    print(f'{len(funcs)} ggx C functions')
    for st in sorted({f['status'] for f in funcs}):
        print(f'  {st}: {sum(f["status"] == st for f in funcs)}')
    for f in funcs:
        if not f['status'].startswith('dictated'):
            print(f'    {f["name"]:24} {f["status"]:20} similarity={f.get("similarity")} '
                  f'longest_run={f.get("longest_shared_run")} tokens ggx/ours={f["upstream_tokens"]}/{f.get("ours_tokens")}'
                  + (f' (US {f["us_name"]})' if 'us_name' in f else ''))
        bad += f['status'] in ('IDENTICAL TOKENS', 'SIMILAR')
    print(f'ggx aliases/macros in src/ and include/: {len(report["aliases"])}', report['aliases'][:10])
    bad += len(report['aliases'])
    print(f'files sharing 4-line shingles with ggx: {len(report["shared_text"])}')
    for rel, us in report['shared_text'].items():
        scope = 'research-only' if rel.startswith(RESEARCH_ONLY) else 'tree'
        for u, ws in us.items():
            for why, d in ws.items():
                print(f'  [{scope}] {rel} <- {u}: {d["count"]} {why}')
                bad += why == 'UNEXPLAINED' and scope == 'tree'
    if 'names' in report:
        r = report['names']
        print(f'names, us/symbol_addrs.txt: {r["Korean .symtab name"]} Korean .symtab names, '
              f'{r["auto name"]} auto names, ggx-only {len(r["ggx-only name"])} {r["ggx-only name"][:10]}, '
              f'other {len(r["other"])} {r["other"][:10]}')
        bad += len(r['ggx-only name'])
    if a.json:
        with open(a.json, 'w') as f:
            json.dump(report, f, indent=1)
    print('UNEXPLAINED ITEMS:', bad)
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
