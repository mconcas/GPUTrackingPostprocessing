#! /bin/bash -x

# main processing
./cpu-manual.sh common-config_pp.sh
./cuda-manual.sh common-config_pp.sh
./hip-manual.sh common-config_pp.sh
./gpu-workflow-manual.sh common-config_pp.sh

# post processing for tracking output
./postprocessing.sh