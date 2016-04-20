#!/bin/python

import fnmatch
import os
import SCons
import glob
import platform

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
			rootcintactionNoLinkDef  = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCE" % (rootcintpath," -I".join(env['CPPPATH'])), 'ROOTCINT   [$SOURCE]')
			rootclingactionNoLinkDef = SCons.Script.Action("%s  -f $TARGET -c -p -I%s $SOURCE" % (rootclingpath," -I".join(env['CPPPATH'])), 'ROOTCLING  [$SOURCE]')
	else:
			rootcintactionNoLinkDef  = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCE" % (rootcintpath," -I".join(env['CPPPATH'])))
			rootclingactionNoLinkDef = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCE" % (rootclingpath," -I".join(env['CPPPATH'])))

	if os.path.exists(rootclingpath) :
			bldNoLinkDef = SCons.Script.Builder(action = rootclingactionNoLinkDef, suffix='_Dict.cc', src_suffix='.h')
	elif os.path.exists(rootcintpath):
			bldNoLinkDef = SCons.Script.Builder(action = rootcintactionNoLinkDef, suffix='_Dict.cc', src_suffix='.h')
	else:

			print 'Neither rootcint nor rootcling exists. Unable to create ROOT dictionaries if any encountered.'
			return

	if env['SHOWBUILD']==0:
			rootcintactionLinkDef  = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCES" % (rootcintpath," -I".join(env['CPPPATH'])), 'ROOTCINT   [$SOURCE]')
			rootclingactionLinkDef = SCons.Script.Action("%s  -f $TARGET -c -p -I%s $SOURCES" % (rootclingpath," -I".join(env['CPPPATH'])), 'ROOTCLING  [$SOURCE]')
	else:
			rootcintactionLinkDef  = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCES" % (rootcintpath," -I".join(env['CPPPATH'])))
			rootclingactionLinkDef = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCES" % (rootclingpath," -I".join(env['CPPPATH'])))

	if os.path.exists(rootclingpath) :
			bldLinkDef = SCons.Script.Builder(action = rootclingactionLinkDef)
	elif os.path.exists(rootcintpath):
			bldLinkDef = SCons.Script.Builder(action = rootcintactionLinkDef)
	else:
			print 'Neither rootcint nor rootcling exists. Unable to create ROOT dictionaries if any encountered.'
			return	


	env.Append(BUILDERS = {'ROOTDictNoLinkDef' : bldNoLinkDef})
	env.Append(BUILDERS = {'ROOTDictLinkDef' : bldLinkDef})

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
			filename, file_extension = os.path.splitext(f)	
                        if (platform.system()=="Darwin"): 
                                print "ON MAC no ROOT dictionary generation is NOT (yet) supported"
                        else:
                            if(int(env['SHOWBUILD'])>=1):
				print "  ----->  ROOT dictionary for %s" % f
                            if os.path.isfile(filename+"_LinkDef.h"):
                                if(int(env['SHOWBUILD'])>=1):
                                    print "  -----> Using "+filename+"_LinkDef.h for dictionary" 
                                retVal=env.ROOTDictLinkDef(reldir+"/"+filename+"_Dict.cc",[reldir+"/"+f,reldir+"/"+filename+"_LinkDef.h"])
                            else:
                                retVal=env.ROOTDictNoLinkDef(reldir+"/"+f)
        os.chdir(curpath)
	
	
	



