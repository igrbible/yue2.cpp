# Ruach Studio's yue2.cpp

This branch, `ruach-studio`, is [Ruach Studio](https://github.com/igrbible/Ruach_Studio)'s derivative of [yue2.cpp](https://github.com/ServeurpersoCom/yue2.cpp) by ServeurpersoCom. It holds the engine and the page exactly as they ship in `build/` of Ruach Studio's public releases, one commit a release, laid over the upstream commit the Studio last merged: `8af0174` «ggml: fork rebase» (16.10.2026, merged into the Studio as HERESY 1604). The fork's `master` stays a plain mirror of upstream.

We hide nothing and we are not ashamed of it. Every change we made to yue2.cpp is here in the open, so that anyone can read it, compare it with upstream, take a piece of it into a pull request, or build on it.

## What it changes

- **The Studio's page** (`tools/console/`): the whole Ruach Studio interface over upstream's console, grown from [YuE2 Kit](https://github.com/IronWolve/yue2-kit) by IronWolve: the rooms, the library, the piano roll, the DAW, the Refiner's canvas, the Trainer, the Narrator, the translations.
- **The server** (`tools/yue-server.cpp`): the Studio's API, the library and its takes, decoders by name, fades, the https door, the assets' cache keys (`tools/asset-cache.h`).
- **The engine** (`src/`): LoRA adapters merged at load (`lora.h`) and applied while the graph runs on a wave through the song (`lora-rt.h`), the voice and genre sliders (`sliders.h`), the planned score's chord variety (`harmony.h`, ported from [timoncool/yue2.cpp](https://github.com/timoncool/yue2.cpp), MIT), SheetSage2-Refined as a second transcriber (`sheetsage-refined.h`, after [scragnog/HOT-Step-CPP](https://github.com/scragnog/HOT-Step-CPP)'s conversion of [anime-song/SheetSage2-Refined](https://github.com/anime-song/SheetSage2-Refined), MIT code), a FLAC encoder (`flac-enc.h`), whole-or-nothing writes (`atomic-write.h`), and changes through the pipeline, the NAR, the VAE and the request.
- **MuScriptor** (`muscriptor/`): a small tool that turns a sound into multi-instrument MIDI through [muscriptor.cpp](https://github.com/DamRsn/muscriptor.cpp) (MIT).
- **ggml** stays a submodule; `build.sh` applies `ggml-patches/` over it. **audio.cpp** ([0xShug0/audio.cpp](https://github.com/0xShug0/audio.cpp)) is a submodule from 2.0.7 on.
- **cpp-httplib** builds with gzip where the machine has zlib (`vendor/cpp-httplib/CMakeLists.txt`, HERESY 2035).
- **Tests** of our own in `tests/`.

Every change is numbered and explained in the Studio's patch log, `heresy/docs/HERESY.md` of [Ruach Studio](https://github.com/igrbible/Ruach_Studio/blob/master/heresy/docs/HERESY.md).

## The commits

| release | HERESY | upstream under it |
|---|---|---|
| [2.0.0](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.0) | up to 1797 | `8af0174`, ggml `cc5df49` |
| [2.0.1](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.1) | 1798–1811 | the same |
| [2.0.2](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.2) | 1812–1814 | the same |
| [2.0.3](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.3) | 1815–1963 | the same |
| [2.0.4](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.4) | 1964–1976 | the same |
| [2.0.5](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.5) | 1977–2009 | the same |
| [2.0.6](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.6) | 2010–2162 | the same |
| [2.0.7](https://github.com/igrbible/Ruach_Studio/releases/tag/v2.0.7) | 2163–2391 | `8af0174`, ggml `2101389` (0.29.0, HERESY 2240), audio.cpp `408935b` |

The release candidates of 2.0.0 are not here: they ran on an older upstream, before the Studio merged `8af0174`. Upstream's `tests/*.log`, which the Studio's release package leaves out, are kept as upstream has them.

## Licence

Upstream's code keeps its MIT licence (`LICENSE` in this tree), and so do ggml and the other MIT projects inside. Our changes are licensed as Ruach Studio's release states it: the **GNU Affero General Public License, version 3 or any later version**, and from 2.0.5 on with the [additional terms of its section 7](https://github.com/igrbible/Ruach_Studio/blob/master/LICENSE-ADDITIONAL-TERMS.md) ([LICENSE](https://github.com/igrbible/Ruach_Studio/blob/master/LICENSE), [NOTICE](https://github.com/igrbible/Ruach_Studio/blob/master/NOTICE)). The parts that came from YuE2 Kit by IronWolve keep its Apache License 2.0. A copy or a work built on these changes keeps the line:

> Based on Ruach Studio by Viktor Zhuromskyy · https://github.com/igrbible/Ruach_Studio · GNU AGPL v3 or later
