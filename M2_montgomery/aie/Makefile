VITIS_PLATFORM=/opt/xilinx/platforms/xilinx_vck5000_gen4x8_qdma_2_202220_1/xilinx_vck5000_gen4x8_qdma_2_202220_1.xpfm

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  compile_x86sim  : Compile AI Engine code for x86 simulator"
	@echo "  compile_hw      : Compile AI Engine code for hardware"
	@echo "  run_x86sim      : Run x86 simulator"
	@echo "  run_aiesim      : Run AI Engine simulator"
	@echo "  clean           : Remove temporary/work directories"
	@echo ""

compile_x86sim:
	mkdir -p work
	cd work && \
		v++ -c --mode aie \
			--platform=$(VITIS_PLATFORM) \
			--target=x86sim \
			--work_dir=x86sim \
			--config ../aie.cfg \
			--aie.output-archive=libadf_x86sim.a \
			../src/graph.cpp

compile_hw:
	mkdir -p work
	cd work && \
		v++ -c --mode aie \
			--platform=$(VITIS_PLATFORM) \
			--target=hw \
			--work_dir=hw \
			--config=../aie.cfg \
			--aie.output-archive=libadf_hw.a \
			../src/graph.cpp

run_x86sim:
	cd work && \
		x86simulator --pkg-dir=./x86sim

run_aiesim:
	cd work && \
		aiesimulator --pkg-dir=./hw --dump-vcd=dump --profile

clean:
	rm -rf work
