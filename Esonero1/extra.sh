#!/bin/bash

n=3
r0=0.01
P0=1
D0=0
dr=0.01
rmax=100

rm -f extra.dat

STEPS="0.01, 0.009, 0.005, 0.003, 0.001"

for STEP in ${STEPS} ; do
    ./extra.exe ${n} ${r0} ${P0} ${D0} ${STEP} ${rmax} >> extra.dat
done
