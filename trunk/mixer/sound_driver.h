//
// C++ Interface: sound_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include "globals/config.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class Mixer;

class AudioParams {
public:
	enum Bits {

		BITS_8,
		BITS_16,
		BITS_32
	};
private:		
	int mix_freq;
	Bits bits;
	int buffer_size;
	bool stereo;
	
	Mixer *mixer;
public:	

	void set_mix_freq(int p_freq);
	void set_bits(Bits p_bits);
	void set_buffer_size(int p_buffer_size);
	void set_stereo(bool p_stero);
	void set_mixer(Mixer *p_mixer);
	
	
	int get_mix_freq();
	Bits get_bits();
	int get_buffer_size();
	bool is_stereo();
	Mixer *get_mixer();
	
	AudioParams();
};

class SoundDriver {
public:
	
	virtual void lock()=0; ///< Lock called from UI,game,etc (non-audio) thread, to access audio variables
	virtual void unlock()=0; ///< UnLock called from UI,game,etc (non-audio) thread, to access audio variables
	
	virtual const char * get_name()=0;
	
	virtual Uint16 get_max_level_l()=0; //max level, range 0 - 1024
	virtual Uint16 get_max_level_r()=0; //max level, range 0 - 1024
	
	virtual bool is_active()=0;
	virtual bool init()=0;
	virtual void finish()=0;

	virtual void set_audio_params(AudioParams *p_params)=0;
	SoundDriver();
	virtual ~SoundDriver();

};

#endif
