VITIS_PLATFORM=/opt/xilinx/platforms/xilinx_vck5000_gen4x8_qdma_2_202220_1/xilinx_vck5000_gen4x8_qdma_2_202220_1.xpfm

COMPONENTS=mm2s s2mm

csim: $(COMPONENTS:%=csim-%)
csynth: $(COMPONENTS:%=csynth-%)
cosim: $(COMPONENTS:%=cosim-%)
impl: $(COMPONENTS:%=impl-%)

csim-%:
	mkdir -p work/$*
	cd work/$*; vitis-run --csim --config ../../$*.cfg --work_dir .

csynth-%:
	mkdir -p work/$*
	cd work/$*; v++ -c --mode hls --config ../../$*.cfg --work_dir .

cosim-%:
	cd work/$*; vitis-run --cosim --config ../../$*.cfg --work_dir .

impl-%:
	cd work/$*; vitis-run --impl --config ../../$*.cfg --work_dir .

clean:
	rm -rf ./work