#! /bin/bash -x

# main processing
./cpu-manual.sh
./cuda-manual.sh
./hip-manual.sh

# post processing for tracking output
./postprocessing.sh
