from utils import *
from init_env import init_environment
import platform 

env = init_environment("qt5 gemc clas12 evio jana clhep root banks ccdb geant4 xercesc")

debug = ARGUMENTS.get('debug', 0)
if int(debug):
   env.Append(CCFLAGS = '-g')

#A.C. probably do this better
if (platform.system()=="Darwin"):
	print "We are on MAC"
	env.AppendUnique(LINKFLAGS='-flat_namespace')
	env.AppendUnique(SHLINKFLAGS=['-undefined', 'suppress'])
 
env.Append(CPPPATH='#/src/external')
env.Append(CPPPATH='#/src/libraries')

libExt=SConscript('src/external/SConstruct',exports='env')
lib=SConscript('src/libraries/SConstruct',exports='env')
progs=SConscript('src/programs/SConstruct',exports='env')
users=SConscript('src/users/SConstruct',exports='env')

env.Prepend(LIBS=libExt) 
env.Prepend(LIBS=lib)

env.Append(LIBPATH = ['#/lib'])

#bdxReco  = env.Program(source = 'bdxReco.cc',target = 'bdxReco')





