#! /bin/bash -x

./cpu-manual.sh 2>&1 > cpu.log
./cuda-manual.sh 2>&1 > cuda.log
./hip-manual.sh 2>&1 > hip.log

root -q compareTracks.C+

gnuplot plot.gp

tgup *.png
