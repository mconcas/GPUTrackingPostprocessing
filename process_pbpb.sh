#! /bin/bash -x

rm -rf *.png

# main processing
./cpu-manual.sh common-config_pbpb.sh
./cuda-manual.sh common-config_pbpb.sh
./hip-manual.sh common-config_pbpb.sh
./gpu-workflow-manual.sh common-config_pbpb.sh

# post processing for tracking output
./postprocessing.sh