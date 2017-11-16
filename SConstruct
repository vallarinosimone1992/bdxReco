from utils import *
from init_env import init_environment
import platform
import SCons



mc_enable = ARGUMENTS.get('MC',0)
if int(mc_enable):
    print "MC support is enabled"
    env = init_environment("qt5 root geant4 clhep evio xercesc ccdb mlibrary clas12 jana")
    env.AppendUnique(CPPDEFINES='MC_SUPPORT_ENABLE')
else:
    print bcolors.WARNING," no MC support",bcolors.ENDC
    env = init_environment("clhep evio xercesc ccdb jana root")
    env.Replace(ENV = os.environ)

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
env.Append(LIBPATH = ['#/lib'])

env.Replace(RPATH=Dir('#/lib').srcnode().abspath)



Export('env debug et_enable mc_enable')

libExt=SConscript('src/external/SConstruct')
lib=SConscript('src/libraries/SConstruct')
progs=SConscript('src/programs/SConstruct')
plugins=SConscript('src/plugins/SConstruct')
users=SConscript('src/users/SConstruct')










