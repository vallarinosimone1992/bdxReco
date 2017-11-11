#!/bin/tcsh -f

if ($#argv != 1) then
        echo "use: runReconOnET.JLabMeasurement.csh runNumber"
        exit
endif

set runNumber = $1

echo runReconOnET: RunNumber is: $1

#this is the command
#need to connect to the et with the syntax:  ET:sessionName:stationName:host:port


#sh -c "exec -a bdxReco_onlineDST $BDXRECO_ROOT/bin/bdxReco -PTT:XML_FILENAME="$BDXRECO_ROOT/tt.SecondProto.xml" --config=$BDXRECO_ROOT/config/bdx.config.CataniaSecondProtoUP.onlineDST -PSYSTEM:BUILD_DST="CataniaProto2" ET:bdxtest:DSTMaker:jdaq3:11111"

sh -c "exec -a bdxReco_onlineDST $BDXRECO_ROOT/bin/bdxReco -PTT:XML_FILENAME="$BDXRECO_ROOT/tt.JLabFluxMeasurement.xml" --config=$BDXRECO_ROOT/config/bdx.config.JLabFlux.online --plugin=andrea_janaview ET:bdxtest:DSTMaker:jdaq3:11111"
