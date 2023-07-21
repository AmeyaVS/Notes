IVERILOG_TOOL := iverilog
# Uncomment following line to enable Iverilog Warnings like in this current example
override IVERILOG_FLAGS += -Wall  # or -Wmacro-redefinition
VERILATOR_TOOL := verilator
override VERILATOR_FLAGS += --cc

all: inorder.out reverseorder.out

inorder.out: 1.v 2.v
	$(IVERILOG_TOOL) $(IVERILOG_FLAGS) $^ -o $@

reverseorder.out: 2.v 1.v
	$(IVERILOG_TOOL) $(IVERILOG_FLAGS) $^ -o $@

inorderverilate: 1.v 2.v
	$(VERILATOR_TOOL) $(VERILATOR_FLAGS) $^

reverseorderverilate: 2.v 1.v
	$(VERILATOR_TOOL) $(VERILATOR_FLAGS) $^

clean:
	rm -rfv inorder.out reverseorder.out