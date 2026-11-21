# Lineage: this tree against WistfulHopes/ggx

[WistfulHopes/ggx](https://github.com/WistfulHopes/ggx) has no licence, so a public release of this tree may contain nothing derived from its code. ggx is a Guilty Gear X Plus (Korean disc) project. This tree is the Guilty Gear X US decompilation (SLUS-202.87), built on the X Plus decompilation, whose own lineage report covers the X Plus discs. This report covers the US tree: it compares it with ggx at `85b1242` (2025-07-27, its HEAD), separates what came from ggx from what is ours, and records how each ggx-derived piece was removed (2026-11-20).

**Result: no file in the tree derives from ggx code.**
- **Headers:** all nine of ggx's headers are gone, and the six translation units that used them declare their own types and prototypes.
- **Functions:** of ggx's 52 C functions, 6 have independently written bodies here, 43 have a form the binary dictates (an empty body, or one statement, under the `.mdebug` signature), 1 was already ours, and 2 are not in the US build.
- **Names:** every name in `us/symbol_addrs.txt` is a name on the Korean disc's `.symtab`, a splat-style address name, or ours. ggx's 147 own names (jump-table labels and register names) were removed.
- **Layout:** every US segment starts where an X Plus Korean `.mdebug` source-file record does.
- **Lineage check:** `scripts/lineage.py` finds 0 unexplained items, and `scripts/audit_repo.py` keeps the removed headers, aliases and names out of the tree.
- **Build:** the full ELF stays byte-identical, with all 3,266 counted game functions still linked from C (see "Build proof").

## How it was checked

```sh
git clone https://github.com/WistfulHopes/ggx <scratch>/ggx && git -C <scratch>/ggx checkout 85b1242
python3 scripts/lineage.py --upstream <scratch>/ggx --kr-elf SLPM_645.25 --json lineage.json
python3 scripts/audit_repo.py
```

`scripts/lineage.py` is the X Plus check adapted to this tree. It exits 1 if anything is unexplained.

1. **Headers.** None of ggx's `include/` paths may exist here.
2. **Functions.** Every C function body in ggx's `src/` is compared with our definition of the same name (or its US counterpart: ggx's `ChaseInit` is `func_001581D0` here) as token sequences. Whitespace and comments are ignored, and ggx's `s32`/`u16` aliases are mapped to native C. The signature check uses the `.mdebug` signatures in `include/generated/<tu>.h`.
3. **Aliases.** None of ggx's type aliases or macros (`s32`, `u16`, `UNK_TYPE`, `INCLUDE_ASM`, ...) may appear in `src/` or `include/` outside the generated tree.
4. **Shared text.** Every tracked text file here is compared with every ggx text file as 4-line shingles of normalised lines. ggx's `kappa-db.json` (reference disassembly) is never shingled; nothing here contains it. A shared shingle is explained only if one of these holds:
   - every line is a symbol-list entry or a splat segment line (checks 5 and the layout check below cover those);
   - it is in the `.mdebug` output (`include/generated/`), or it is a struct whose fields all carry `.mdebug` field names;
   - it lies inside one of the independently written functions.
5. **Names.** Every name in `us/symbol_addrs.txt` is checked against the Korean ELF's own `.symtab`, using a built-in ELF reader.

`scripts/audit_repo.py` also runs a guard that needs no ggx checkout: it fails if any of the nine header paths returns, if anything includes one, if a ggx alias or macro appears in `src/` or `include/` outside the generated tree, or if `us/symbol_addrs.txt` holds a `switchdataD_` or `REG_` name.

Result on this tree:

| Check | Result |
| --- | --- |
| ggx headers present | none |
| ggx functions | 6 independent rewrites, 42 dictated empty bodies, 1 dictated one-statement body, 1 independent, 2 absent |
| ggx aliases and macros in `src/`, `include/` | 0 |
| Shared shingles | only inside the independent rewrites, or structs whose fields are all `.mdebug` names; plus symbol-list entries |
| `us/symbol_addrs.txt` | 4,518 Korean `.symtab` names, 250 address names, 4 ours, 0 ggx-only |
| Unexplained | 0 |

## What came from ggx, and what happened to it

### Headers (9 in ggx, all removed)

