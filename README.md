# ysyx Project Workspace

This repository is the workbench for “One Student, One Chip” project: NEMU, Navy-apps, AbstractMachine, multiple NPC variants, and supporting tools/labs.

## Layout
```
.
├── abstract-machine/   # bare-metal runtime environment
├── am-kernels/         # runtime kernels and benchmarks
├── chisel/             # Chisel exercises and examples
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
├── ysyx-workbench/     # Nested copy of this repo
├── init.sh             # Subproject init script
└── Makefile            # Root make entry
```

one student one chip homepage: https://ysyx.oscc.cc
