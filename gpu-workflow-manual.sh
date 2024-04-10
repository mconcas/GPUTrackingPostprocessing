#! /bin/bash -x

source common-config.sh

export ROCR_VISIBLE_DEVICES="0"

export GPUMEMSIZE=10000000000
ITS_RECOPAR+="GPU_proc.forceMemoryPoolSize=10000000000;GPU_global.deviceType=HIP"
export GPU_DEVICE=3

o2-ctf-reader-workflow ${GLOSET} --severity error --max-tf=${MAXTF:-1} --ctf-input data.lst --onlyDet ITS --allow-missing-detectors --its-digits --configKeyValues ";;" | \
o2-its-reco-workflow ${GLOSET}  --disable-mc --digits-from-upstream ${ITS_CLUSTERPAR} -b --disable-tracking | \
o2-gpu-reco-workflow --session default --severity info --shm-segment-size 16000000000 --input-type=compressed-clusters-ctf,its-clusters --disable-mc --output-type tracks,clusters,tpc-triggers,its-tracks  --disable-ctp-lumi-request --configKeyValues "GPU_global.deviceType=CUDA;GPU_proc.forceMemoryPoolSize=8000000000;GPU_proc.forceHostMemoryPoolSize=1073741824;;;;"
