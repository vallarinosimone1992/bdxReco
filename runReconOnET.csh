#!/bin/tcsh -f

if ($#argv != 1) then
        echo "use: runReconOnET.csh runNumber"
        exit
endif

set runNumber = $1

echo runReconOnET: RunNumber is: $1

#this is the command
#need to connect to the et with the syntax:  ET:sessionName:stationName:host:port
$BDXRECO_ROOT/bin/bdxReco -PTT:XML_FILENAME="$BDXRECO_ROOT/tt.SecondProto.xml" --config=$BDXRECO_ROOT/config/bdx.config.CataniaSecondProtoUP.onlineDST -PSYSTEM:BUILD_DST="CataniaProto2" ET:JLabFadc:DSTMaker:jdaq4:11111