| ggx header | Was here | Now |
| --- | --- | --- |
| `include/common.h` | `s8`..`f32` typedefs, `UNK_*`/`ARRAYSIZE`/`CLEAR` macros, `NULL`, and includes of generated types | **Deleted.** act, colli, cpu, key, tension and throw use native C types, and `NULL` comes from `include/ggxeuc/objtypes.h`. `key.c`'s two `u64` are `unsigned long`. |
| `include/include_asm.h` | `INCLUDE_ASM`/`INCLUDE_RODATA` | **Deleted.** Unused: nothing here calls `INCLUDE_ASM`. |
| `include/ggxeuc/act.h`, `effect.h`, `gcl.h`, `objwork.h`, `player.h` | act.c's prototypes and externs, and otherwise only includes of generated types | **Deleted.** `include/ggxeuc/objtypes.h` (ours, as in the X Plus cleanup) selects the generated types those TUs use. act.c declares its own functions and globals in the forms it matches with (names and types from `include/generated/act.h`), and includes `CREATE_ARG` directly. |
| `include/ggxeuc/actsub.h`, `js_act.h` | prototypes; one extern | **Deleted.** Included by nothing. |

### C functions (52 in ggx)

| ggx functions | Here | How |
| --- | ---: | --- |
| `af_ASTFLAGCONTROL`, `af_BACK_MOTION`, `af_AFTERIMAGE`, `SetAnimeNo` (act.c), `ObjectCreate_ActNo` (object.c) | 5 | **The X Plus clean-room rewrites** (below), carried over unchanged and verified on the US build. Before this cleanup the first four were token-identical to ggx and `ObjectCreate_ActNo` differed from ggx's by a few tokens. |
| `ChaseInit` (chase.c) | 1 | **The X Plus clean-room rewrite, adapted to US.** The US function has no `.symtab` name and is `func_001581D0` here. It was ported from the clean-room `ChaseInit` on 2026-10-30: the US build allocates `ChaseEffect` with `smalloc` first and has no Justice, so no `JSTokorotenFlag` store. |
| 42 empty functions (`MakeFrameBuffer`, `ChaseEnd`, the `*_ColliCheck` stubs, ...) | 42 | **Dictated.** The header is the `.mdebug` signature and the body is `{ }`. |
| `IsChaseFade` | 1 | **Dictated.** `int IsChaseFade(void)` with `return ChaseFlag;`. |
| `GetChaseTrnsl` | 1 | **Ours.** Token similarity 0.61 to ggx, longest shared run 6 tokens. |
| `stst_exit`, `stst_font_exit` | 0 | Not in the US build. |

**The clean-room rewrites (X Plus, 2026-10-25).** A separate agent wrote the six functions from two inputs only: the original JP assembly of each function, and our extraction of the Korean disc's STABS. It worked in a copy of the tree with the six definitions removed and was forbidden to read any existing C for them: ggx, this repository and its history, earlier attempts and build caches. Each matched on the JP and Korean X Plus discs.

On the US build they compile with the US compiler (EE-GCC 2.9-ee-991111-01) and the US layouts from `include/generated/`, and match as they stand. The only edit is placement: `SetAnimeNo`'s three callee prototypes (`SetDefaultHitVoice`, `SetDefaultTension`, `ExCollisionClear`) sit directly above it, as in X Plus.

| Function | Token similarity to ggx | Longest shared run (tokens) |
| --- | ---: | ---: |
| `af_ASTFLAGCONTROL` | 0.61 | 12 |
| `af_BACK_MOTION` | 0.60 | 15 |
| `af_AFTERIMAGE` | 0.79 | 19 |
| `SetAnimeNo` | 0.68 | 118 |
| `ObjectCreate_ActNo` | 0.75 | 8 |
| `func_001581D0` (`ChaseInit`) | 0.71 | 69 |

The two long runs are stores the binary dictates: `SetAnimeNo` resets eleven `HitParam` fields and then `trans`/`transmode`, and `ChaseInit` is a list of stores to `wtf[0..5]`, written here in index order where ggx's order is shuffled.

### Names, layout and types

