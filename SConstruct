from init_env import init_environment

env = init_environment("qt5 clas12 evio jana clhep root banks xercesc ccdb geant4")

# Include Path
incpath = ['.', 'src', 'event', 'ctof']
env.Append(CPPPATH = incpath)

env.Append(LIBPATH = ['lib'])
env.Append(LIBS = ['system', 'detectors'])

#detectors = Split("""
#                  """)

detectors = Split("""ctof/ctofHitR_factory.cc
							ctof/marcoCluster_factory.cc
						""")


system = Split("""src/options.cc
						src/Factory.cc
						event/JEventSourceEVIO.cc
						event/JEventSourceEVIOGenerator.cc
					""")

bdxRecoSources = Split("""bdxReco.cc
								  src/EventProcessor.cc
								  src/bdxOptions.cc
				           """)


libsys = env.Library(source = system,      target = 'lib/system');
libdet = env.Library(source = detectors,   target = 'lib/detectors');

bdxReco  = env.Program(source = bdxRecoSources, target = 'bdxReco')

Depends(libdet,  libsys);
Depends(bdxReco,  libsys);

