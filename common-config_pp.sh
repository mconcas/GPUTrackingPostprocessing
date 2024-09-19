#!/bin/bash -x

# Common configurations and commands
rm -f /dev/shm/*fmq*;
killall o2-its-reco-workflow -s 9;
killall o2-ctf-reader-workflow -s 9;

MODE=async
MODE_NUM=
[ $MODE == "sync" ] && MODE_NUM=0 || MODE_NUM=1
ITS_RECOPAR="--configKeyValues "
MAXTF=2;
GLOSET="--shm-segment-size 16000000000";

# Define ConfigKeyValues
ITS_CLUSTERPAR=";ITSClustererParam.maxBCDiffToMaskBias=-10;ITSClustererParam.maxBCDiffToSquashBias=10;"
FASTMULTPAR=";fastMultConfig.cutMultClusLow=-1;fastMultConfig.cutMultClusHigh=-1;fastMultConfig.cutMultVtxHigh=-1;"

# Cuts for pp specifically
ITS_TRKPAR="ITSGpuTrackingParam.nBlocks=20;ITSGpuTrackingParam.nThreads=256;ITSCATrackerParam.useFastMaterial=false;ITSCATrackerParam.nThreads=1;ITSVertexerParam.nThreads=1;ITSCATrackerParam.saveTimeBenchmarks=true;ITSVertexerParam.phiCut=0.5;ITSVertexerParam.clusterContributorsCut=3;ITSVertexerParam.tanLambdaCut=0.2;ITSCATrackerParam.saveTimeBenchmarks=true;ITSCATrackerParam.trackingMode=${MODE_NUM}"

# Sync or async
# --ccdb-meanvertex-seed
ITS_TRACKING_MODE="--tracking-mode ${MODE}"

# GPU workflow specific config to run TPC GPU
GPU_WORKFLOW_PARS=";GPU_global.deviceType=CUDA;GPU_proc.forceMemoryPoolSize=8000000000;GPU_proc.forceHostMemoryPoolSize=1073741824;"

# Assemble parameters
ITS_RECOPAR+=";$ITS_CLUSTERPAR;$FASTMULTPAR;$ITS_TRKPAR;"
GPU_WORKFLOW_PARS+=";$ITS_TRKPAR;$FASTMULTPAR;"
