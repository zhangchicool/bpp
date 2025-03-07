#!/bin/bash

rm coal_A^a1_C^c1
rm coal_A^a1_C^c5
rm coal_A^a1_A^a2
rm coal_A^a1_A^a3
rm coal_C^c1_C^c2
rm coal_A^a3_B^b2
# For each batch (this is naming for tree simualation)
for ((gene=1;gene<=10;gene++));
do
	../../../testCode/./compare_tree out.txt.gtree.L${gene} A^a1 C^c1 A^a1 C^c5 A^a1 A^a2 A^a1 A^a3 C^c1 C^c2 A^a3 B^b2 > coal_${gene} &
	#../testCode/./compare_tree out2.txt.gtree.L${gene} A^a1 C^c1 A^a1 C^c5 A^a1 A^a2 A^a1 A^a3 C^c1 C^c2 A^a3 B^b2 > coal2_${gene} &
	#../testCode/./compare_tree out2.txt.gtree.L${gene} B^b1 B^b2 B^b1 B^b3 > coal3_${gene} &
	#../testCode/./compare_tree out.txt.gtree.L${gene} A^a1 C^c1 > coal_a1_c1_${gene} & 
	#../testCode/./compare_tree out.txt.gtree.L${gene} A^a1 C^c5 > coal_a1_c5_${gene} & 
	#../testCode/./compare_tree out.txt.gtree.L${gene} A^a1 A^a2 > coal_a1_a2_${gene} & 
	#../testCode/./compare_tree out.txt.gtree.L${gene} A^a1 A^a3 > coal_a1_a3_${gene} & 
	#../testCode/./compare_tree out.txt.gtree.L${gene} C^c1 C^c2 > coal_c1_c2_${gene} & 
	#../testCode/./compare_tree out.txt.gtree.L${gene} A^a3 B^b2 > coal_a3_b2_${gene} & 

	#../testCode/./compare_tree out2.txt.gtree.L${gene} A^a1 A^a3 > coal2_a1_a3_${gene} & 

	wait

	grep "A^a1_C^c1" coal_${gene} |awk '{print $2}' >> coal_A^a1_C^c1
	grep "A^a1_C^c5" coal_${gene} |awk '{print $2}' >> coal_A^a1_C^c5
	grep "A^a1_A^a2" coal_${gene} |awk '{print $2}' >> coal_A^a1_A^a2
	grep "A^a1_A^a3" coal_${gene} |awk '{print $2}' >> coal_A^a1_A^a3
	grep "C^c1_C^c2" coal_${gene} |awk '{print $2}' >> coal_C^c1_C^c2
	grep "A^a3_B^b2" coal_${gene} |awk '{print $2}' >> coal_A^a3_B^b2
	
	#grep "A^a1_C^c1" coal2_${gene} |awk '{print $2}' >> coal2_a1_c1
	#grep "A^a1_C^c5" coal2_${gene} |awk '{print $2}' >> coal2_a1_c5
	#grep "A^a1_A^a2" coal2_${gene} |awk '{print $2}' >> coal2_a1_a2
	#grep "A^a1_A^a3" coal2_${gene} |awk '{print $2}' >> coal2_a1_a3
	#grep "C^c1_C^c2" coal2_${gene} |awk '{print $2}' >> coal2_c1_c2
	#grep "A^a3_B^b2" coal2_${gene} |awk '{print $2}' >> coal2_a3_b2
	#grep "B^b1_B^b2" coal3_${gene} |awk '{print $2}' >> coal3_b1_b2
	#grep "B^b1_B^b3" coal3_${gene} |awk '{print $2}' >> coal3_b1_b3
done
wait 

exit
	Rscript ../testCode/reformatTimes.R coal3_b1_b2 b1_b2_mcmc3
	sed -i 's/,/\t/g' b1_b2_mcmc3
