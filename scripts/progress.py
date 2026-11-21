#!/usr/bin/env python3
"""Count what a byte-identical build linked from C: ggx-progress.json.

Reads build.py's result.json and the census (us/census.csv: every function of
SLUS_202.87 with its address, size, translation unit and whether it is game code or
SDK/C runtime). A function counts only if the full build that produced result.json was
byte-identical and linked it from C.

  progress.py --result WORK/result.json [--out ggx-progress.json] [--check]

--check compares the counts with the existing --out file instead of writing it, and
exits 1 if they differ.
"""
import argparse
import csv
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def counts(res, census):
    if not (res['elf_byte_identical'] and res['rebuilt_sha256'] == res['original_sha256']):
        raise SystemExit('result.json is not from a byte-identical build')
    linked = set(res['c_function_names'])
    by_tu = {}
    for r in census:
        t = by_tu.setdefault(r['tu'], {'matched': 0, 'total': 0, 'matched_bytes': 0, 'total_bytes': 0})
        size = int(r['size'])
        t['total'] += 1
        t['total_bytes'] += size
        if r['name'] in linked:
            t['matched'] += 1
            t['matched_bytes'] += size

    def part(rows, tus):
        return {'functions': {'matched': sum(r['name'] in linked for r in rows), 'total': len(rows)},
                'bytes': {'matched': sum(int(r['size']) for r in rows if r['name'] in linked),
                          'total': sum(int(r['size']) for r in rows)},
                'files': {'matched': sum(1 for t in tus.values() if t['matched'] == t['total']),
                          'total': len(tus)}}
    game = [r for r in census if r['game'] == '1']
    sdk = [r for r in census if r['game'] != '1']
    game_tus = {k: v for k, v in by_tu.items() if not k.startswith('sdk/')}
    sdk_tus = {k: v for k, v in by_tu.items() if k.startswith('sdk/')}
    return {
        'game': 'Guilty Gear X (PS2, US SLUS-202.87)',
        'gate': 'build.py --target us: the full ELF rebuilds byte-identical (SHA-256 '
                + res['rebuilt_sha256'] + ') and every counted function is linked from C',
        'compiler': 'EE-GCC 2.9-ee-991111-01 cc1, -O2 -G8; GAS 2.9-ee-991111 (ee-gcc2.9-991111-01-dtls13010) '
                    'for game code, SCE GAS 2.10 for object and cp_obj (scripts/sdkprofile.py)',
        'game_code': part(game, game_tus),
        'sdk': part(sdk, sdk_tus),
        'sdk_source': 'libs/sony-sdk (build.py --sdk-tree)' if res.get('sdk_tree') else 'none: SDK left as assembly',
        'denominator': 'Census functions of SLUS_202.87 (us/census.csv); bytes exclude alignment padding. '
                       'Game code is every function outside the sdk/ translation units. A file is one '
                       'translation unit; it counts when all its functions are C.',
        'c_functions_not_linked': sorted(res.get('c_functions_not_linked', [])),
        'tu_data_left_in_bins': sorted(res.get('tu_data_left_in_bins', [])),
        'by_tu': dict(sorted(by_tu.items())),
    }


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--result', type=Path, required=True)
    ap.add_argument('--out', type=Path, default=ROOT / 'ggx-progress.json')
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args()
    res = json.loads(a.result.read_text())
    census = list(csv.DictReader((ROOT / 'us/census.csv').open(newline='')))
    out = counts(res, census)
    text = json.dumps(out, indent=1) + '\n'
    if a.check:
        old = json.loads(a.out.read_text())
        same = all(old.get(k) == out[k] for k in ('game_code', 'sdk', 'c_functions_not_linked', 'by_tu'))
        print('progress matches' if same else 'progress differs from ' + str(a.out))
        return 0 if same else 1
    a.out.write_bytes(text.encode())
    g, s = out['game_code'], out['sdk']
    print(f"game {g['functions']['matched']:,}/{g['functions']['total']:,} functions, "
          f"{g['bytes']['matched']:,}/{g['bytes']['total']:,} bytes, {g['files']['matched']}/{g['files']['total']} files; "
          f"SDK {s['functions']['matched']:,}/{s['functions']['total']:,} functions, "
          f"{s['bytes']['matched']:,}/{s['bytes']['total']:,} bytes")
    return 0


if __name__ == '__main__':
    sys.exit(main())
