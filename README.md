# ysyx Project Workspace

This repository is the workbench for the “One Student, One Chip” project and the Ladder RISC-V processor core.

# Guide

To check the **RISC-V architectural emulator**, go to `nemu/`.
To check the **bare-metal runtime environment**, go to `abstract-machine/` and `am-kernels/`.
To check the **processor core**, go to `npc/`.
To check the **simple OS**, go to `nanos-lite/`.
To check the **software running on the OS**, go to `navy-apps/`.

## Layout
```
.
├── abstract-machine/   # bare-metal runtime environment
├── am-kernels/         # bare-metal runtime programs and benchmarks
├── fceux-am/           # NES emulator port
├── nanos-lite/         # Nanos-lite OS
├── navy-apps/          # user-space apps and libs
├── nemu/               # NEMU RISC-V emulator
├── npc/                # NPC (Ladder) CPU project
├── npc-top/            # NPC top-level variant
├── npc-singlecyc/      # NPC single-cycle implementation
├── out/                # build output (generated)
├── project/            # sbt/mill project metadata
├── target/             # sbt/mill build cache
├── init.sh             # subproject init script
└── Makefile            # root make entry
```

one student one chip homepage: https://ysyx.oscc.cc
