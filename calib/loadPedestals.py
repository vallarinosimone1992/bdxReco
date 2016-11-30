                                                                                                                                                                                                                            
# -*- coding: utf-8 -*-                                                                                                                                                                                                                             

import string,math,os,sys,subprocess
import argparse

#check current working dir
command = "cd "+os.environ['BDXRECO_ROOT']+"/calib ; pwd "
p = subprocess.Popen(command,shell=True,stdout=subprocess.PIPE)
bdx_fold , err = p.communicate()

command = "pwd"
p = subprocess.Popen(command,shell=True,stdout=subprocess.PIPE)
cur_fold , err = p.communicate()

if (cur_fold != bdx_fold):
    print "Error, cur folder is ",cur_fold
    print "This script is supposed to run from ",bdx_fold
    sys.exit()

#Chek input arguments
#this should be: -v Variation -min runMin -max runMax
#if variation is not specified, use "default"
#if min and/or max are not specified, used CCDB default behavior



parser = argparse.ArgumentParser(description='Load data in CCDB for given variation / runs')
parser.add_argument('--variation',type=str, default="default", help='If set, set the variation to use. The variation should correspond to a folder with the txt files to upload')
parser.add_argument('--connection',type=str,required=True,help="Connection string to use")
parser.add_argument('--firstRun',type=int,default=1000,help="minimum run to consider in the folder")
args = parser.parse_args()

variation = args.variation
connection_string = args.connection
firstRun = args.firstRun
#check if DAQ_pedestals folder exists
if (os.path.isdir("DAQ_pedestals")==False):
    print "Error, folder DAQ_pedestals does not exists"
    sys.exit()     

commandBase = "ccdb -c "+connection_string+" add -v "+variation

print "Loading pedestals for variation ",variation," and run_min: ",firstRun

#now need to loop over ALL the files there
for fn in os.listdir("DAQ_pedestals"):
    if os.path.isfile("DAQ_pedestals/"+fn):
        #fn is in the form: runxxxxx.ped
        run=fn[3:].split('.')[0]
        if (int(run)<firstRun):
            continue
        print("Run number: "+run)
        outF=open("DAQ_pedestals/tables/run_"+run+".dat","w")
        #write the first crate up to slot #3
        for slot in range(0,4):
            for channel in range(0,16):
                outF.write("0 "+str(slot)+" "+str(channel)+" 0 0 \n")
        
        #in case of default variation, do not load
        if (variation=="default"):
            for slot in range(4,10):
                for channel in range(0,16):
                       outF.write("0 "+str(slot)+" "+str(channel)+" "+"0 0 \n")
        else: #load
            with open("DAQ_pedestals/"+fn) as inF:
                for line in inF:
                    data=line.split()
                    slot=data[0]
                    channel=data[1]
                    ped=data[2]
                    RMS=data[3]
                    outF.write("0 "+str(slot)+" "+str(channel)+" "+str(ped)+" "+str(RMS)+"\n")
            inF.close()
        #done up to slot 9. need to proceed up to 19
        for slot in range(10,20):
            for channel in range(0,16):
                outF.write("0 "+str(slot)+" "+str(channel)+" 0 0\n")        
        for crate in range(1,20):
                for slot in range(0,20):
                    for channel in range(0,16):
                        outF.write(str(crate)+" "+str(slot)+" "+str(channel)+" 0 0\n")        
        outF.close()
        #at this point, we have the file for run 
        #specify in the command
        command = commandBase+" -r "+str(run)+"-"+str(run)
        command += " /DAQ/pedestals DAQ_pedestals/tables/run_"+run+".dat"
        print command
        subprocess.call(command,shell=True)

