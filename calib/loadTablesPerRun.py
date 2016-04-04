#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import string,math,os,sys,subprocess
import argparse
import glob

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
parser.add_argument('--variation', type=str, default="default", help='If set, set the variation to use. The variation should correspond to a folder with the txt files to upload')
parser.add_argument('--connection', type=str, required=True, help='Connection string')


args = parser.parse_args()

variation = args.variation
connection_string = args.connection



#check if variation folder exists
if (os.path.isdir(variation)==False):
    print "Error, folder",variation,"does not exists"
    sys.exit()

#Proceed with tables loading
#Prepare the command
commandBase = "ccdb -c "+connection_string+" add -v "+variation+" "

#Calorimeter
table = "/Calorimeter/Ene "
filen = variation+"/Calorimeter.Ene"
files = glob.glob(filen+".*")
for file in files:
    run=file.split("/")[1].split(".")[2]
    print file+" ",run
    command = commandBase+table+"-r "+str(run)+"-"+str(run)+" "+file
    subprocess.call(command,shell=True)


#InnerVeto
table = "/InnerVeto/sipm_gain "
filen = variation+"/InnerVeto.sipm_gain"
files = glob.glob(filen+".*")
for file in files:
    run=file.split("/")[1].split(".")[2]
    print file+" ",run
    command = commandBase+table+"-r "+str(run)+"-"+str(run)+" "+file
    subprocess.call(command,shell=True)

table = "/InnerVeto/sipm_ampl "
filen = variation+"/InnerVeto.sipm_ampl"
files = glob.glob(filen+".*")
for file in files:
    run=file.split("/")[1].split(".")[2]
    print file+" ",run
    command = commandBase+table+"-r "+str(run)+"-"+str(run)+" "+file
    subprocess.call(command,shell=True)


#ExtVeto
table = "/ExtVeto/Ene "
filen = variation+"/ExtVeto.Ene"
files = glob.glob(filen+".*")
for file in files:
    run=file.split("/")[1].split(".")[2]
    print file+" ",run
    command = commandBase+table+"-r "+str(run)+"-"+str(run)+" "+file
    subprocess.call(command,shell=True)

#Paddles
table = "/Paddles/Ene "
filen = variation+"/Paddles.Ene"
files = glob.glob(filen+".*")
for file in files:
    run=file.split("/")[1].split(".")[2]
    print file+" ",run
    command = commandBase+table+"-r "+str(run)+"-"+str(run)+" "+file
    subprocess.call(command,shell=True)

