#!/bin/bash

cat output | grep -a "Tore down FDTP flow" > output_time
	cat output_time | cut -d "(" -f2 | cut -d ")" -f1 > output_sum
	cat output_sum | gawk 'BEGIN { sum=0; n=0; }{ sum+=$1; n+=1; }END { printf("%f\n%f", sum, sum/n);}' >> output_sum
	#cat output_sum | awk '{s+=$1 n+=1} END {print s}' >> output_sum
	cat output_sum > ${outputs[$i]}
