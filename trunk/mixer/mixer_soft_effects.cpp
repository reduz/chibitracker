//
// C++ Implementation: mixer_soft_effects
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mixer_soft.h"
#include <stdio.h>
#include <math.h>


#define SINE_INC_FRAC 32
#define SINE_SHIFT_FRAC (32-9)
#define SINE_SHIFT_MASK 


void Mixer_Soft::prepare_chorus(int p_frames ) {

	unsigned int pos=chorus.ringbuffer_pos;
	for (int i=0;i<p_frames;i++) {
	
		
		chorus.ringbuffer[ (pos & CHORUS_RINGBUFF_MASK)<<1 ]=0;
		chorus.ringbuffer[ ((pos & CHORUS_RINGBUFF_MASK)<<1) + 1 ]=0;
		
		pos++;
	}
}

static inline int approximate_sin(unsigned int x)
{	
	unsigned s=-int(x>>31);
	x+=x;
	x=x>>16;
	x*=x^0xffff;            // x=x*(2-x)
	x+=x;                   // optional
	return x^s;
}

void Mixer_Soft::process_chorus(int p_frames ) {


	int delay_s=chorus.delay_ms * data.mixfreq_current / 1000;
	unsigned int delay_left_s=chorus.separation_ms * data.mixfreq_current / 1000;
	int depth_s=(int)chorus.depth_ms10 * data.mixfreq_current / 10000;
	
	
	unsigned int osc_incr=(Uint64)0xFFFFFFFF / (( (Uint64)data.mixfreq_current * 10 ) / (Uint64)(chorus.speed_hz10) );
				
	
	Sint32 *dst=&data.mixdown_buffer[ data.mixdown_pos ];

	for (int i=0;i<p_frames;i++) {
		
		int delay_sine=(approximate_sin( chorus.osc_pos ) >> 17 ) * depth_s >>15;
		//int delay_sine=sin( ((double)chorus.osc_pos/(double)0xFFFFFFFF)*M_PI*2.0 ) * depth_s;
		
		unsigned int delay=(unsigned int)(delay_sine+delay_s+chorus.depth_ms10);
		unsigned int src_pos_r=chorus.ringbuffer_pos-delay;
		unsigned int src_pos_l=src_pos_r-delay_left_s;
		//printf("spl %i\n",src_pos_r);
		Sint32 sample_l=chorus.ringbuffer[ (src_pos_l & CHORUS_RINGBUFF_MASK)<<1 ];
		Sint32 sample_r=chorus.ringbuffer[ ((src_pos_r & CHORUS_RINGBUFF_MASK)<<1) + 1 ];
		
		*(dst++) += sample_l;
		*(dst++) += sample_r;
		
		chorus.ringbuffer_pos++;
		chorus.osc_pos+=osc_incr;
	}

}