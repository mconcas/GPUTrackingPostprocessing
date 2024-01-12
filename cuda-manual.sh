#! /bin/bash -x

source common-config.sh

# ${USE_CHECKER:+compute-sanitizer --tool memcheck --target-processes=all --report-api-errors=all}
# export CUDA_COREDUMP_SHOW_PROGRESS=1
# export CUDA_ENABLE_COREDUMP_ON_EXCEPTION=1
# export ALICEO2_CCDB_LOCALCACHE=$PWD/locCCDB \
# export IGNORE_VALIDITYCHECK_OF_CCDB_LOCALCACHE=1 \
# export USE_CHECKER=

export GPUMEMSIZE=10000000000
ITS_RECOPAR+="GPU_proc.forceMemoryPoolSize=10000000000;GPU_global.deviceType=CUDA"
export GPU_DEVICE=2
o2-ctf-reader-workflow ${GLOSET}  --severity error --max-tf=${MAXTF:-1} --ctf-input data.lst --onlyDet ITS  --allow-missing-detectors  --its-digits --configKeyValues ";;" | \
o2-its-reco-workflow ${GLOSET} --trackerCA --tracking-mode async --disable-mc --digits-from-upstream --use-gpu-workflow --gpu-device=${GPU_DEVICE} ${ITS_RECOPAR} -b --run;

mv its_time_benchmarks.txt its_time_benchmarks_cuda.txt
mv o2trac_its.root o2trac_its_cuda.root
