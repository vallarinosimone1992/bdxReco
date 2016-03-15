#!/bin/tcsh -f

#print only if there is a prompt
alias echo 'if($?prompt) echo \!*  '

if(! $?BDXRECO_ROOT) then
    echo "BDXRECO_ROOT was not set properly"
    exit
endif

echo "BDXRECO_ROOT set to $BDXRECO_ROOT"

set MYSQLDB=clasdb.jlab.org
set MYSQLUSER=bdxreader

setenv JANA_CALIB_URL sqlite:///$BDXRECO_ROOT/calib/ccdb.calib
#setenv JANA_CALIB_URL mysql://$MYSQLUSER@$MYSQLDB/BDX
setenv JANA_PLUGIN_PATH $BDXRECO_ROOT/lib:$BDXRECO_ROOT/lib/users

