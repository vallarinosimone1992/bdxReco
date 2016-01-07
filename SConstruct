from utils import *
from init_env import init_environment

env = init_environment("qt5 clas12 evio jana clhep root banks xercesc ccdb geant4")


env.Append(LIBPATH = ['lib'])
env.Prepend(LIBS = ['system','event','detectors','daq','evioReader'])


daq        = Glob('daq/*.cc');env.Append(CPPPATH='daq')
detectors  = Glob('ctof/*.cc');env.Append(CPPPATH='ctof')
event      = Glob('event/*.cc');env.Append(CPPPATH='event')
evioReader = Glob('evioReader/*.cc');env.Append(CPPPATH='evioReader')
system     = Glob('system/*.cc');env.Append(CPPPATH='system')


libdaq =	 	env.Library(source = daq,        target = 'lib/daq');
libevioreader = env.Library(source = evioReader, target = 'lib/evioReader');
libdetectors = 	env.Library(source = detectors,  target = 'lib/detectors');
libevent = 		env.Library(source = event,      target = 'lib/event');
libsystem = 	env.Library(source = system,     target = 'lib/system');


#Here goes users's plugins
libuser={};


users = getSubdirs('./users')

for user in users :
	plugins = getSubdirs('./users/'+str(user))
	for plugin in plugins:
		print  bcolors.OKGREEN,"Found plugin ",plugin," for user ",user,bcolors.ENDC
		#We do not want to increment "at libitum" CPPPATH
		tmp_cpppath=env['CPPPATH'];
		dir = './users/'+str(user)+'/'+str(plugin)
		user_plugin_src  =  Glob(dir+'/*.cc');env.Append(CPPPATH=dir)

		env.SharedLibrary(source=user_plugin_src,target = 'lib/'+user+'_'+plugin)
		env.Replace(CPPPATH = tmp_cpppath)		


#The main executable
bdxReco  = env.Program(source = 'bdxReco.cc',target = 'bdxReco')





