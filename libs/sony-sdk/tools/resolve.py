"""Resolve SCE_SDK conditionals in a shared source for one library version.

    python3 tools/resolve.py src/libpad/libpad.c 0x2500 > libpad.resolved.c

For game builds that split C per function before compiling (the Guilty Gear
builds' csplit flow), a member must be plain C for its version. This keeps every other
preprocessor line, and replaces only #if/#elif conditions that mention SCE_SDK
(comparisons of SCE_SDK with integer constants, joined by && || !), plus their
#else/#endif, with blank lines so line numbers stay the same.
"""
import re
import sys

COND = re.compile(r'^\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)$')


def evaluate(expr, version):
    if 'SCE_SDK' not in expr:
        return None
    e = expr.split('//')[0]
    e = re.sub(r'/\*.*?\*/', '', e)
    e = e.replace('SCE_SDK', str(version)).replace('&&', ' and ').replace('||', ' or ')
    e = re.sub(r'!(?!=)', ' not ', e)
    if not re.fullmatch(r'[\s0-9a-fA-Fx()<>=!andortx]*', e):
        raise ValueError('cannot evaluate: ' + expr)
    return bool(eval(e, {'__builtins__': {}}))


def resolve(text, version):
    out, stack = [], []     # stack: None (foreign #if) or [keep_now, taken]
    def live():
        return all(s is None or s[0] for s in stack)
    for line in text.splitlines(keepends=True):
        m = COND.match(line)
        kind, rest = (m.group(1), m.group(2)) if m else (None, '')
        if kind == 'if' or kind == 'elif':
            v = evaluate(rest, version)
            if kind == 'if':
                if v is None:
                    stack.append(None)
                else:
                    stack.append([v, v])
                    out.append('\n')
                    continue
            elif stack and stack[-1] is not None:
                if v is None:
                    raise ValueError('#elif without SCE_SDK inside an SCE_SDK block')
                top = stack[-1]
                top[0] = v and not top[1]
                top[1] = top[1] or v
                out.append('\n')
                continue
        elif kind in ('ifdef', 'ifndef'):
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


if __name__ == '__main__':
    sys.stdout.write(resolve(open(sys.argv[1]).read(), int(sys.argv[2], 0)))
