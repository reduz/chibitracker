//
// C++ Interface: reverb
//
// Description: 
//
//
// Author: James Martin (email?) (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef REVERB_H
#define REVERB_H

#include "mixer/mixer.h"

class ReverbParams;

class Reverb{
	
	struct State {
		int lwl;
		int lwr;
		int rwl;
		int rwr;
		unsigned int Offset;
		void reset() { lwl=0; lwr=0; rwl=0; rwr=0; Offset=0; }
		State() { reset(); }
	} state;
	
	ReverbParams *current_params;
	

	int *reverb_buffer;
	unsigned int reverb_buffer_size;
	Mixer::ReverbMode mode;
	int mix_rate;
	
	void adjust_current_params();
	
public:
	
	void set_mode(Mixer::ReverbMode p_mode);
	void process(int *p_input,int *p_output,int p_frames);
	void set_mix_rate(int p_mix_rate);
	
	Reverb();
	~Reverb();

};

#endif
