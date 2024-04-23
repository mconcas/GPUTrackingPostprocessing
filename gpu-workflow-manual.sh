#! /bin/bash -x

source common-config.sh

export ROCR_VISIBLE_DEVICES="0"
export GPUMEMSIZE=10000000000

o2-ctf-reader-workflow ${GLOSET} --severity error --max-tf=${MAXTF:-1} --ctf-input data.lst --onlyDet ITS,TPC --allow-missing-detectors --its-digits |
    o2-its-reco-workflow ${GLOSET} --trackerCA ${ITS_TRACKING_MODE} --disable-tracking --disable-mc --digits-from-upstream ${ITS_RECOPAR} -b |
    ${USE_CHECKER:+compute-sanitizer --tool memcheck --target-processes=all --report-api-errors=all} o2-gpu-reco-workflow -b --shm-segment-size 16000000000 \
    --input-type=compressed-clusters-ctf,its-clusters,its-mean-vertex --disable-mc --output-type tracks,clusters,its-tracks --disable-ctp-lumi-request --configKeyValues "${GPU_WORKFLOW_PARS};" | \
    o2-its-track-writer-workflow --disable-mc --run $1 2>&1 | tee gpu-wf.log

mv its_time_benchmarks.txt its_time_benchmarks_gpuwf.txt
mv o2trac_its.root o2trac_its_gpuwf.root
