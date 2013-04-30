#!/bin/bash

tests=("examples/chain" "examples/dumb_bell_1" "examples/dumb_bell_2" "examples/dumb_bell_3" "examples/test_recommended_1" "examples/test_recommended_2" "examples/test_recommended_3" "examples/test_recommended_4" "examples/mesh1" "examples/mesh2" "examples/ring1" "config/case1" "config/case2" "config/case3" "config/case4")
outputs=("results/res_examples_chain" "results/res_examples_dumb_bell_1" "results/res_examples_dumb_bell_2" "results/res_examples_dumb_bell_3" "results/res_examples_test_recommended_1" "results/res_examples_test_recommended_2" "results/res_examples_test_recommended_3" "results/res_examples_test_recommended_4" "results/mesh1" "results/mesh2" "results/mesh3" "results/res_config_case1" "results/res_config_case2" "results/res_config_case3" "results/res_config_case4")

rm -rf output*
rm -rf *.rcv
for i in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14; do
	echo "Testing ${tests[$i]}"
	./netsim_app -f ${tests[$i]} -t 4 &> output
	cat output | grep -a "Tore down FDTP flow" > output_time
	cat output_time | cut -d "(" -f2 | cut -d ")" -f1 > output_sum
	cat output_sum | gawk 'BEGIN { sum=0; n=0; }{ sum+=$1; n+=1; }END { printf("%f\n%f", sum, sum/n);}' >> output_sum
	#cat output_sum | awk '{s+=$1 n+=1} END {print s}' >> output_sum
	cat output_sum > ${outputs[$i]}
	rm -rf output*
	rm -rf *.rcv

done




