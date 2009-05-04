//
// C++ Interface: nds_backends
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NDS_BACKENDS_H
#define NDS_BACKENDS_H

#include "base/file_system.h"

#include <nds.h>
#include <fat.h>
#include <sys/dir.h>
#include "mixer/sound_driver.h"
#include "base/timer.h"

using namespace GUI;
class FileSystemNDS : public FileSystem {
	
	DIR_ITER* dir_stream;

	static FileSystem *create_fs();

	String current_dir;

public:

	virtual bool list_dir_begin(); ///< This starts dir listing
	virtual String get_next(bool* p_is_dir=0); ///< This retrieves dir entries, until an empty string is returned, optional p_is_dir can be passed
	virtual void list_dir_end(); ///< 

	virtual int get_drive_count();
	virtual String get_drive(int p_drive);

	virtual bool change_dir(String p_dir); ///< can be relative or absolute, return false on success
	virtual String get_current_dir(); ///< return current dir location
	virtual bool make_dir(String p_dir);

	virtual bool file_exists(String p_file);	


	static void set_default_filesystem();

	FileSystemNDS();
	~FileSystemNDS();

};

class Mixer;



class SoundDriverNDS : public SoundDriver {


	int lockc;
public:

	virtual void lock(); ///< Lock called from UI,game,etc (non-audio) thread, to access audio variables
	virtual void unlock(); ///< UnLock called from UI,game,etc (non-audio) thread, to access audio variables

	virtual const char * get_name();

	virtual Uint16 get_max_level_l(); //max level, range 0 - 1024
	virtual Uint16 get_max_level_r(); //max level, range 0 - 1024

	virtual bool is_active();
	virtual bool init();
	virtual void finish();

	virtual void set_audio_params(AudioParams *p_params);
	SoundDriverNDS();
	virtual ~SoundDriverNDS();

};


class TimerNDS : public Timer {
public:

	virtual TimerID create_timer( Method p_callback, int p_interval_msec );
	virtual void remove_timer(TimerID p_timer);
	virtual int get_interval(TimerID p_timer);
	virtual void change_timer_interval(TimerID p_timer,int p_interval_msec);
	virtual unsigned long int get_tick_ms(); /* get a millisecond tick, for delta times and not synced to clock */

	TimerNDS();
	virtual ~TimerNDS();
};




#endif
