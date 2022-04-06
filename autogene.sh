#!/bin/bash

rm error_batch*.txt
rm valid_error_batch*.txt
for batchsize in 10 20 40 80 
do
	epoch=10
	echo "batchsize: ${batchsize}"
	while [ "${epoch}" -lt 101 ]
	do
	echo "this is for epoch ${epoch}:"
	mv error_all_batch_${batchsize}_epoch_${epoch}.txt std_epoch_error.txt
	mv valid_error_all_batch_${batchsize}_epoch_${epoch}.txt std_epoch_valid_error.txt
	echo "${epoch}  \c" >> error.txt
	echo "${epoch}  \c" >> valid_error.txt
	root -l -q txt2root.C
	mv std_epoch_error.txt error_all_batch_${batchsize}_epoch_${epoch}.txt
	mv std_epoch_valid_error.txt valid_error_all_batch_${batchsize}_epoch_${epoch}.txt
	epoch=`expr ${epoch} + 10`
	done
	mv error.txt error_batch_${batchsize}.txt
	mv valid_error.txt valid_error_batch_${batchsize}.txt
done
root -l draw_error.C
