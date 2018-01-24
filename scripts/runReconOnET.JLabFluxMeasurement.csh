#!/bin/tcsh -f

if ($#argv != 1) then
        echo "use: runReconOnET.JLabMeasurement.csh runNumber"
        exit
endif

set runNumber = $1

echo runReconOnET: RunNumber is: $1

#this is the command
#need to connect to the et with the syntax:  ET:sessionName:stationName:host:port


#Local DB
#setenv JANA_CALIB_URL sqlite:///$BDXRECO_ROOT/bdx_sqlite.db

setenv JANA_CALIB_URL mysql://bdxwriter:e1tocome@jdaq3/BDX

#$BDXRECO_ROOT/bin/bdxReco --config=$BDXRECO_ROOT/config/bdx.config.JLabFlux.online -PPLUGINS=monitoring_rootspy,monitoring_DAQWaveformsMode1 ET:bdxtest:DSTMaker:jdaq3:11111
$BDXRECO_ROOT/bin/bdxReco --config=$BDXRECO_ROOT/config/bdx.config.JLabFlux.online -PPLUGINS=monitoring_rootspy ET:bdxtest:DSTMaker:jdaq3:11111
