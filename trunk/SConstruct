
EnsureSConsVersion(0,14);


import os;
import string;
import sys

palmos=False;
win32=False

win32cross=False
win32msvc=False
NDS=False
#env = Environment(CC='arm-palmos-gcc',CXX='arm-palmos-g++' , RANLIB='arm-palmos-ranlib', AR='arm-palmos-ar', CPPPATH=['#/globals','#.']);

if (NDS):
	env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['-DANSIC_LIBS_ENABLED'],ENV=os.environ);

elif (win32 and win32cross):
	env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['-DANSIC_LIBS_ENABLED','-g3','-Wall','-fstrict-aliasing'],CXX='i586-mingw32msvc-g++',RANLIB='i586-mingw32msvc-ranlib',LD='i586-mingw32msvc-ld',CC='i586-mingw32msvc-gcc',AR='i586-mingw32msvc-ar' );
elif (win32 and win32msvc):
	env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['/DANSIC_LIBS_ENABLED'],ENV=os.environ);
else:
	env = Environment(CPPPATH=['#/globals','#gui','#.'],CPPFLAGS=['-DANSIC_LIBS_ENABLED'],ENV=os.environ);

env.build_nds=NDS

env.Append(LIBS=['interface','drivers','tracker','gui','fileio','player','song','mixer','globals']);

env.icon_obj=""

opts=Options()
opts.Add('optimize', 'Optimize.', 0)
opts.Add('debug', 'Add debug symbols.', 1)
opts.Add('profile', 'profile', 0)
opts.Add('prefix', 'The installation prefix', '/usr/local/')
opts.Add('meta_as_alt', 'META Key works as ALT (MAC keyboards)', 0)
opts.Add('DEVKITPRO', 'the DEVKITPRO env variable', '/usr/local/devkitPro')
opts.Add('DEVKITARM', 'the DEVKITARM env variable', '/usr/local/devkitPro/devkitARM')

opts.Update(env)
Help(opts.GenerateHelpText(env))

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

elif (win32cross and win32):

	env.Append(CPPFLAGS=['-I/usr/i586-mingw32msvc/include/SDL','-D_REENTRANT','-DWINDOWS_ENABLED','-DSDL_ENABLED']);
	env.Append(LINKFLAGS=['-L/usr/i586-mingw32msvc/lib','-mwindows']);
	env.Append(LIBS=['mingw32','SDLmain','SDL']);
	env.icon_obj="cticon.o"

elif (win32):
	if (win32msvc):

		env.Append(CPPFLAGS=['/IC:\Program Files\Microsoft Visual Studio 8\VC\include\SDL','/D_REENTRANT','/DWINDOWS_ENABLED','/Zi','/Yd','/MT','/Gd']);
	        env.Append(LIBS=['SDLmain','SDL']);
		env.Append(LINKFLAGS=['/FORCE:MULTIPLE','/DEBUG']);
        else:
		env.Append(CPPFLAGS=['-IC:\\MingW\\include\\SDL','-D_REENTRANT','-DWINDOWS_ENABLED','-DSDL_ENABLED']);
		env.Append(LINKFLAGS=['-mconsole']);
	        env.Append(LIBS=['mingw32','SDLmain','SDL']);
		env.icon_obj="cticon.o"

else:
	#UNIX, needs some detection
	errorval=os.system("sdl-config --version");

        if (errorval):
                print "Error: cant execute sdl-config, make sure SDL is installed";
                exit(255);
	else:
		print("libSDL Detected");

	env.ParseConfig('sdl-config --cflags --libs')

	env.Append(CPPFLAGS=['-DPOSIX_ENABLED','-DSDL_ENABLED','-fno-exceptions']);
	
	errorval=os.system("pkg-config alsa --modversion");

        if (errorval):
                print "ALSA not detected.";
	else:
	 	print "ALSA detected.";
		env.ParseConfig('pkg-config alsa --cflags --libs')
		env.Append(CPPFLAGS=['-DALSA_ENABLED']);	

	
	
#	env['CXX']='g++-2.95'


env.Append(LIBPATH=['#song','#mixer','#gui','#drivers','#fileio','#tracker','#globals','#player','#interface']);


if (not win32msvc):
	print env['optimize']
	if (int(env['debug']) and not int(env['optimize'])):
	        env.Append(CXXFLAGS=['-g3','-Wall']);
	if (int(env['profile'])):
	        env.Append(CXXFLAGS=['-pg']);
		env.Append(LINKFLAGS=['-pg']);
	if (int(env['optimize'])):
		env.Append(CXXFLAGS=['-O2','-ffast-math']);

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

