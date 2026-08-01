# MPICH Study

This repository contains MyDef source files (`.def`) that implement `mymake`, a shortcut build system for MPICH.

## MyDef

- All source files use the `.def` extension and are written in MyDef, a template language for generating source code.
- Compile with `mydef_page <file>.def` to generate output files. When in doubt about syntax, compile and check the output.
- Use the `/mydef` skill for general MyDef syntax and the `/mydef-c` skill for C-specific MyDef patterns.

## mymake

`mymake.def` compiles to `mymake/mymake.pl` (Perl). It is a shortcut build system for MPICH with two modes:

- **Slow mode** (default): Skips MPICH's `autogen.sh` with custom autogen, but still uses MPICH's `configure`. After configure, extracts the automake Makefile into a simplified Makefile.
- **Quick mode** (`-quick`): Replaces MPICH's configure entirely with Perl scripts that assume common platform defaults. Generates config headers and Makefiles in seconds.

Both modes use pre-built submodules from `modules.tar.gz` to skip submodule autogen and configure.

## Repository Structure

- `mymake.def` — Main entry point
- `macros_common/` — Shared macros (opts, file I/O patterns)
- `macros_mymake/` — mymake-specific macros (args, config defines, Makefile generation, subdirs, hydra, etc.)
- `macros/` — MPICH-specific macros (bindings, threading, error checking, etc.)
- `study/` — Study/analysis of MPICH internals (init, send/recv, etc.)
- `scripts/` — Code transformation scripts
- `github/` — GitHub/PR management tools
- `toy/` — Toy/test MPI programs
- `mymake/` — Generated output directory
