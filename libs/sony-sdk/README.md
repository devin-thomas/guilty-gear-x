# Sony SDK and C runtime

C for the Sony PS2 SDK libraries (libkernl, libgraph, libdma, libdev, libpad, libcdvd, libmc; SDK 2.3.0) and the EE-GCC 2.9 C runtime (newlib libc and libm, libgcc) that `SLUS_202.87` links. This is the part of the executable that is not game code, and it is counted separately.

`build.py --sdk-tree libs/sony-sdk` compiles the functions that `export/ggx-us.json` lists as matched, each with its own compile profile (`tools/profiles.py`: compiler, flags and assembler per library), and byte-checks every one in the same gate as the game code. A function the gate rejects stays assembly. Everything not listed stays assembly generated from your executable at build time. Without `--sdk-tree`, all SDK code stays assembly and the game-code result is unchanged.

| Path | What |
| --- | --- |
| `export/ggx-us.json` | every SDK object and function of this build: address, size, source file, compile profile, and whether it is matched |
| `src/<library>/<object>.c` | one file per archive member; `#if SCE_SDK` blocks select the 2.3.0 version (`tools/resolve.py`) |
| `src/<library>/*_k.h` | constants private to one member |
| `include/{sce,newlib,libm,libgcc}/` | headers, one group per profile family |
| `tools/profiles.py`, `tools/resolve.py` | compile profiles; `SCE_SDK` resolution |
| `LICENSE-ico` | the MIT notice for files adapted from nathanialf/ico |

The `summary` in `export/ggx-us.json` is the SDK tree's own count, made with its own list of SDK functions (413 / 575). The counts this repository reports (409 / 581 in `ggx-progress.json` and `VERIFICATION.md`) come from the build itself, over the functions `us/census.csv` lists as SDK or C runtime, and only include what the build linked from C.

Files adapted from other projects say so in their first comment and keep their terms; see [THIRD_PARTY_NOTICES.md](../../THIRD_PARTY_NOTICES.md).
