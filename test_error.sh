#!bin/bash

rm happy.txt
rm happy1.txt
rm error_epoch_100_test.txt
rm error_epoch_100_train.txt

for batch in 10 20 40 80
do

epoch=0
epoch1=0
cat error_batch_${batch}.txt | while read line
do

epoch=`expr $epoch + 10`
if [ $epoch == 100 ]
then
echo "$batch  $line" >> happy.txt
fi

done
cat valid_error_batch_${batch}.txt | while read line
do

epoch1=`expr $epoch1 + 10`
if [ $epoch1 == 100 ]
then
echo "$batch  $line" >> happy1.txt
fi

done

done

sed 's/100  //g' happy.txt >> error_epoch_100_test.txt
sed 's/100  //g' happy1.txt >> error_epoch_100_train.txt
