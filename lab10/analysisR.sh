#!/bin/bash

L=40
STORIES=5000
T=4000

rm -f Rdiffusione.dat

for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9
do
    ./reticular.exe ${i} ${L} ${T} ${STORIES} >> Rdiffusione.dat
done
