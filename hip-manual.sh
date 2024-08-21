#! /bin/bash -x

source $1

export ROCR_VISIBLE_DEVICES="0"

export GPUMEMSIZE=10000000000
ITS_RECOPAR+="GPU_proc.forceMemoryPoolSize=10000000000;GPU_global.deviceType=HIP"
export GPU_DEVICE=3

o2-ctf-reader-workflow ${GLOSET} --severity error --max-tf=${MAXTF:-1} --ctf-input data.lst --onlyDet ITS --allow-missing-detectors --its-digits --configKeyValues ";;" |
    o2-its-reco-workflow ${GLOSET} --trackerCA --tracking-mode sync --disable-mc --digits-from-upstream --use-gpu-workflow --gpu-device=${GPU_DEVICE} ${ITS_RECOPAR} -b --run 2>&1 | sed 's/\[[^]]*:[^]]*\]//g' | tee hip.log

mv its_time_benchmarks.txt its_time_benchmarks_hip.txt
mv o2trac_its.root o2trac_its_hip.root