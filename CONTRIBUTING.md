# Contributing

**Licence rule:** by opening a pull request you dedicate your changes to the public domain under [CC0 1.0](LICENSE). Code you adapt from elsewhere must be under a compatible licence that you name (see below).

## Ground rules

- **No game data.** Don't commit assets, binaries, disc images, files from a disc, or disassembly of the game (split or whole), and don't link to them. The build generates its assembly from your own executable and keeps it out of git. See [LEGAL.md](LEGAL.md).
- **Screenshots are welcome** in `docs/screenshots/`: PNG, JPEG or WebP, at most 2 MB each, of the game running. No logos, box art or other artwork.
- **No compiler.** The PS2 toolchain is fetched by `scripts/fetch_toolchain.py`. Never commit it.
- **Only your own work, or code under a compatible licence that you name.** Don't copy from unlicensed projects, leaked SDKs or other decompilations without a licence. If you adapt licensed code, say where it comes from in the file's first comment and add it to [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Matching code

- The build must stay byte-identical with the US executable (SLUS-20287), with every game function linked from C. Build before you open a pull request; the README has the commands, and `scripts/progress.py --check` must pass.
- Write C the way a programmer would have written it. Use the names and types in `include/generated/`.
- A fake match must be tagged `FAKEMATCH` in the source and listed in [FAKEMATCHES.md](FAKEMATCHES.md). Replacing one with natural C that matches is the most useful contribution there is.
- SDK and C-runtime code goes in `libs/sony-sdk/`; a function counts only when the full build stays byte-identical with it linked in.

## Before you open a pull request

- Run `python3 scripts/audit_repo.py` and make sure it reports no findings.
