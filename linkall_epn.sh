#! /usr/bin/bash -xe

ln -s $1/process_pbpb.sh .
ln -s $1/common-config_pbpb.sh .
ln -s $1/cpu-manual.sh .
ln -s $1/hip-manual.sh .
ln -s $1/gpu-workflow-manual.sh .
ln -s $1/doBenchmarkAnalysis.C .
ln -s $1/plot.gp .
ln -s $1/process_file.C .
ln -s $1/compareTracks.C .
ln -s $1/parselog.py .
ln -s $1/postprocessing.sh .
