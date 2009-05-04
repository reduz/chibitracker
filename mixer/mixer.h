/***************************************************************************
                          mixer.h  -  description
                             -------------------
    begin                : Tue Apr 17 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MIXER_H
#define MIXER_H

#include "sample_defs.h"

/**Abstract base class representing a mixer
  *@author Juan Linietsky
  */


/******************************
 mixer.h
 ----------

Abstract base class for the mixer.
This is what the player uses to setup
voices and stuff.. this way
it can be abstracted to hardware
devices or other stuff..
********************************/

class Sample_ID; /* need this */

class Mixer {
public: 
	
	enum {
		
		FREQUENCY_BITS=8
		
	};
	
	enum ReverbMode {
		REVERB_MODE_ROOM,
		REVERB_MODE_STUDIO_SMALL,
		REVERB_MODE_STUDIO_MEDIUM,
		REVERB_MODE_STUDIO_LARGE,
		REVERB_MODE_HALL,
		REVERB_MODE_SPACE_ECHO,
		REVERB_MODE_ECHO,
		REVERB_MODE_DELAY,
		REVERB_MODE_HALF_ECHO
	};
		
	/* Callback */	
		
	virtual void set_callback_interval(int p_interval_us)=0; //in usecs, for tracker it's 2500000/tempo
	virtual void set_callback(void (*p_callback)(void*),void *p_userdata)=0;
	
	/* Voice Control */
			
	virtual void setup_voice(int p_voice_index,Sample_ID p_sample_id,Sint32 p_start_index) =0;
	virtual void stop_voice(int p_voice_index) =0;
	virtual void set_voice_frequency(int p_voice_index,Sint32 p_freq) =0; //in freq*FREQUENCY_BITS
	virtual void set_voice_panning(int p_voice_index,int p_pan) =0;
	virtual void set_voice_volume(int p_voice_index,int p_vol) =0;
        virtual void set_voice_filter(int p_filter,bool p_enabled,Uint8 p_cutoff, Uint8 p_resonance )=0;
        virtual void set_voice_reverb_send(int p_voice_index,int p_reverb)=0;
	virtual void set_voice_chorus_send(int p_voice_index,int p_chorus)=0; /* 0 - 255 */
	
	virtual void set_reverb_mode(ReverbMode p_mode)=0;
	virtual void set_chorus_params(unsigned int p_delay_ms,unsigned int p_separation_ms,unsigned int p_depth_ms10,unsigned int p_speed_hz10)=0;
	
	
	/* Info retrieving */	
	
	virtual Sint32 get_voice_sample_pos_index(int p_voice_index) =0;
	virtual int get_voice_panning(int p_voice_index) =0;
	virtual int get_voice_volume(int p_voice_index) =0;
	virtual Sample_ID get_voice_sample_id(int p_voice_index) =0;
	virtual bool is_voice_active(int p_voice_index) =0;
	virtual int get_active_voice_count()=0;
	virtual int get_total_voice_count()=0;
	
	
	virtual Uint32 get_mix_frequency()=0; //if mixer is not software, return 0

	/* Methods below only work with software mixers, meant for software-based sound drivers, hardware mixers ignore them */
	virtual Sint32 process(Sint32 p_frames)=0; /* Call this to process N frames, returns how much it was processed */
	virtual Sint32 *get_mixdown_buffer_ptr()=0; /* retrieve what was mixed */
	virtual void set_mix_frequency(Sint32 p_mix_frequency)=0;
		
	virtual ~Mixer() {}
};

#endif
