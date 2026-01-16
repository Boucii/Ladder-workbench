# ysyx Project Workspace

This repository is the workbench for “One Student, One Chip” project, and workbench for Ladder RISC-V processor core.

# Guide

This repository 
to check the **RISC-V architectural emulator**, go to nemu/

to check the **bare-metal runtime environment**, go to abstract-machine/ and am-kernels/

to check the **Processor core**, go to npc/

to check the **simple OS**, go to nanos-lite/

to check the **softwares running on the OS**, go to navy-apps/ 

## Layout
```
.
├── abstract-machine/   # bare-metal runtime environment
├── am-kernels/         # bare-metal runtime programs and benchmarks
├── fceux-am/           # NES emulator port
├── nanos-lite/         # Nanos-lite OS
├── nanos-lite22/       # Nanos-lite 2022 variant
├── navy-apps/          # User-space apps and libs
├── nemu/               # NEMU RISC-V emulator
├── npc/                # NPC (Ladder) CPU project
├── npc-top/            # NPC top-level variant
├── npc-singlecyc/      # NPC single-cycle implementation
├── nvboard/            # FPGA I/O board support
├── veriloatortest/     # Verilator samples
├── init.sh             # Subproject init script
└── Makefile            # Root make entry
├── chisel/             # Chisel exercises and examples
```

one student one chip homepage: https://ysyx.oscc.cc
