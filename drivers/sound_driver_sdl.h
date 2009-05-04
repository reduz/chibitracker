//
// C++ Interface: sound_driver_sdl
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOUND_DRIVER_SDL_H
#define SOUND_DRIVER_SDL_H

#ifdef SDL_ENABLED

#include "mixer/sound_driver.h"
#include <SDL.h>

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundDriverSDL : public SoundDriver {

	SDL_AudioSpec internal_params;
	
	Sint32 max_l;
	Sint32 max_r;
	
	bool active;
	AudioParams *params;
	
	void process (Uint8* p_stream, int p_bytes);
	static void process_cbk(void* arg, Uint8* p_stream, int p_bytes);
	
public:
	
	virtual void lock(); ///< Lock called from UI,game,etc (non-audio) thread, to access audio variables
	virtual void unlock(); ///< UnLock called from UI,game,etc (non-audio) thread, to access audio variables
	
	virtual Uint16 get_max_level_l(); //max level, range 0 - 1024
	virtual Uint16 get_max_level_r(); //max level, range 0 - 1024
	
	virtual const char * get_name();
	virtual bool is_active();
	virtual bool init();
	virtual void finish();

	virtual void set_audio_params(AudioParams *p_params);
	
	SoundDriverSDL();
	~SoundDriverSDL();

};

#endif
#endif