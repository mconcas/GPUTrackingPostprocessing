#!/bin/bash -x

# Common configurations and commands
rm -f /dev/shm/*fmq*;
killall o2-its-reco-workflow -s 9;
killall o2-ctf-reader-workflow -s 9;

MODE=sync
MODE_NUM=$((MODE == "sync" ? 0 : 1))
ITS_RECOPAR="--configKeyValues "
MAXTF=1;
GLOSET="--shm-segment-size 16000000000";

# Define ConfigKeyValues
ITS_CLUSTERPAR=";ITSClustererParam.maxBCDiffToMaskBias=-10;ITSClustererParam.maxBCDiffToSquashBias=10;"
FASTMULTPAR=";fastMultConfig.cutMultClusLow=-1;fastMultConfig.cutMultClusHigh=-1;fastMultConfig.cutMultVtxHigh=-1;"

# Cuts for PbPb specificially
ITS_TRKPAR="ITSCATrackerParam.nThreads=20;ITSVertexerParam.nThreads=20;ITSVertexerParam.phiCut=0.005;ITSVertexerParam.clusterContributorsCut=16;ITSVertexerParam.tanLambdaCut=0.002;ITSVertexerParam.lowMultBeamDistCut=0;ITSCATrackerParam.saveTimeBenchmarks=true;ITSCATrackerParam.trackingMode=${MODE_NUM}"

# Sync or async
# --ccdb-meanvertex-seed
ITS_TRACKING_MODE="--tracking-mode ${MODE}"

# GPU workflow specific config to run TPC GPU
GPU_WORKFLOW_PARS=";GPU_global.deviceType=HIP;GPU_proc.forceMemoryPoolSize=10000000000;GPU_proc.forceHostMemoryPoolSize=1073741824;"

# Assemble parameters
ITS_RECOPAR+=";$ITS_CLUSTERPAR;$FASTMULTPAR;$ITS_TRKPAR;"
GPU_WORKFLOW_PARS+=";$ITS_TRKPAR;$FASTMULTPAR;"