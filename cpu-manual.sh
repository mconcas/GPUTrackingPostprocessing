#! /bin/bash -x

source common-config.sh

o2-ctf-reader-workflow ${GLOSET}  --severity error --max-tf=${MAXTF:-1} --ctf-input data.lst --onlyDet ITS  --allow-missing-detectors  --its-digits --configKeyValues ";;" | \
o2-its-reco-workflow ${GLOSET} --trackerCA --tracking-mode async --disable-mc --digits-from-upstream ${ITS_RECOPAR} -b --run $12>&1 | sed 's/\[[^]]*:[^]]*\]//g' | tee cpu.log

mv its_time_benchmarks.txt its_time_benchmarks_cpu.txt
mv o2trac_its.root o2trac_its_cpu.root
