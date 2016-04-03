#!/bin/python

import fnmatch
import os
import SCons
import glob

def getSubdirs(abs_path_dir) :  
    lst = [ name for name in os.listdir(abs_path_dir) if os.path.isdir(os.path.join(abs_path_dir, name)) and name[0] != '.' ]
    lst.sort()
    return lst
    
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    RED = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def AddROOTdict(env,reldir,absdir):
	rootsys = os.getenv('ROOTSYS')
	rootcintpath  = "%s/bin/rootcint" % (rootsys)
	rootclingpath = "%s/bin/rootcling" % (rootsys)
	if env['SHOWBUILD']==0:
			rootcintaction  = SCons.Script.Action("%s -f $TARGET -c $SOURCE" % (rootcintpath) , 'ROOTCINT   [$SOURCE]')
			rootclingaction = SCons.Script.Action("%s -f $TARGET -c $SOURCE" % (rootclingpath), 'ROOTCLING  [$SOURCE]')
	else:
			rootcintaction  = SCons.Script.Action("%s -f $TARGET -c $SOURCE" % (rootcintpath) )
			rootclingaction = SCons.Script.Action("%s -f $TARGET -c $SOURCE" % (rootclingpath))
	if os.path.exists(rootclingpath) :
			bld = SCons.Script.Builder(action = rootclingaction, suffix='_Dict.cc', src_suffix='.h')
	elif os.path.exists(rootcintpath):
			bld = SCons.Script.Builder(action = rootcintaction, suffix='_Dict.cc', src_suffix='.h')
	else:
			print 'Neither rootcint nor rootcling exists. Unable to create ROOT dictionaries if any encountered.'
			return
	env.Append(BUILDERS = {'ROOTDict' : bld})
	# Generate ROOT dictionary file targets for each header
	# containing "ClassDef"
	#
	# n.b. It seems if scons is run when the build directory doesn't exist,
	# then the cwd is set to the source directory. Otherwise, it is the
	# build directory. Since the headers will only exist in the source
	# directory, we must temporarily cd into that to look for headers that
	# we wish to generate dictionaries for. (This took a long time to figure
	# out!)
	curpath = os.getcwd()
	srcpath = absdir
	if(int(env['SHOWBUILD'])>=1):
		print "---- Scanning for headers to generate ROOT dictionaries in: %s" % (srcpath)	
	os.chdir(srcpath)
	retVal="";
	for f in glob.glob('*.[h|hh|hpp]'):
		if 'ClassDef' in open(f).read():
			if(int(env['SHOWBUILD'])>=1):
				print "  ----->  ROOT dictionary for %s" % f
			print reldir
			retVal=env.ROOTDict(reldir+"/"+f)
#			#env.AppendUnique(ALL_SOURCES = env.ROOTDict(f))
	os.chdir(curpath)
	
	
	



