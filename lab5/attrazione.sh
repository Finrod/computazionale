#!/bin/bash


pi=`echo "h=10;4*a(1)" | bc -l`
start=-$pi
dx=0.00628318531


rm -f attrazione.dat

for((x=-3.14; x<$pi; x= x+dx)) ; do
    for((v=-$pi; v<$pi; v= v+dx)) ; do
	./attrazione.exe ${x} ${v} >> attrazione.dat
    done
done



#echo ${dx}

#rm -f attrazione.dat

#./attrazione.exe ${a} ${b} >> attrazione.dat


#for ((i=0; i<=20; i++)) ; do
 #  x+=${i}*${dx}
  #  echo ${x}
   # for ((j=0; j<=20; j++)) ; do
#	v+=${j}*${dx}
#	./attrazione.exe ${x} ${v} >> attrazione.dat
#   done
#done
