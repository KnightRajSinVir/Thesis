.PHONY: test_gen x86simulator_output_validate

test_gen:
	python3 ./aie/test/test_gen.py

x86simulator_output_validate:
	python3 strip_timestamps.py \
		aie/work/x86simulator_output/test/x3_out.txt \
		aie/work/x86simulator_output/test/x3_out_cleaned.txt

	python3 validate.py \
		aie/test/x3_output.txt \
		aie/test/y3_output.txt \
		aie/work/x86simulator_output/test/x3_out_cleaned.txt \
		aie/work/x86simulator_output/test/y3_out.txt


aiesimulator_output_validate:
	python3 strip_timestamps.py \
		aie/work/aiesimulator_output/test/x3_out.txt \
		aie/work/aiesimulator_output/test/x3_out_cleaned.txt

	python3 strip_timestamps.py \
		aie/work/aiesimulator_output/test/y3_out.txt \
		aie/work/aiesimulator_output/test/y3_out_cleaned.txt

	python3 validate.py \
		aie/test/x3_output.txt \
		aie/test/y3_output.txt \
		aie/work/aiesimulator_output/test/x3_out_cleaned.txt \
		aie/work/aiesimulator_output/test/y3_out_cleaned.txt