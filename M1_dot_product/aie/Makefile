VITIS_PLATFORM=/opt/xilinx/platforms/xilinx_vck5000_gen4x8_qdma_2_202220_1/xilinx_vck5000_gen4x8_qdma_2_202220_1.xpfm

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
