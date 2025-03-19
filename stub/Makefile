build_aie: work/libadf_hw.a
	mkdir -p work
	make -C aie compile_hw
	cp aie/work/libadf_hw.a work

build_pl: work/mm2s.xo work/s2mm.xo
	mkdir -p work
	make -C pl csynth
	cp pl/work/mm2s/mm2s.xo work
	cp pl/work/mm2s/s2mm.xo work

link_hw: work/hw.xsa
	mkdir -p work
	cd work && \
	v++ -l --platform $(VITIS_PLATFORM) *.xo -t hw --save-temps -g --config ../system.cfg -o hw.xsa

package_hw: work/hw.xclbin
	cd work && \
	v++ -p -f $(VITIS_PLATFORM) -t hw hw.xsa -o hw.xclbin --package.boot_mode=ospi

clean:
	make -C aie clean
	make -C pl clean
	rm -rf work