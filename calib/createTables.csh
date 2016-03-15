#!/bin/tcsh -f

#a dummy script to create all the tables

set command = "ccdb -c mysql://bdxwriter:e1tocome@clasdb.jlab.org/BDX"

#DAQ
set DAQFolder = "/DAQ"
set DAQIndexes = "crate=int slot=int channel=int"
set DAQPedestalLines = 6400
echo "DAQ"
$command mkdir $DAQFolder
$command mktbl $DAQFolder/pedestals -r $DAQPedestalLines $DAQIndexes pedestal=double "#daq pedestals in fadc counts"

#Calorimeter
set CalorimeterFolder = "/Calorimeter"
set CalorimeterIndexes = "sector=int x=int y=int readout=int"
set CalorimeterSipmGainLines = 40000

echo "Calorimeter"
echo "Sipm gain"
$command mkdir $CalorimeterFolder
$command mktbl $CalorimeterFolder/sipm_gain -r $CalorimeterSipmGainLines  $CalorimeterIndexes sipm_gain=double "#sipm gain"


#Inner Veto
set InnerVetoFolder = "/InnerVeto"
set InnerVetoIndexes = "sector=int layer=int component=int readout=int"
set InnerVetoSipmGainLines = 8000
set InnerVetoSipmAmplLines = 8000
set InnerVetoEneLines = 1600

echo "InnerVeto"
$command mkdir $InnerVetoFolder
echo "Sipm gain"
$command mktbl $InnerVetoFolder/sipm_gain -r $InnerVetoSipmGainLines $InnerVetoIndexes sipm_gain=double "#sipm gain"
echo "Sipm ampl"
$command mktbl $InnerVetoFolder/sipm_ampl -r $InnerVetoSipmAmplLines $InnerVetoIndexes sipm_ampl=double "#sipm ampl"
echo "Ene"
$command mktbl $InnerVetoFolder/Ene -r $InnerVetoEneLines $InnerVetoIndexes sipm_gain=double "#ene readout is always 0"


#Ext Veto
set ExtVetoFolder = "/ExtVeto"
set ExtVetoIndexes = "sector=int layer=int component=int readout=int"
set ExtVetoPMTGainLines = 1600
set ExtVetoThrLines = 1600
set ExtVetoEneLines = 1600

echo "ExtVeto"
$command mkdir $ExtVetoFolder
echo "PMT gain"
$command mktbl $ExtVetoFolder/PMT_gain -r $ExtVetoPMTGainLines $ExtVetoIndexes PMT_gain=double "#PMT gain"
echo "Threshold"
$command mktbl $ExtVetoFolder/Threshold -r $ExtVetoThrLines $ExtVetoIndexes threshold=double "#thr"
echo "Ene"
$command mktbl $ExtVetoFolder/Ene -r $ExtVetoEneLines $ExtVetoIndexes ene=double "#ene"

#Paddles
set PaddlesFolder = "/Paddles"
set PaddlesIndexes = "ID=int"
set PaddlesPMTGainLines = 2
set PaddlesThrLines = 2
set PaddlesEneLines = 2

echo "Paddles"
$command mkdir $Paddles
echo "PMT gain"
$command mktbl $PaddlesFolder/PMT_gain -r $PaddlesPMTGainLines $PaddlesIndexes PMT_gain=double "#PMT gain"
echo "threshold"
$command mktbl $PaddlesFolder/Threshold -r $PaddlesThrLines $PaddlesIndexes threshold=double "#thr"
echo "energy"
$command mktbl $PaddlesFolder/Ene -r $PaddlesEneLines $PaddlesIndexes energy=double "#energy"
