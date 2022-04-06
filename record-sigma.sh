#!/bin/sh

rm error*
rm valid*
for batchsize in 1 550
do
	echo "this is batchsize = ${batchsize}: "
	epoch=10
	while [ "${epoch}" -lt 101 ]
	do
		echo "this is No.${epoch} epoch: "
		i=0
		while [ "$i" -lt 100 ]
		do
		echo "this is No.$i py: "
		python train.py ${batchsize} ${epoch}
		python test.py
		mv error.txt error_$i.txt
		cat error_$i.txt >> error_all_batch_${batchsize}_epoch_${epoch}.txt
		mv valid_error.txt valid_error_$i.txt
		cat valid_error_$i.txt >> valid_error_all_batch_${batchsize}_epoch_${epoch}.txt
		i=`expr $i + 1`
		done
		cp error_all_batch_${batchsize}_epoch_${epoch}.txt ./root/
		cp valid_error_all_batch_${batchsize}_epoch_${epoch}.txt ./root/
		epoch=`expr ${epoch} + 10`
	done
done
# mv error_all.txt epoch_$1_error.txt
# mv valid_error_all.txt epoch_$1_valid_error.txt
# cp epoch_$1_error.txt ./root/
# cp epoch_$1_valid_error.txt ./root/