| Input | ggx's part | Now |
| --- | --- | --- |
| `us/symbol_addrs.txt` | The US names were placed by our `scripts/align.py`, `make_us.py` and `xref.py`, which read ggx's `config/symbol_addrs.txt` as the list of Korean function starts and data names. That list also holds ggx's own names: 20 Ghidra jump-table labels (`switchdataD_*`) and 127 hardware-register names (`REG_*`), all of which had been carried into the US list. | **Those 147 lines are removed**, and `xref.py` now keeps only names on the Korean disc's `.symtab`, so a regeneration does not bring them back. The list holds 4,518 Korean `.symtab` names placed at US addresses, 250 address names (`func_`, `D_`), and 4 SDK names from the X Plus SDK work (`sceDmaDebug`, `_printf_r`, `printf`, and the prefixed libgcc local `fpbit_fpadd_parts`). |
| `us/slus_202.87.yaml` | none directly. `make_us.py` moves the X Plus Korean segment list to US addresses. | Every one of the 237 Korean segment starts in the placement record (`data/us-tus.csv`, a research record not published with the source) is a Korean `.mdebug` source-file start (or the start of `.text`), so the US layout traces to the disc's debug records. |
| `include/generated/` | none. These are the X Plus headers, generated by the X Plus `scripts/gen_headers.py` from our STABS extraction of the Korean ELF. | Unchanged apart from 12 files with the GGX US layouts (CHARACTER_WORK without `actnonexttemp`, PLAYER_ENTRY without the four combination-button masks, 17-character tables, and the sound, BGM, result and backup structs), each commented where it differs. |

### Scripts and tools

No ggx tool code is in the tree. `scripts/align.py` and `scripts/xref.py` are ours, but they read ggx's `config/symbol_addrs.txt` and `kappa-db.json` (raw words) as **input data**, as the X Plus versions did. Their outputs are facts about the discs: which Korean function sits at which US address, and the US addresses of the Korean disc's data names. Re-running them needs a ggx checkout, which is not distributed. They are research tools, not build inputs, and the public release leaves them out, as it does the research CSVs under `data/` (`us-data-placement.csv` and others), which still list ggx's labels as rows of what was placed and where. The build's byte check uses only `scripts/mipsrefs.py`, the instruction scan split out of `xref.py`, which reads no ggx data.

## What is ours

- **Game C:** all 3,266 counted game functions. Everything in `src/ggxeuc/` is our own C, apart from the 43 dictated bodies above, whose form the binary fixes. Most of it was ported from the X Plus decompilation and verified against the US executable; the rest was matched for the US build.
- **Names, types, signatures and layout:** these come from the Korean X Plus disc's own `.symtab` and `.mdebug` STABS, extracted by the X Plus tools and moved to the US build by ours. They are facts the disc carries, not ggx's work. ggx read the same data.
- **Tooling:** `build.py` and `scripts/`.

## Credit

ggx is credited as prior research on X Plus: it read the Korean disc's symbols and debug data first, and its symbol list and reference disassembly were inputs to our placement scripts. The original game is by Arc System Works.

## Build proof

On 2026-11-20, after each cleanup step, the tree was exported as git stores it (LF line endings) and built from scratch on a Linux x86-64 host. The build was `build.py --target us --keep-going` with no `--sdk-tree`, so the SDK and C runtime stay assembly, and it ran the EE-GCC 2.9-ee-991111-01 cc1 with the dtls13010 GAS, as the README describes.

| Step | Rebuilt ELF SHA-256 | Identical | Game functions linked from C | Game bytes | C left as assembly |
| --- | --- | :---: | ---: | ---: | ---: |
| Names: ggx's 147 names removed | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` | yes | 3,266 / 3,266 | 611,804 / 611,804 | 0 |
| Headers: nine ggx headers removed | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` | yes | 3,266 / 3,266 | 611,804 / 611,804 | 0 |
| Functions: independent bodies | `7e8000d17ba3c74c4793147d5a5dae57c4b1911439b0f9a91a9f8e79461eefc9` | yes | 3,266 / 3,266 | 611,804 / 611,804 | 0 |

These are the same game counts as the build before the cleanup (`ggx-progress.json`), and no counted function fell back to assembly. The five tagged compiler stand-ins in [FAKEMATCHES.md](FAKEMATCHES.md) are unchanged. SDK coverage from the SDK C in `libs/sony-sdk` is reported separately and was not part of these builds.
