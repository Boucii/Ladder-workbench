BUILD_DIR = ./build

VSRCS = $(shell find $(abspath ./build) -name "*.v") $(shell find $(abspath ./sim) -name "*.v")
CSRCS = $(shell find $(abspath ./sim) -name "*.c" -or -name "*.cc" -or -name "*.cpp")


test:
	mill -i __.test

verilog:
	$(call git_commit, "generate verilog")
	mkdir -p $(BUILD_DIR)
	mill -i __.test.runMain cpu.GenVerilog -td $(BUILD_DIR)

help:
	mill -i __.test.runMain cpu.GenVerilog --help

compile:
	mill -i __.compile

bsp:
	mill -i mill.bsp.BSP/install

reformat:
	mill -i __.reformat

checkformat:
	mill -i __.checkFormat

clean:
	-rm -rf $(BUILD_DIR)

genwave:
	verilator --cc --exe --trace --build -CFLAGS -g  -Isim/ $(CSRCS) $(VSRCS) 

.PHONY: test verilog help compile bsp reformat checkformat clean genwave sim

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
#	@echo "Write this Makefile by yourself."
	verilator --cc --exe --build -CFLAGS -g  -Isim/ $(CSRCS) $(VSRCS) --Mdir $(BUILD_DIR)./SimGen #sim/memory.cpp sim/main.cpp build/TOP.v

include ../Makefile
