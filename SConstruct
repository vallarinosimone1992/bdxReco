from utils import *
from init_env import init_environment

env = init_environment("qt5 clas12 evio jana clhep root banks xercesc ccdb geant4")

debug = ARGUMENTS.get('debug', 0)
if int(debug):
   env.Append(CCFLAGS = '-g')

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





