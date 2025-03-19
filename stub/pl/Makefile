COMPONENTS=mm2s s2mm

csim: $(COMPONENTS:%=csim-%)
csynth: $(COMPONENTS:%=csynth-%)
cosim: $(COMPONENTS:%=cosim-%)
impl: $(COMPONENTS:%=impl-%)

csim-%:
        mkdir -p build/$*
        cd build/$*; vitis-run --csim --config ../../$*.cfg --work_dir .

csynth-%:
        mkdir -p build/$*
        cd build/$*; v++ -c --mode hls --config ../../$*.cfg --work_dir .

cosim-%:
        cd build/$*; vitis-run --cosim --config ../../$*.cfg --work_dir .

impl-%:
        cd build/$*; vitis-run --impl --config ../../$*.cfg --work_dir .

clean:
        rm -rf ./build