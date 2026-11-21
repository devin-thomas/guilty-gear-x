"""SDK C: GGX's SDK 2.3.0 C comes from the SDK tree in libs/sony-sdk.

The tree's export for this build (export/ggx-us.json) lists every SDK object with its source,
compile profile (cc1 package, flags, assembler, SCE_SDK define, include groups) and which of its
functions the SDK tree matched. load() turns it into, per GGX SDK segment (splat TU):
  text     the objects' sources with their SCE_SDK conditionals resolved for 0x2300
           (tools/resolve.py), concatenated in address order;
  funcs    the function names to compile here: matched in the export and placed in the segment;
  profile  compiler package and version, cc1 flags, GAS package, cpp predefines and -D defines,
           include directories (host paths into the tree).
Nothing in the tree is edited. Each function is still byte-checked by this project's own gate
(build.py), and a function the gate rejects is left as assembly (data/sdk-drop.txt).
"""
import importlib.util
import json
import os
import re
import subprocess
from pathlib import Path

RELEASE = '2300'


def _mod(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    m = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(m)
    return m


def load(tree, tus, build='ggx-us', drop=()):
    """tree: libs/sony-sdk checkout; tus: [(lo, hi, name)] GGX segments. Returns
    ({segment: {'text', 'funcs', 'profile', 'objects'}}, info)."""
    tree = Path(tree)
    exp = json.loads((tree / 'export' / f'{build}.json').read_text())
    prof = _mod(tree / 'tools/profiles.py', 'sdk_profiles')
    res = _mod(tree / 'tools/resolve.py', 'sdk_resolve')
    root = tree.parent.parent                      # sources are named libs/sony-sdk/src/...

    def seg_of(a):
        for lo, hi, n in tus:
            if lo <= a < hi:
                return n
        return None
    out = {}
    for o in exp['objects']:
        if not o.get('source'):
            continue
        matched = [f for f in o['functions'] if f['matched'] and f.get('c_name')]
        if not matched:
            continue
        p = o['profile']
        name = p['name']
        if name not in prof.PROFILES:
            continue
        P = prof.PROFILES[name]
        cc = prof.CC[P['cc']]
        incs = [str(tree / i.replace('libs/sony-sdk/', '')) for i in p.get('include', [])]
        defs = ' '.join(f'-D{k}={v}' for k, v in sorted(p.get('defines', {}).items()))
        profile = dict(name=name, cc_dir=f'{os.environ.get("GGX_TOOLS", "/tools")}/{cc["pkg"]}', cc_ver=cc['ver'], flags=P['flags'],
                       as_dir=f'{os.environ.get("GGX_TOOLS", "/tools")}/{prof.AS[P["as_"]]}', cpp_defs=prof.predefines(name) + ' ' + defs,
                       incs=incs)
        src = (root / o['source']).read_text()
        version = int(p.get('defines', {}).get('SCE_SDK', '0x' + RELEASE), 16)
        text = res.resolve(src, version) if 'SCE_SDK' in src else src
        for f in matched:
            seg = seg_of(int(f['addr'], 16))
            if seg is None or not seg.startswith('sdk/') or f['c_name'] in drop:
                continue
            e = out.setdefault(seg, dict(texts={}, funcs=set(), profile=dict(profile, incs=list(incs)),
                                         objects=[]))
            if {k: v for k, v in e['profile'].items() if k != 'incs'} !=                     {k: v for k, v in profile.items() if k != 'incs'}:
                continue                           # one profile per segment (the first object's)
            # the object's own private headers (<object>_k.h) sit next to its source
            sd = str((root / o['source']).parent)
            if sd not in e['profile']['incs']:
                e['profile']['incs'].insert(0, sd)
            e['texts'].setdefault(o['source'], (int(o['layout']['start'], 16), text))
            e['funcs'].add(f['c_name'])
            if o['object'] not in e['objects']:
                e['objects'].append(o['object'])
    for seg, e in out.items():
        e['text'] = ''.join(f'/* {s} (libs/sony-sdk, SCE_SDK resolved for {RELEASE}) */\n{t}\n'
                            for s, (a, t) in sorted(e['texts'].items(), key=lambda kv: kv[1][0]))
        del e['texts']
    try:
        commit = subprocess.run(['git', '-C', str(tree), 'rev-parse', 'HEAD'], capture_output=True,
                                text=True).stdout.strip()
    except OSError:
        commit = ''
    if not commit and (tree / 'COMMIT').exists():
        commit = (tree / 'COMMIT').read_text().strip()
    return out, {'tree': str(tree), 'commit': commit, 'export_summary': exp.get('summary')}
