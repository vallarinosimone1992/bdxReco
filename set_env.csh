#!/bin/tcsh -f

#print only if there is a prompt
alias echo 'if($?prompt) echo \!*  '

if(! $?BDXRECO_ROOT) then
    echo "BDXRECO_ROOT was not set properly"
    exit
endif

echo "BDXRECO_ROOT set to $BDXRECO_ROOT"

set MYSQLHOST=g3db.ge.infn.it
#set MYSQLHOST=clasdb.jlab.org
set MYSQLUSER=bdxreader
set MYSQLDB=gr3db
#set MYSQLDB=BDX

#setenv JANA_CALIB_URL sqlite:///$BDXRECO_ROOT/calib/ccdb.calib
setenv JANA_CALIB_URL mysql://$MYSQLUSER@$MYSQLHOST/$MYSQLDB
#setenv JANA_CALIB_URL mysql://mysql://bdxreader:g3db/gr3db
setenv JANA_PLUGIN_PATH $BDXRECO_ROOT/lib:$BDXRECO_ROOT/lib/users

#a.c. this is a work-around for MAC, where -rpath doens't work!!
setenv DYLD_LIBRARY_PATH ${BDXRECO_ROOT}/lib:${DYLD_LIBRARY_PATH}