# Guilty Gear X

[![License: CC0 1.0](https://img.shields.io/badge/license-CC0--1.0-blue)](LICENSE)

A matching decompilation of the original *Guilty Gear X* for the PlayStation 2, US release (SLUS-20287). The C in `src/` compiles back to the game's code byte for byte, and the whole executable rebuilds identical to the one on the disc.

This is the original-X matching-source release. The native PC port is built on *Guilty Gear X Plus* and is not part of this repository; the X Plus decompilation is at [guilty-gear-x-plus](https://github.com/devin-thomas/guilty-gear-x-plus).

![Sol against Chipp in Guilty Gear X, US release](docs/screenshots/match.png)

More screenshots of the US disc are in [`docs/screenshots/`](docs/screenshots/).

## Status

**3,266 / 3,266 game functions** are C (611,804 / 611,804 bytes of game code, 116 / 116 translation units), and the executable rebuilds byte-identical:

| Disc | Executable | SHA-256 |
| --- | --- | --- |
| SLUS-20287 (USA) | `SLUS_202.87` | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` |

[VERIFICATION.md](VERIFICATION.md) records the build that checked this, and [ggx-progress.json](ggx-progress.json) has the counts per translation unit.

Five functions match through tagged compiler stand-ins: constructs that steer the 1999 compiler rather than the source a programmer would have written. Each is marked `FAKEMATCH` in the source and explained in [FAKEMATCHES.md](FAKEMATCHES.md), with the natural forms that were tried. They are included in the count.

The Sony SDK and C runtime linked into the game are counted separately: **409 / 581 SDK and C-runtime functions** are C, in [`libs/sony-sdk/`](libs/sony-sdk/README.md). The rest stay assembly, which the build generates from your own executable. Finishing the SDK in C is a separate milestone.

## How it was made

- *Guilty Gear X* shares its engine with *Guilty Gear X Plus*. The work began by porting the X Plus decompilation's C, headers and build to the US disc of X, then matching where the two games differ.
- X's executable has no symbols. Names, types and the 116 translation units come from the X Plus Korean disc's own debug data, placed in X by matching code.
- The compiler is EE-GCC 2.9-ee-991111-01 (`cc1`, `-O2 -G8`), not the 2.9-ee-991111 that X Plus uses. Game code is assembled by the GAS of the `ee-gcc2.9-991111-01-dtls13010` package, except `object` and `cp_obj`, which need SCE's GAS 2.10.
- Most of the matching was done by AI agents, Anthropic's Claude and OpenAI's Codex, working under Lil Gohan's direction. A function counted only when the whole executable stayed byte-identical with it linked in. Comments and explanations are the agents' reading and can be wrong.

## Building

You need Linux (or WSL) on x86-64 with the 32-bit C library (Debian/Ubuntu: `libc6-i386`), Python 3.12+, and the executable from your own disc. No game data is included; see [LEGAL.md](LEGAL.md).

```sh
python3 -m venv .venv && .venv/bin/pip install -r requirements.txt
.venv/bin/python scripts/fetch_toolchain.py --dest tools  # EE-GCC and binutils; never committed
export GGX_TOOLS=$PWD/tools GGX_QEMU=                     # the i386 compiler runs directly
.venv/bin/python build.py --elf /path/to/SLUS_202.87 --work /tmp/ggx-us \
    --binutils $PWD/tools/binutils --map /=/ --keep-going --sdk-tree libs/sony-sdk
.venv/bin/python scripts/progress.py --result /tmp/ggx-us/result.json --check
```

Copy `SLUS_202.87` off your disc first; it is at the root of the disc image. The build checks its hash and stops on any difference. The work directory must be outside a git checkout.

`--keep-going` is the gate: a function whose C does not match stays assembly, is listed in `result.json`, and the build exits 1. The build passes when the rebuilt executable is byte-identical and no C function was left out. Without `--sdk-tree`, the SDK stays assembly and the game result is the same. On a host without i386 support, set `GGX_QEMU` to a qemu-i386 command instead.

## Source layout

| Path | What |
| --- | --- |
| `src/ggxeuc/` | the game's C, one file per translation unit |
| `include/` | headers: `generated/` holds the types and declarations from the X Plus debug data, with X's layouts where they differ |
| `libs/sony-sdk/` | the Sony SDK and C runtime in C, with their compile profiles |
| `us/slus_202.87.yaml`, `us/symbol_addrs.txt` | the splat configuration and the names placed in X |
| `us/census.csv` | every function of the executable: address, size, translation unit, game or SDK, and where its name came from (`xplus`: the X Plus Korean disc's symbols; `auto`: an address name) |
| `build.py` | the full build and gate |
| `scripts/verify.py` | the per-function byte check the build uses |
| `scripts/fetch_toolchain.py` | downloads and checks the toolchain |
| `scripts/progress.py` | counts what a build linked from C |
| `scripts/audit_repo.py` | checks the tree for game data, and for anything taken from ggx, before a commit |
| `scripts/lineage.py`, `LINEAGE.md` | the check against WistfulHopes/ggx and its result |

## Licence

The decompilation is dedicated to the public domain under [CC0 1.0](LICENSE). Some SDK and C-runtime files in `libs/sony-sdk/` keep their upstream terms; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md). This is an unofficial fan project, not affiliated with Arc System Works or Sammy.

## Credits

- Lil Gohan ([@gohan__FGC](https://twitter.com/gohan__FGC), [fgc.lilgohan.com](https://fgc.lilgohan.com)) started and led this decompilation and the X Plus one before it.
- *Guilty Gear X* was made by Arc System Works.
- [WistfulHopes/ggx](https://github.com/WistfulHopes/ggx) was the first decompilation of *Guilty Gear X Plus*, where the X Plus work began. None of its code is included here ([LINEAGE.md](LINEAGE.md) records the check), but it showed the way. Thank you, WistfulHopes.
- [decomp.me](https://decomp.me) and decompals, for the compiler archive and the toolchain.
- [nathanialf/ico](https://github.com/nathanialf/ico), for SDK and C-runtime reconstructions (MIT).
- [chaoticgd/ccc](https://github.com/chaoticgd/ccc), for the STABS reader that read the X Plus Korean disc's debug data.
- [splat](https://github.com/ethteck/splat) and [spimdisasm](https://github.com/Decompollaborate/spimdisasm), which split the executable at build time.
