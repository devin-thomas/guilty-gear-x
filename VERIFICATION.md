# Verification

This file records one complete build of this repository's source, made by the release check
before publication. Anyone with the disc can repeat it with the commands in the README.

## Input

| | |
| --- | --- |
| Disc | Guilty Gear X, USA (SLUS-20287) |
| Executable | `SLUS_202.87`, 4,648,276 bytes |
| SHA-256 | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` |

The executable came from the disc and was kept outside the source tree. Nothing from it is in
this repository.

## Source

The build ran on a fresh clone of a commit holding exactly this repository's files, except the
two reports written from its result (`ggx-progress.json` and this file).

| | |
| --- | --- |
| Git tree of the built source | `c865e00eedae7e8ffbdcd3b446222baa7394d2dd` |
| SHA-256 of its file manifest | `ed5b29a46d67727a85471e67a884895d38a9c05c08309a104c84d9c32c7f7f31` |
| Files | 1,177 |

To compare a checkout with it: remove `ggx-progress.json` and `VERIFICATION.md`, then
`git add -A && git write-tree` gives the tree id above, and

```sh
git ls-files -z | LC_ALL=C sort -z | xargs -0 sha256sum | sha256sum
```

gives the manifest hash.

## Toolchain

Fetched by `scripts/fetch_toolchain.py`, which checks every archive below and the programs inside it
by SHA-256.

| Archive | SHA-256 |
| --- | --- |
| `binutils-mips-ps2-decompals-linux-x86-64.tar.gz` | `9fe31ea3ee1a37536f9f0e2e12c668e7c3cb99e59f5154bd2f1fc4762473094c` |
| `ee-gcc2.9-991111-01-dtls13010.tar.gz` | `3ab52a7ea962100f2a7df3e04d3e7a758aad669eb365251682e769b755d9d586` |
| `ee-gcc2.9-991111-01.tar.xz` | `ed684fd98f89d36b0121caab311052089103e3b36241fcef4338cc9ea41c75b8` |
| `ee-gcc2.9-991111.tar.xz` | `99cb5db603cd985b50fc6851157020ad69489f2227309ccf825bd43ac7bc19dd` |
| `ee-gcc2.96.tar.xz` | `0590d2ca9da8f5903889d66761220d14b47a8d14ba987ca53db84a1650a1fd0a` |

Python 3.13.5; packages installed from `requirements.txt`, with their dependencies:
PyYAML==6.0.3, colorama==0.4.6, crunch64==0.6.2, intervaltree==3.1.0, n64img==0.3.3, pyelftools==0.33, pygfxd==1.0.5, pylibyaml==0.1.0, pypng==0.20220715.0, rabbitizer==1.16.2, sortedcontainers==2.4.0, spimdisasm==1.42.4, splat64==0.50.1, tqdm==4.67.1.
The i386 compiler ran directly on x86-64 Linux (`GGX_QEMU=''`).

## Command

```sh
GGX_TOOLS=$PWD/tools GGX_QEMU= python build.py --target us --elf SLUS_202.87 --work WORK \
    --binutils $PWD/tools/binutils --map /=/ --keep-going --jobs 4 --sdk-tree libs/sony-sdk
python scripts/progress.py --result WORK/result.json
```

## Result

| | |
| --- | --- |
| Rebuilt executable SHA-256 | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` |
| Byte-identical to the input | yes (whole file, 4,648,276 bytes; `.text` 727,220 bytes) |
| C functions the build tried and left as assembly | 0 |
| Game functions from C | **3,266 / 3,266** |
| Game code bytes from C | 611,804 / 611,804 |
| Game translation units fully C | 116 / 116 |
| SDK and C-runtime functions from C (`libs/sony-sdk`) | 409 / 581 |
| SDK and C-runtime bytes from C | 101,620 / 107,960 |
| C data ranges placed and checked | 1,693 |

Every counted function was compiled from the C in this repository, byte-checked, and linked at its
original address, and the linked executable equals the original byte for byte. The remaining SDK
and C-runtime functions, and the data some translation units still leave in the original's data
sections (32 entries in `ggx-progress.json`), are taken from your executable at
build time.

Five functions match through tagged compiler stand-ins; [FAKEMATCHES.md](FAKEMATCHES.md) lists
them and how each was forced. They are included in the counts above.

## Lineage

`scripts/lineage.py` compared this source with a fresh clone of
[WistfulHopes/ggx](https://github.com/WistfulHopes/ggx) at `85b1242bfd03e89e8cd335dcec78e7aa31aee21e` and found
**0 unexplained items**: 0 of ggx's headers, 0 of its
type aliases or macros, and its C functions as follows: 2 absent here, 42 dictated: empty body, .mdebug signature, 1 dictated: one statement, .mdebug signature, 1 independent, 6 independent rewrite.

The name check read the Korean X Plus executable's own symbol table: `us/symbol_addrs.txt` holds 4,518 names from it, 250 address names, 4 of ours and 0 ggx-only names.
[LINEAGE.md](LINEAGE.md) describes each check.
