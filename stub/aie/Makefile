VITIS_PLATFORM=xilinx-vck5000-gen4x8-qdma-base-2

compile_x86sim: work/libadf_x86sim.a
	mkdir -p work
	cd work && \
	aiecompiler \
		--platform=$(VITIS_PLATFORM) \
		--target=x86sim \
		--pl-freq=$(PL_FREQ) \
		--include=../src \
		--workdir=x86sim \
		--output-archive=libadf_x86sim.a \
		../src/*.cpp

compile_hw: work/libadf_hw.a
	mkdir -p work
	cd work && \
	aiecompiler \
        --platform=$(VITIS_PLATFORM) \
        --target=hw \
        --pl-freq=$(PL_FREQ) \
        --include={{.TASKFILE_DIR}}/src \
        --workdir=hw \
        --output-archive=libadf_hw.a \
        --xlopt=2 \
        --verbose \
        ../src/*.cpp \

run_x86sim: compile_x86sim
	cd work && \
	x86simulator --pkg-dir=./x86sim

run_aiesim: compile_hw
	cd work && \
	aiesimulator --pkg-dir=./hw --dump-vcd=dump --profile

clean:
	rm -rf work