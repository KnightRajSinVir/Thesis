# -------------------------------------------------------------------
# User‐configurable paths (can be overridden on the make command line)
# -------------------------------------------------------------------

# Set this to the folder that contains both 'aie/' and 'pl/' subdirectories.
PARENT_DIR ?= /home/rajdeep/Thesis/src/thesis_c/m8

# Derive AIE_DIR, PL_DIR and HOST_SRC from PARENT_DIR
AIE_DIR    := $(PARENT_DIR)/aie
PL_DIR     := $(PARENT_DIR)/pl
HOST_SRC   := $(PARENT_DIR)/host/main.cpp

# Where to put all intermediate and final build artifacts
WORKDIR    := /home/rajdeep/Thesis/work

# Path to the Xilinx Vitis platform file (hard‐coded). Change if needed.
VITIS_PLATFORM := /opt/xilinx/platforms/xilinx_vck5000_gen4x8_qdma_2_202220_1/xilinx_vck5000_gen4x8_qdma_2_202220_1.xpfm

# -------------------------------------------------------------------
# AIE Targets (inlined from aie/Makefile)
# -------------------------------------------------------------------

.PHONY: aie_compile_x86sim aie_compile_hw aie_run_x86sim aie_run_aiesim aie_clean

aie_compile_x86sim:
	@mkdir -p $(AIE_DIR)/work
	@cd $(AIE_DIR)/work && \
	v++ -c --mode aie \
	    --platform=$(VITIS_PLATFORM) \
	    --target=x86sim \
	    --work_dir=x86sim \
	    --config $(AIE_DIR)/aie.cfg \
	    --aie.output-archive=libadf_x86sim.a \
	    $(AIE_DIR)/src/graph.cpp

aie_compile_hw:
	@mkdir -p $(AIE_DIR)/work
	@cd $(AIE_DIR)/work && \
	v++ -c --mode aie \
	    --platform=$(VITIS_PLATFORM) \
	    --target=hw \
	    --work_dir=hw \
	    --config $(AIE_DIR)/aie.cfg \
	    --aie.output-archive=libadf_hw.a \
	    $(AIE_DIR)/src/graph.cpp

aie_run_x86sim:
	@cd $(AIE_DIR)/work && \
	x86simulator --pkg-dir=./x86sim

aie_run_aiesim:
	@cd $(AIE_DIR)/work && \
	aiesimulator --pkg-dir=./hw --dump-vcd=dump --profile

aie_clean:
	@rm -rf $(AIE_DIR)/work
	@rm -rf $(AIE_DIR)/test/a_in.txt $(AIE_DIR)/test/b_in.txt $(AIE_DIR)/test/c_out.txt

# -------------------------------------------------------------------
# PL Targets (inlined from pl/Makefile)
# -------------------------------------------------------------------

.PHONY: pl_csim-mm2s pl_csim-s2mm \
        pl_csynth-mm2s pl_csynth-s2mm \
        pl_cosim-mm2s pl_cosim-s2mm \
        pl_impl-mm2s pl_impl-s2mm \
        pl_clean

pl_csim-mm2s:
	@mkdir -p $(PL_DIR)/work/mm2s
	@cd $(PL_DIR)/work/mm2s && \
	vitis-run --csim --config $(PL_DIR)/mm2s.cfg --work_dir .

pl_csim-s2mm:
	@mkdir -p $(PL_DIR)/work/s2mm
	@cd $(PL_DIR)/work/s2mm && \
	vitis-run --csim --config $(PL_DIR)/s2mm.cfg --work_dir .

pl_csynth-mm2s:
	@mkdir -p $(PL_DIR)/work/mm2s
	@cd $(PL_DIR)/work/mm2s && \
	v++ -c --mode hls --config $(PL_DIR)/mm2s.cfg --work_dir .

pl_csynth-s2mm:
	@mkdir -p $(PL_DIR)/work/s2mm
	@cd $(PL_DIR)/work/s2mm && \
	v++ -c --mode hls --config $(PL_DIR)/s2mm.cfg --work_dir .

pl_cosim-mm2s:
	@cd $(PL_DIR)/work/mm2s && \
	vitis-run --cosim --config $(PL_DIR)/mm2s.cfg --work_dir .

pl_cosim-s2mm:
	@cd $(PL_DIR)/work/s2mm && \
	vitis-run --cosim --config $(PL_DIR)/s2mm.cfg --work_dir .

