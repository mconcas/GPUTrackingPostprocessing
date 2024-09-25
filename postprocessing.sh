#! /usr/bin/bash -x

# post processing for tracking output
root -l -q compareTracks.C+

# post processing for benchmark output
./parselog.py cpu.log
./parselog.py cuda.log
./parselog.py hip.log
./parselog.py gpu-wf.log

gnuplot plot.gp

root -q -l doBenchmarkAnalysis.C+\(\"its_time_benchmarks_cpu\"\) >>cpu.log
root -q -l doBenchmarkAnalysis.C+\(\"its_time_benchmarks_cuda\"\) >>cuda.log
root -q -l doBenchmarkAnalysis.C+\(\"its_time_benchmarks_hip\"\) >>hip.log
root -q -l doBenchmarkAnalysis.C+\(\"its_time_benchmarks_gpuwf\"\) >>gpu-wf.log

tail -n 21 *.log | grep -v "Processing " | grep -v "INFO" >Summary.md

# notification for completion  + results
tgprint "Processing completed."
tgup *.png
