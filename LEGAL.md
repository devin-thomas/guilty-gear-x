# Legal

This repository holds a matching decompilation of the original *Guilty Gear X* for the PlayStation 2 (US release). It contains source code and documentation only, plus game screenshots under `docs/screenshots/` where present.

## What is not here

- No game assets: no graphics, sound, music, text dumps, fonts or video taken from the disc.
- No binaries: no game executables, ELFs, overlays or IOP modules.
- No disc data: no disc images, sectors or files taken from a disc.
- No disassembly: no assembly listings of the game, split or whole. The build generates its own from your executable and keeps it out of git.
- No logo or artwork files of Arc System Works or Sammy. Screenshots of the running game show what the game puts on screen, title screen included.
- No compiler: the PS2 toolchain is not vendored (see below).

Pull requests that add any of these are closed. Screenshots of the game running are the one kind of image accepted, as PNG, JPEG or WebP files of at most 2 MB in `docs/screenshots/`.

## Bring your own disc

To build, you need your own legally obtained copy of this disc:

| Disc | Release | Main executable | Size (bytes) | SHA-256 | SHA-1 |
| --- | --- | --- | ---: | --- | --- |
| SLUS-20287 | Guilty Gear X, USA | `SLUS_202.87` | 4,648,276 | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` | `16c49b835681fc24b9878ebda21bd74893c2c693` |

The build reads the executable from your disc and refuses any file with a different hash.

We don't provide disc images or executables, and we don't help anyone find them. Requests for them, and links to them, are removed. Don't pirate the game.

## Toolchain

The matching build needs the PS2 compiler EE-GCC 2.9-ee-991111-01 and the assemblers listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md). They are never stored in this repository. `scripts/fetch_toolchain.py` downloads them at build time from [decompme/compilers](https://github.com/decompme/compilers) and checks their hashes.

## Not affiliated

This is an unofficial fan project. It is not affiliated with, endorsed by or sponsored by Arc System Works, Sammy, Sony Interactive Entertainment or any of their affiliates. *Guilty Gear* and *Guilty Gear X* are trademarks of Arc System Works. *PlayStation* is a trademark of Sony Interactive Entertainment. Every trademark belongs to its owner and is used only to identify the game this code works with.

## Licences

- The decompilation (game C, headers, build scripts, configs and documentation) is dedicated to the public domain under [CC0 1.0](LICENSE).
- Files that carry their own notice keep it. This applies to some SDK and C-runtime code in `libs/sony-sdk/`; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

The licences cover this project's own code. They grant no rights in the game, its code as shipped on the disc, or its assets.

## Lineage

This project began from the Guilty Gear X Plus decompilation, which started from the research in [WistfulHopes/ggx](https://github.com/WistfulHopes/ggx). That repository has no licence. Nothing derived from its code is included here: the functions, headers and configs in this repository were written or regenerated from this project's own work, the X Plus decompilation and the X Plus Korean disc's own debug data, and the release was checked for inherited files before publishing. See [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) for credits.

This file is not legal advice.
