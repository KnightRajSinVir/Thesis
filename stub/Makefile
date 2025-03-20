VITIS_PLATFORM=/opt/xilinx/platforms/xilinx_vck5000_gen4x8_qdma_2_202220_1/xilinx_vck5000_gen4x8_qdma_2_202220_1.xpfm

build_aie:
	mkdir -p work
	make -C aie compile_hw
	cp aie/work/libadf_hw.a work/libadf.a

build_pl:
	mkdir -p work
	make -C pl csynth
	cp pl/work/mm2s/mm2s.xo work
	cp pl/work/s2mm/s2mm.xo work

link_hw:
	mkdir -p work
	cd work && \
	v++ -l --platform $(VITIS_PLATFORM) libadf.a *.xo -t hw --save-temps -g --config ../system.cfg -o hw.xsa

package_hw:
	cd work && \
	v++ -p -f $(VITIS_PLATFORM) -t hw hw.xsa -o hw.xclbin --package.boot_mode=ospi

clean:
	make -C aie clean
	make -C pl clean
	rm -rf work
