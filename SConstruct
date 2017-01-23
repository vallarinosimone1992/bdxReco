from utils import *
from init_env import init_environment
import platform
import SCons

env = init_environment("qt5 clas12 root evio jana clhep banks ccdb geant4 xercesc mlibrary")


debug = ARGUMENTS.get('debug', 0)
if int(debug):
   print "DEBUG IS ON"
   env.Append(CCFLAGS = '-g')

et_enable = ARGUMENTS.get('ET',0)
if int(et_enable):
    print "ET-ring support is enabled"
    env.AppendUnique(CPPDEFINES='ET_SUPPORT_ENABLE')


#A.C. probably do this better
if (platform.system()=="Darwin"):
	print "We are on MAC"
	env.AppendUnique(LINKFLAGS='-flat_namespace')
	env.AppendUnique(SHLINKFLAGS=['-undefined', 'suppress'])
 
env.Append(CPPPATH=Dir('#/src/external').srcnode().abspath)
env.Append(CPPPATH=Dir('#/src/libraries').srcnode().abspath)
env.Append(CPPPATH=Dir('#/.').srcnode().abspath)
#env.AppendUnique(LINKFLAGS=['-lMinuit2','-lMinuit'])
env.Append(LIBPATH = ['#/lib'])
env.Replace(RPATH=Dir('#/lib').srcnode().abspath)

Export('env debug et_enable')

libExt=SConscript('src/external/SConstruct')
lib=SConscript('src/libraries/SConstruct')
progs=SConscript('src/programs/SConstruct')
users=SConscript('src/users/SConstruct')

#env.Prepend(LIBS="libExt") 
#env.Prepend(LIBS="libbdxReco")









