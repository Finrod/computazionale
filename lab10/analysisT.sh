#!/bin/bash

POINTS=50
RHO=0.6
L=80
STORIES=3000
STEP=20



for((i=1; i<=${POINTS}; i++)) ; do
    let T=${i}*${STEP}
    ./reticular.exe ${RHO} ${L} ${T} ${STORIES} >> dati/L80_R06.dat
done
