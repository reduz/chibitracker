
EnsureSConsVersion(0,14);


import os;
import string;
import sys

palmos=False;
win_enabled=False

if (os.name=="nt"):
	wind32=True
	if (os.getenv("MINGW_PREFIX")!=None):
		env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['-DANSIC_LIBS_ENABLED','-DWINDOWS_ENABLED','-DSDL_ENABLED'],ENV=os.environ,tools=['mingw']);
		env.Append(CPPFLAGS=['-I'+os.getenv("MINGW_PREFIX")+'\\include\\SDL']);
		env.Append(LIBS=['mingw32','SDLmain','SDL']);
		env.icon_obj="cticon.o"
		env["DEBUG_CXXFLAGS"]=['-g3','-Wall']
		env["DEBUG_LINKFLAGS"]=[]
		env["RELEASE_CXXFLAGS"]=['-O2','-ffast-math']
		env["RELEASE_LINKFLAGS"]=['-Wl,--subsystem,windows']
		env["PROFILE_CXXFLAGS"]=['-pg','-g3','-Wall']
		env["PROFILE_LINKFLAGS"]=['-pg']
		
	else:
		env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['/DANSIC_LIBS_ENABLED','/DWINDOWS_ENABLED'],ENV=os.environ);
#		env.Append(CPPFLAGS=['/IC:\Program Files\Microsoft Visual Studio 8\VC\include\SDL','/D_REENTRANT','/DWINDOWS_ENABLED','/Zi','/Yd','/MT','/Gd']);
#	        env.Append(LIBS=['SDLmain','SDL']);
#		env.Append(LINKFLAGS=['/FORCE:MULTIPLE','/DEBUG']);
#		env["DEBUG_CXXFLAGS"]=['-g3','-Wall']
#		env["DEBUG_LINKFLAGS"]=[]
#		env["RELEASE_CXXFLAGS"]=['-O2','-ffast-math']
#		env["RELEASE_LINKFLAGS"]=[]
#		env["PROFILE_CXXFLAGS"]=['-pg','-g3','-Wall']
#		env["PROFILE_LINKFLAGS"]=['-pg']
		
		env.icon_obj=""
	
	win_enabled=True		
else:
	#unix

	env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['-DANSIC_LIBS_ENABLED'],ENV=os.environ);
	errorval=os.system("sdl-config --version");

        if (errorval):
                print "Error: cant execute sdl-config, make sure SDL is installed";
                exit(255);
	else:
		print("libSDL Detected");

	env.ParseConfig('sdl-config --cflags --libs')

	env.Append(CPPFLAGS=['-DPOSIX_ENABLED','-DSDL_ENABLED','-fno-exceptions']);
	
	errorval=os.system("pkg-config alsa --modversion");

        if (True or errorval):
                print "ALSA not detected.";
	else:
	 	print "ALSA detected.";
		env.ParseConfig('pkg-config alsa --cflags --libs')
		env.Append(CPPFLAGS=['-DALSA_ENABLED']);	

	env["DEBUG_CXXFLAGS"]=['-g3','-Wall']
	env["DEBUG_LINKFLAGS"]=[]
	env["RELEASE_CXXFLAGS"]=['-O2','-ffast-math']
	env["RELEASE_LINKFLAGS"]=[]
	env["PROFILE_CXXFLAGS"]=['-pg','-g3','-Wall']
	env["PROFILE_LINKFLAGS"]=['-pg']
	env.icon_obj=""

		
NDS=False

#if (NDS):
#	env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['-DANSIC_LIBS_ENABLED'],ENV=os.environ);

env.build_nds=NDS

env["LIBS"]=['interface','drivers','tracker','gui','fileio','player','song','mixer','globals']+env["LIBS"]


opts=Options()
opts.Add('target', 'Target: (debug/profile/release).', "debug")
opts.Add('prefix', 'The installation prefix', '/usr/local/')
opts.Add('meta_as_alt', 'META Key works as ALT (MAC keyboards)', 0)

opts.Update(env)
Help(opts.GenerateHelpText(env))
"""
if (NDS):

	os.environ['DEVKITPRO'] = env['DEVKITPRO']
	os.environ['DEVKITARM'] = env['DEVKITARM']
	env['PROGSUFFIX'] = '.elf'

	env['AS']='arm-eabi-as'
	env['CC'] = 'arm-eabi-gcc'
	env['CXX'] = 'arm-eabi-g++'
	env['AR'] = 'arm-eabi-ar'
	env['RANLIB'] = 'arm-eabi-ranlib'
	env['LD'] = 'arm-eabi-g++'
	env['CCFLAGS'] = Split('-MMD -MP -g0 -O3 -Wall -ffast-math -mcpu=arm9tdmi -mtune=arm9tdmi -mthumb-interwork -DARM9')
	env.Append(CPPPATH='$DEVKITPRO/libnds/include')
	
	env['ENV']['PATH'] = env['ENV']['PATH']+":"+env['DEVKITARM']+"/bin"
	env.Append(LIBS=['fat', 'nds9']);
	
	env.Append(LINKFLAGS=['-specs=ds_arm9.specs','-g','-mthumb-interwork','-Wl,-Map,output.map'])	
	env.Append(CPPFLAGS=['-DNDS_ENABLED', '-fno-rtti','-fno-exceptions'])
	env.Append(LIBPATH=['$DEVKITPRO/libnds/lib'])
	env.env_arm7 = Environment();
	env.env_arm7['CC']='arm-eabi-gcc'
	env.env_arm7['ENV']['PATH'] = env.env_arm7['ENV']['PATH']+":"+env['DEVKITARM']+"/bin"
	env.env_arm7.Append(LIBS=['nds7']);
	env.env_arm7.Append(CPPFLAGS=['-DARM7']);
	env.env_arm7.Append(CPPPATH=env["DEVKITPRO"]+'/libnds/include')
	env.env_arm7.Append(LINKFLAGS=['-specs=ds_arm7.specs','-Wl,-Map,output7.map'])	
	env.env_arm7.Append(LIBPATH=env["DEVKITPRO"]+'/libnds/lib')
"""


	
	
#	env['CXX']='g++-2.95'


env.Append(LIBPATH=['#song','#mixer','#gui','#drivers','#fileio','#tracker','#globals','#player','#interface']);


if (env['target']=='debug'):
        env.Append(CXXFLAGS=env["DEBUG_CXXFLAGS"]);
        env.Append(LINKFLAGS=env["DEBUG_LINKFLAGS"]);
	
if (env['target']=='profile'):
        env.Append(CXXFLAGS=env["PROFILE_CXXFLAGS"]);
        env.Append(LINKFLAGS=env["PROFILE_LINKFLAGS"]);

if (env['target']=='release'):
        env.Append(CXXFLAGS=env["RELEASE_CXXFLAGS"]);
        env.Append(LINKFLAGS=env["RELEASE_LINKFLAGS"]);

if ((env['meta_as_alt'])):
	env.Append(CXXFLAGS=['-DMETA_AS_ALT']);

env.bin_targets=[]
Export('env');

SConscript('globals/SCsub');
SConscript('song/SCsub');
SConscript('mixer/SCsub');
SConscript('player/SCsub');
SConscript('drivers/SCsub');
SConscript('fileio/SCsub');
SConscript('gui/SCsub');
SConscript('tracker/SCsub');
SConscript('interface/SCsub');

SConscript('program/SCsub');



if env['prefix']:
        dst_target = env.Install(env['prefix']+'/bin', env.bin_targets)
        env.Alias('install', dst_target )

