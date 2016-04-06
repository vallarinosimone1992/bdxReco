from utils import *
from init_env import init_environment
import platform
import SCons

	
	
	



env = init_environment("clas12 qt5 evio jana clhep root banks ccdb geant4 xercesc")


debug = ARGUMENTS.get('debug', 0)
if int(debug):
   env.Append(CCFLAGS = '-g')

#A.C. probably do this better
if (platform.system()=="Darwin"):
	print "We are on MAC"
	env.AppendUnique(LINKFLAGS='-flat_namespace')
	env.AppendUnique(SHLINKFLAGS=['-undefined', 'suppress'])
 
env.Append(CPPPATH=Dir('#/src/external').srcnode().abspath)
env.Append(CPPPATH=Dir('#/src/libraries').srcnode().abspath)
env.Append(CPPPATH=Dir('#/.').srcnode().abspath)
env.AppendUnique(LINKFLAGS=['-lMinuit2','-lMinuit'])
env.Append(LIBPATH = ['#/lib'])
env.Replace(RPATH=Dir('#/lib').srcnode().abspath)

libExt=SConscript('src/external/SConstruct',exports='env')
lib=SConscript('src/libraries/SConstruct',exports='env')
progs=SConscript('src/programs/SConstruct',exports='env')
users=SConscript('src/users/SConstruct',exports='env')

#env.Prepend(LIBS="libExt") 
#env.Prepend(LIBS="libbdxReco")









