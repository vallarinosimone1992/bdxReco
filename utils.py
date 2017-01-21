#!/bin/python

import fnmatch
import os
import SCons
import glob
import platform
import sys



################
# File handling
################s

def getSubdirs(abs_path_dir) :  
    lst = [ name for name in os.listdir(abs_path_dir) if os.path.isdir(os.path.join(abs_path_dir, name)) and name[0] != '.' ]
    lst.sort()
    return lst

def recursiveDirs(root) :
    """Return a list of all subdirectories of root, top down,
       including root, but without .svn and .<arch> directories"""
    return filter( (lambda a : (a.rfind( ".svn")==-1) and \
                               (a.rfind( ".Linux")==-1) and \
                               (a.rfind( ".SunOS")==-1) and \
                               (a.rfind( ".Darwin")==-1) and \
                               (a.rfind( ".vxworks")==-1)),  [ a[0] for a in os.walk(root)]  )



def unique(list) :
    """Remove duplicates from list"""
    return dict.fromkeys(list).keys()



def scanFiles(dir, accept=["*.cpp"], reject=[]) :
    """Return a list of selected files from a directory & its subdirectories"""
    sources = []
    paths = recursiveDirs(dir)
    for path in paths :
        for pattern in accept :
            sources+=glob.glob(path+"/"+pattern)
    for pattern in reject :
        sources = filter( (lambda a : a.rfind(pattern)==-1 ),  sources )
    return unique(sources)



def subdirsContaining(root, patterns):
    """Return a list of subdirectories containing files of the given pattern"""
    dirs = unique(map(os.path.dirname, scanFiles(root, patterns)))
    dirs.sort()
    return dirs
    
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
    rootclingpath = "%s/bin/rootcling" % (rootsys)
    dictdir = os.getenv('BDXRECO_ROOT')+"/lib"

    
#    print "$SOURCE","$TARGET"
    if env['SHOWBUILD']==0:
			rootclingactionNoLinkDef = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCE ; mv `echo $TARGET | awk '{print substr($0,0,length($0)-3) \"_rdict.pcm\"}'` %s" % (rootclingpath," -I".join(env['CPPPATH']),dictdir), 'ROOTCLING  [$SOURCE]')
                        rootclingactionLinkDef = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCES ; mv `echo $TARGET | awk '{print substr($0,0,length($0)-3) \"_rdict.pcm\"}'` %s" % (rootclingpath," -I".join(env['CPPPATH']),dictdir), 'ROOTCLING  [$SOURCE]')
    else:	     	
			rootclingactionNoLinkDef = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCE ; mv `echo $TARGET | awk '{print substr($0,0,length($0)-3) \"_rdict.pcm\"}'` %s" % (rootclingpath," -I".join(env['CPPPATH']),dictdir))
                        rootclingactionLinkDef = SCons.Script.Action("%s -f $TARGET -c -p -I%s $SOURCES ; mv `echo $TARGET | awk '{print substr($0,0,length($0)-3) \"_rdict.pcm\"}'` %s" % (rootclingpath," -I".join(env['CPPPATH']),dictdir))
    if os.path.exists(rootclingpath) :
			bldNoLinkDef = SCons.Script.Builder(action = rootclingactionNoLinkDef, suffix='_Dict.cc', src_suffix='.h')
                        bldLinkDef = SCons.Script.Builder(action = rootclingactionLinkDef)
    else:
			print 'Rootcling does not exists. Unable to create ROOT dictionaries if any encountered.'
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
            if(int(env['SHOWBUILD'])>=1):
                print "  ----->  ROOT dictionary for %s" % f
            if os.path.isfile(filename+"_LinkDef.h"):     
                retVal=env.ROOTDictLinkDef(reldir+"/"+filename+"_Dict.cc",[reldir+"/"+f,reldir+"/"+filename+"_LinkDef.h"])
                if(int(env['SHOWBUILD'])>=1):
                    print "  -----> Using "+filename+"_LinkDef.h for dictionary"
            else:
                retVal=env.ROOTDictNoLinkDef(reldir+"/"+f)
    os.chdir(curpath)            
        
        
        
        
        
        
        

def configureJNI(env):
    """Configure the given environment for compiling Java Native Interface
       c or c++ language files"""

    # first look for a shell variable called JAVA_HOME
    java_base = os.environ.get('JAVA_HOME')
    if not java_base:
        if sys.platform == 'darwin':
            # Apple's OS X has its own special java base directory
            java_base = '/System/Library/Frameworks/JavaVM.framework'
        else:
            # Search for the java compiler
            print "JAVA_HOME environment variable not set. Searching for javac to find jni.h ..."
            if not env.get('JAVAC'):
                print "The Java compiler must be installed and in the current path, exiting"
                return 0
            jcdir = os.path.dirname(env.WhereIs('javac'))        
            if not jcdir:
                print "   not found, exiting"
                return 0
            # assuming the compiler found is in some directory like
            # /usr/jdkX.X/bin/javac, java's home directory is /usr/jdkX.X
            jc = os.path.join(jcdir,'javac')
            jcrealdir = os.path.dirname(os.path.realpath(jc))
            java_base = os.path.normpath(os.path.join(jcrealdir, ".."))
            print "  found, dir = " + java_base        
        
    if sys.platform == 'darwin':
        # Apple does not use Sun's naming convention
        java_headers = [os.path.join(java_base, 'Headers')]
        java_libs = [os.path.join(java_base, 'Libraries')]
    else:
        # linux
        java_headers = [os.path.join(java_base, 'include')]
        java_libs = [os.path.join(java_base, 'lib')]
        # Sun's windows and linux JDKs keep system-specific header
        # files in a sub-directory of include
       
        if java_base == '/usr' or java_base == '/usr/local':
            # too many possible subdirectories. Just use defaults
            print "stocazzo",java_libs,java_base,java_headers
            java_headers.append(os.path.join(java_headers[0], 'linux'))
            java_headers.append(os.path.join(java_headers[0], 'solaris'))
        else:
            # add all subdirs of 'include'. The system specific headers
            # should be in there somewhere
            java_headers = recursiveDirs(java_headers[0])

    # add Java's include and lib directory to the environment
    env.Append(CPPPATH = java_headers)
    # (only need the headers right now so comment out next line)
    #env.Append(LIBPATH = java_libs)

    # add any special platform-specific compilation or linking flags
    # (only need the headers right now so comment out next lines)
    #if sys.platform == 'darwin':
    #    env.Append(SHLINKFLAGS = '-dynamiclib -framework JavaVM')
    #    env['SHLIBSUFFIX'] = '.jnilib'

    # Add extra potentially useful environment variables
    #env['JAVA_HOME']   = java_base
    #env['JNI_CPPPATH'] = java_headers
    #env['JNI_LIBPATH'] = java_libs

    return 1