pl_impl-mm2s:
	@cd $(PL_DIR)/work/mm2s && \
	vitis-run --impl --config $(PL_DIR)/mm2s.cfg --work_dir .

pl_impl-s2mm:
	@cd $(PL_DIR)/work/s2mm && \
	vitis-run --impl --config $(PL_DIR)/s2mm.cfg --work_dir .

pl_clean:
	@rm -rf $(PL_DIR)/work

# -------------------------------------------------------------------
# Testing and Validation Targets
# -------------------------------------------------------------------

.PHONY: aie_test_gen aie_x86sim aie_aiesim aie_validate aie_test_and_validate

# Generate test input files a_in and b_in for AIE 
aie_test_gen:
	$(MAKE) -C $(PARENT_DIR) test_gen

# Run AIE in x86 simulator
.PHONY: aie_x86sim
aie_x86sim:
	$(MAKE) aie_compile_x86sim
	$(MAKE) aie_run_x86sim

# Run AIE in AIE simulator
.PHONY: aie_aiesim
aie_aiesim:
	$(MAKE) aie_compile_hw
	$(MAKE) aie_run_aiesim

aie_validate:
	$(MAKE) -C $(PARENT_DIR) x86simulator_output_validate

aie_hw_validate:
	$(MAKE) -C $(PARENT_DIR) aiesimulator_output_validate
	
# This target pulls in the three steps in sequence
aie_test_and_validate: aie_test_gen aie_x86sim aie_validate

# This target pulls in the three steps in sequence
aie_hw_test_and_validate: aie_test_gen aie_aiesim aie_hw_validate

# -------------------------------------------------------------------
# Top‐level Build Targets
# -------------------------------------------------------------------

# 1. Build AIE graph (produces libadf_hw.a), then copy into WORKDIR
.PHONY: build_aie
build_aie: aie_compile_hw
	@mkdir -p $(WORKDIR)
	@cp $(AIE_DIR)/work/libadf_hw.a $(WORKDIR)/libadf.a

# 2. Synthesize PL kernels (produces .xo files), then copy into WORKDIR
.PHONY: build_pl
build_pl:
	@mkdir -p $(WORKDIR)
	@$(MAKE) pl_csynth-mm2s
	@$(MAKE) pl_csynth-s2mm
	@cp $(PL_DIR)/work/mm2s/mm2s.xo $(WORKDIR)
	@cp $(PL_DIR)/work/s2mm/s2mm.xo $(WORKDIR)

# 3. Link AIE + PL into an .xsa (intermediate hardware archive)
.PHONY: link_hw
link_hw:
	@mkdir -p $(WORKDIR)
	@cd $(WORKDIR) && \
	v++ -l \
	    --platform $(VITIS_PLATFORM) \
	    libadf.a \
	    *.xo \
	    -t hw \
	    --save-temps \
	    -g \
	    --config ../system.cfg \
	    -o hw.xsa

# 4. Package the .xsa into a bootable .xclbin (OSPI‐flash mode)
.PHONY: package_hw
package_hw:
	@cd $(WORKDIR) && \
	v++ -p \
	    -f $(VITIS_PLATFORM) \
	    -t hw \
	    libadf.a \
	    hw.xsa \
	    -o hw.xclbin \
	    --package.boot_mode=ospi

# 5. Compile the host (C++ driver) into a.out
.PHONY: compile_host
compile_host:
	@mkdir -p $(WORKDIR)
	@cd $(WORKDIR) && \
	g++ -std=c++17 \
	    -I$(XILINX_XRT)/include \
	    -L$(XILINX_XRT)/lib \
	    ../$(HOST_SRC) \
	    -lxrt_coreutil \
	    -lpthread

# 6. Run the host executable
.PHONY: run_host
run_host:
	@cd $(WORKDIR) && ./a.out

# -------------------------------------------------------------------
# Convenience targets to run all steps in sequence
# -------------------------------------------------------------------
.PHONY: run_all
run_all: build_pl build_aie link_hw package_hw compile_host run_host

# -------------------------------------------------------------------
# Clean up everything (AIE, PL, and work dir)
# -------------------------------------------------------------------
.PHONY: clean
clean: aie_clean pl_clean
	@rm -rf $(WORKDIR)
	@rm -f c_out_fixed.txt
