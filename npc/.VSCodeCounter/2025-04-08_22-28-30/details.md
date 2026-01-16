# Details

Date : 2025-04-08 22:28:30

Directory d:\\StudyDocuments\\OOOCoreDesign\\ysyx-workbench\\ysyx-workbench\\npc

Total : 67 files,  94973 codes, 1166 comments, 1061 blanks, all 97200 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [.github/workflows/template-cleanup.yml](/.github/workflows/template-cleanup.yml) | YAML | 38 | 7 | 11 | 56 |
| [.github/workflows/test.yml](/.github/workflows/test.yml) | YAML | 25 | 0 | 3 | 28 |
| [Makefile](/Makefile) | Makefile | 40 | 8 | 14 | 62 |
| [README.md](/README.md) | Markdown | 15 | 0 | 9 | 24 |
| [build.sbt](/build.sbt) | Scala | 20 | 1 | 5 | 26 |
| [build.sc](/build.sc) | Scala | 28 | 2 | 2 | 32 |
| [csrc/LadderMain.cpp](/csrc/LadderMain.cpp) | C++ | 536 | 49 | 45 | 630 |
| [csrc/devices.cpp](/csrc/devices.cpp) | C++ | 93 | 17 | 19 | 129 |
| [csrc/devices.h](/csrc/devices.h) | C++ | 5 | 0 | 1 | 6 |
| [csrc/disasm.cc](/csrc/disasm.cc) | C++ | 82 | 41 | 14 | 137 |
| [csrc/log.cpp](/csrc/log.cpp) | C++ | 9 | 0 | 3 | 12 |
| [csrc/log.h](/csrc/log.h) | C++ | 11 | 0 | 1 | 12 |
| [csrc/memory.cpp](/csrc/memory.cpp) | C++ | 89 | 21 | 18 | 128 |
| [csrc/memory.h](/csrc/memory.h) | C++ | 9 | 0 | 1 | 10 |
| [genrtl/Ladder.anno.json](/genrtl/Ladder.anno.json) | JSON | 17,516 | 0 | 0 | 17,516 |
| [genrtl/ysyx\_22041413.v](/genrtl/ysyx_22041413.v) | Verilog | 72,226 | 29 | 1 | 72,256 |
| [project/build.properties](/project/build.properties) | Properties | 1 | 0 | 1 | 2 |
| [project/plugins.sbt](/project/plugins.sbt) | Scala | 1 | 0 | 1 | 2 |
| [src/test/scala/ladder/CPU\_top.scala](/src/test/scala/ladder/CPU_top.scala) | Scala | 41 | 5 | 8 | 54 |
| [src/test/scala/ladder/back\_end/arch\_regfile.scala](/src/test/scala/ladder/back_end/arch_regfile.scala) | Scala | 23 | 0 | 4 | 27 |
| [src/test/scala/ladder/back\_end/commit/rob.scala](/src/test/scala/ladder/back_end/commit/rob.scala) | Scala | 180 | 54 | 33 | 267 |
| [src/test/scala/ladder/back\_end/csr.scala](/src/test/scala/ladder/back_end/csr.scala) | Scala | 95 | 9 | 11 | 115 |
| [src/test/scala/ladder/back\_end/decoder.scala](/src/test/scala/ladder/back_end/decoder.scala) | Scala | 116 | 17 | 11 | 144 |
| [src/test/scala/ladder/back\_end/dispatch/dispatch.scala](/src/test/scala/ladder/back_end/dispatch/dispatch.scala) | Scala | 41 | 18 | 8 | 67 |
| [src/test/scala/ladder/back\_end/execute/execute.scala](/src/test/scala/ladder/back_end/execute/execute.scala) | Scala | 133 | 31 | 35 | 199 |
| [src/test/scala/ladder/back\_end/execute/execute\_unit.scala](/src/test/scala/ladder/back_end/execute/execute_unit.scala) | Scala | 467 | 177 | 84 | 728 |
| [src/test/scala/ladder/back\_end/execute/mul\_div/booth.scala](/src/test/scala/ladder/back_end/execute/mul_div/booth.scala) | Scala | 58 | 1 | 6 | 65 |
| [src/test/scala/ladder/back\_end/execute/mul\_div/div.scala](/src/test/scala/ladder/back_end/execute/mul_div/div.scala) | Scala | 105 | 2 | 18 | 125 |
| [src/test/scala/ladder/back\_end/execute/mul\_div/mul.scala](/src/test/scala/ladder/back_end/execute/mul_div/mul.scala) | Scala | 73 | 9 | 13 | 95 |
| [src/test/scala/ladder/back\_end/execute/mul\_div/wallace\_tree.scala](/src/test/scala/ladder/back_end/execute/mul_div/wallace_tree.scala) | Scala | 129 | 11 | 23 | 163 |
| [src/test/scala/ladder/back\_end/interrupt\_mask.scala](/src/test/scala/ladder/back_end/interrupt_mask.scala) | Scala | 31 | 4 | 4 | 39 |
| [src/test/scala/ladder/back\_end/issue/reservation\_station.scala](/src/test/scala/ladder/back_end/issue/reservation_station.scala) | Scala | 100 | 72 | 26 | 198 |
| [src/test/scala/ladder/back\_end/issue/reservation\_station\_slot.scala](/src/test/scala/ladder/back_end/issue/reservation_station_slot.scala) | Scala | 121 | 7 | 27 | 155 |
| [src/test/scala/ladder/back\_end/regfile.scala](/src/test/scala/ladder/back_end/regfile.scala) | Scala | 54 | 2 | 12 | 68 |
| [src/test/scala/ladder/back\_end/rename/busy\_table.scala](/src/test/scala/ladder/back_end/rename/busy_table.scala) | Scala | 71 | 44 | 20 | 135 |
| [src/test/scala/ladder/back\_end/rename/free\_list.scala](/src/test/scala/ladder/back_end/rename/free_list.scala) | Scala | 92 | 27 | 159 | 278 |
| [src/test/scala/ladder/back\_end/rename/rename.scala](/src/test/scala/ladder/back_end/rename/rename.scala) | Scala | 62 | 19 | 100 | 181 |
| [src/test/scala/ladder/back\_end/rename/rename\_table.scala](/src/test/scala/ladder/back_end/rename/rename_table.scala) | Scala | 62 | 9 | 12 | 83 |
| [src/test/scala/ladder/backend\_with\_decode.scala](/src/test/scala/ladder/backend_with_decode.scala) | Scala | 131 | 26 | 25 | 182 |
| [src/test/scala/ladder/cache/AxiLite.scala](/src/test/scala/ladder/cache/AxiLite.scala) | Scala | 46 | 2 | 14 | 62 |
| [src/test/scala/ladder/cache/S011HD1P\_dpi.scala](/src/test/scala/ladder/cache/S011HD1P_dpi.scala) | Scala | 18 | 0 | 3 | 21 |
| [src/test/scala/ladder/cache/axi\_arbiter.scala](/src/test/scala/ladder/cache/axi_arbiter.scala) | Scala | 79 | 42 | 5 | 126 |
| [src/test/scala/ladder/cache/data\_array.scala](/src/test/scala/ladder/cache/data_array.scala) | Scala | 58 | 1 | 6 | 65 |
| [src/test/scala/ladder/cache/dcache.scala](/src/test/scala/ladder/cache/dcache.scala) | Scala | 239 | 36 | 17 | 292 |
| [src/test/scala/ladder/cache/icache.scala](/src/test/scala/ladder/cache/icache.scala) | Scala | 104 | 25 | 7 | 136 |
| [src/test/scala/ladder/consts.scala](/src/test/scala/ladder/consts.scala) | Scala | 94 | 25 | 11 | 130 |
| [src/test/scala/ladder/defs.scala](/src/test/scala/ladder/defs.scala) | Scala | 125 | 86 | 26 | 237 |
| [src/test/scala/ladder/dpic-axi-ver.scala](/src/test/scala/ladder/dpic-axi-ver.scala) | Scala | 65 | 1 | 6 | 72 |
| [src/test/scala/ladder/dpic.scala](/src/test/scala/ladder/dpic.scala) | Scala | 56 | 1 | 5 | 62 |
| [src/test/scala/ladder/front\_end/branch\_predictor.scala](/src/test/scala/ladder/front_end/branch_predictor.scala) | Scala | 91 | 28 | 10 | 129 |
| [src/test/scala/ladder/front\_end/branch\_presolve.scala](/src/test/scala/ladder/front_end/branch_presolve.scala) | Scala | 47 | 8 | 3 | 58 |
| [src/test/scala/ladder/front\_end/fetch\_queue.scala](/src/test/scala/ladder/front_end/fetch_queue.scala) | Scala | 27 | 0 | 4 | 31 |
| [src/test/scala/ladder/front\_end/fetchres.scala](/src/test/scala/ladder/front_end/fetchres.scala) | Scala | 39 | 12 | 6 | 57 |
| [src/test/scala/ladder/front\_end/front\_end.scala](/src/test/scala/ladder/front_end/front_end.scala) | Scala | 66 | 12 | 19 | 97 |
| [src/test/scala/ladder/front\_end/front\_end\_control.scala](/src/test/scala/ladder/front_end/front_end_control.scala) | Scala | 29 | 3 | 6 | 38 |
| [src/test/scala/ladder/front\_end/if1\_if2.scala](/src/test/scala/ladder/front_end/if1_if2.scala) | Scala | 31 | 1 | 7 | 39 |
| [src/test/scala/ladder/front\_end/if2\_if3.scala](/src/test/scala/ladder/front_end/if2_if3.scala) | Scala | 31 | 0 | 6 | 37 |
| [src/test/scala/ladder/front\_end/pc\_gen.scala](/src/test/scala/ladder/front_end/pc_gen.scala) | Scala | 37 | 8 | 8 | 53 |
| [src/test/scala/ladder/insts.scala](/src/test/scala/ladder/insts.scala) | Scala | 156 | 9 | 9 | 174 |
| [src/test/scala/ladder/ladder.scala](/src/test/scala/ladder/ladder.scala) | Scala | 17 | 53 | 4 | 74 |
| [src/test/scala/ladder/muldiv\_test.scala](/src/test/scala/ladder/muldiv_test.scala) | Scala | 36 | 7 | 11 | 54 |
| [src/test/scala/ladder/soc\_top.scala](/src/test/scala/ladder/soc_top.scala) | Scala | 91 | 40 | 17 | 148 |
| [verilogtestbench/front\_end\_tb.v](/verilogtestbench/front_end_tb.v) | Verilog | 100 | 4 | 14 | 118 |
| [verilogtestbench/tb.v](/verilogtestbench/tb.v) | Verilog | 92 | 25 | 18 | 135 |
| [verilogtestbench/top\_tb.v](/verilogtestbench/top_tb.v) | Verilog | 70 | 2 | 13 | 85 |
| [vsrc/S011HD1P\_X32Y2D128\_BW.v](/vsrc/S011HD1P_X32Y2D128_BW.v) | Verilog | 25 | 0 | 4 | 29 |
| [vsrc/dpi-funcs\_axi\_ver.v](/vsrc/dpi-funcs_axi_ver.v) | Verilog | 175 | 16 | 14 | 205 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)