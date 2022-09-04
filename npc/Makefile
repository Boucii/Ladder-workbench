BUILD_DIR = ./build

VSRCS = $(shell find $(abspath ./build) -name "*.v") $(shell find $(abspath ./sim) -name "*.v")
CSRCS = $(shell find $(abspath ./sim) -name "*.c" -or -name "*.cc" -or -name "*.cpp")


test:
	mill -i __.test

verilog:
	$(call git_commit, "generate verilog")
	mkdir -p $(BUILD_DIR)
	mill -i __.test.runMain cpu2.GenVerilog -td $(BUILD_DIR)

help:
	mill -i __.test.runMain cpu2.GenVerilog --help

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

.PHONY: test verilog help compile bsp reformat checkformat clean genwave sim wave debug

CXXFLAGS += $(shell llvm-config-14 --cxxflags) 
CXXFLAGS += -g
LDFLAGS += $(shell llvm-config-14 --ldflags) 
LDFLAGS += $(shell llvm-config-14 --libs all)


sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
#	@echo "Write this Makefile by yourself."
	verilator --cc --exe --build --trace -CFLAGS "$(CXXFLAGS)" -LDFLAGS "$(LDFLAGS)" -Isim/ $(CSRCS) $(VSRCS) --Mdir $(BUILD_DIR)/SimGen 
wave:
	#@echo "Hello from npc"
	@echo $(IMAGE)
	./build/SimGen/VTOP $(IMAGE)
	gtkwave wave.vcd

debug:
	#@echo "Hello from npc"
	@echo $(IMAGE)
	gdb  -tui --args ./build/SimGen/VTOP $(IMAGE) 


include ../Makefile
