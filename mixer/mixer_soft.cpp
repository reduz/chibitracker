//
// C++ Implementation: mixer_palm
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mixer_soft.h"
#include "error_macros.h"
#include <stdio.h>

#define MIXDOWN_LENGTH (data.mix_stereo?(MIXDOWN_BUFFER_SIZE/2):MIXDOWN_BUFFER_SIZE)
#define C5FREQ 261.6255653006
static const Uint64 C5FREQ_FP = ((Uint64)(C5FREQ*(double)(1<<Mixer::FREQUENCY_BITS)));

/* Mixer Stuff */


void Mixer_Soft::set_callback_interval(int p_interval_us) {

	callback.interval=p_interval_us;
	callback.frames_remaining=0;
}

void Mixer_Soft::set_callback(void (*p_callback)(void*),void *p_userdata) {
	
	callback.func=p_callback;
	callback.userdata=p_userdata;	
}

void Mixer_Soft::set_mix_frequency(Sint32 p_mix_frequency) {
	
	data.mix_freq=p_mix_frequency;	
}

Uint32 Mixer_Soft::get_mix_frequency() {
	
	return data.mix_freq;
}

void Mixer_Soft::set_mix_stereo(bool p_mix_stereo) {
	
	data.mix_stereo=p_mix_stereo;
	
}

void Mixer_Soft::set_mix_16bits(bool p_mix_16bits) {
	
	data.mix_16bit=p_mix_16bits;
}


Sint32 *Mixer_Soft::get_mixdown_buffer_ptr() {
	
	return data.mixdown_buffer;	
}

void Mixer_Soft::set_mix_interpolation(InterpolationType p_type) {

	data.interpolation_type=p_type;			
}

void Mixer_Soft::process_internal(Sint32 p_frames) {

	int voices_used=0;

	if (data.oversample_current)
		p_frames*=2;

	
	if (data.use_effects) {
		prepare_chorus(p_frames);
		for (int i=0;i<(p_frames*2);i++)
			data.reverb_buffer[i]=0;	
		
	}
	
	for (int i=0;i<MAX_VIRTUAL_VOICES;i++) {
		
		Voice &v=voice_array[i];
		if (!v.active)
			continue;
			

		/* Checking if sample is valid */		
		if (v.sample.current_freq_fp<=0) { /* a slide down or something can do this, so i guess it's legal */
			v.active=false;
			break;
		}
		
		if (!sample_manager->check(v.sample.sample)) { /* but this is not */
			ERR_PRINT("Sample ID is invalid for this voice, disabling");
			v.active=false;			
			break;
		}
		
		/* Calculate fixed point increment, some ugly fixed point math */

		Sint64 sample_c5_fp=(Sint64)(sample_manager->get_c5_freq(v.sample.sample))<<FREQUENCY_BITS;
		Sint64 target_freq=(Sint64)v.sample.current_freq_fp*sample_c5_fp/C5FREQ_FP;
		
		
		target_freq<<=MIXING_FRAC_BITS;
		v.realtime.increment_index_fp=((target_freq/data.mixfreq_current))>>FREQUENCY_BITS;
		
		
		
		//printf("src freq is %i, target freq is %i, sample freq is %i,incr %.4f\n",v.sample.current_freq_fp>>FREQUENCY_BITS,(int)(target_freq>>FREQUENCY_BITS),(int)sample_manager->get_c5_freq(v.sample.sample),((double)v.realtime.increment_index_fp/(double)(1<<MIXING_FRAC_BITS)));
		
		/*
		//printf("diff is %i,%i\n",v.sample.current_freq_fp>>MIXING_FREQ_FRAC_BITS,C5FREQ_FP>>MIXING_FREQ_FRAC_BITS);
		v.realtime.increment_index_fp=((Sint64)(v.sample.current_freq_fp*c5freq_at_srate_inc)/(Sint64)C5FREQ_FP)>>FREQUENCY_BITS;

		v.realtime.increment_index_fp = (v.realtime.increment_index_fp * c5freq_at_srate_inc ) >> MIXING_FRAC_BITS;
		
		*/
		v.realtime.increment_index_fp*=v.realtime.backwards_play?-1:1;


		if (!v.realtime.increment_index_fp) {

			v.active=false;
			break;
		}

		
		v.realtime.old_vol_l=v.realtime.vol_l;
		v.realtime.old_vol_r=v.realtime.vol_r;

		Sint32 vol=v.amp.volume;
		vol=(vol*data.global_volume) >> 7;
		if ( data.mix_stereo ) {
			
			if(v.amp.pan != PAN_SURROUND) {
				v.realtime.vol_l=( (Sint32)vol*(PAN_RIGHT-(Sint32)v.amp.pan) ) >> PAN_BITS;
				v.realtime.vol_r=( (Sint32)vol*(Sint32)v.amp.pan ) >> PAN_BITS;
			} else {
				v.realtime.vol_l=vol >> 1;
				v.realtime.vol_r=-(vol >> 1);
			}
				
		} else  {

			v.realtime.vol_l=vol;
			v.realtime.vol_r=0;
		}
		
		if (v.firsttime) {
			//old_vol may contain anything...			
			
			Sint32 val=sample_manager->get_data( v.sample.sample, v.realtime.current_index_fp>>MIXING_FRAC_BITS, 0 );
			if (val<0) val=-val;
			
			v.realtime.first_sample_rampup=(val>FIRST_SAMPLE_DECLICK_THRESHOLD);
			
			v.realtime.old_vol_l=v.realtime.vol_l;
			v.realtime.old_vol_r=v.realtime.vol_r;
				
			v.firsttime=false;
			
		}
		
		if (v.fx.filter.enabled)
			process_filter(i);
		process_voice(i,p_frames); //la la la
		voices_used++;
		
	}

	/* run declicker */
	
	if (data.declicker_enabled) {
		
		Sint32 *target_buff=&data.mixdown_buffer[data.mixdown_pos];
		
		for (int i=0;i<p_frames;i++) {
			

			int dpos=(data.declicker_pos & DECLICKER_BUFFER_MASK) << 1;
			
			target_buff[i<<1]+=data.declicker_rbuffer[ dpos ];
			target_buff[(i<<1)+1]+=data.declicker_rbuffer[ dpos+1 ];
			data.declicker_rbuffer[ dpos ]=0;
			data.declicker_rbuffer[ dpos+1 ]=0;
			data.declicker_pos++;
					
		}
	}
	
	if (data.use_effects) {
		process_chorus(p_frames);
		reverb.set_mix_rate( data.mixfreq_current );
		reverb.process( data.reverb_buffer, &data.mixdown_buffer[data.mixdown_pos],p_frames);
	}
	
	Sint32 *target_buff=&data.mixdown_buffer[data.mixdown_pos];
	
	for (int i=0;i<(p_frames<<1);i++) {
			
		target_buff[i]*=data.preamp;
		
		if (data.clipping_enabled) {
		
			if (target_buff[i]<(-0x7FFFFFL)) {
				target_buff[i]=-0x7FFFFFL;
		
			} else if (target_buff[i]>0x7FFFFFL) {
				target_buff[i]=0x7FFFFFL;
		
			}
			
		}		
	}
	
	data.voices_used=voices_used;
}

void Mixer_Soft::downsample_buffer(int p_frames) { //frames that must be NORMALLY processed
	

	Sint32 *oversmp_ptr=&data.mixdown_buffer[0];
	
	for(int i=0;i<p_frames;i++) {
		int src_l=i<<2;
		int src_r=src_l+1;
		int dst_l=i<<1;
		int dst_r=dst_l+1;
			
		//oversmp_ptr[dst_l]=(oversmp_ptr[src_l]>>1)+(oversmp_ptr[src_l+2]>>1);
		//oversmp_ptr[dst_r]=(oversmp_ptr[src_r]>>1)+(oversmp_ptr[src_r+2]>>1);
		//oversmp_ptr[dst_l]=oversmp_ptr[src_l];
		//oversmp_ptr[dst_r]=oversmp_ptr[src_r];
		
		
		
		
		Sint32 val_l=oversmp_ptr[src_l];
		Sint32 val_r=oversmp_ptr[src_r];
				
		oversmp_ptr[dst_l]= (data.ovrs_h[CHAN_LEFT].h2 >> 2)  + (data.ovrs_h[CHAN_LEFT].h1 >>1) +  (val_l>>2);
		data.ovrs_h[CHAN_LEFT].h2=val_l;
		data.ovrs_h[CHAN_LEFT].h1=oversmp_ptr[src_l+2];
		
		oversmp_ptr[dst_r]= (data.ovrs_h[CHAN_RIGHT].h2 >> 2)  + (data.ovrs_h[CHAN_RIGHT].h1 >>1) +  (val_r>>2);
		data.ovrs_h[CHAN_RIGHT].h2=val_r;
		data.ovrs_h[CHAN_RIGHT].h1=oversmp_ptr[src_r+2];
		
	}
	
}

Sint32 Mixer_Soft::process(Sint32 p_frames) {

	Sint32 total=p_frames;
	if (total>MIXDOWN_LENGTH)
		total=MIXDOWN_LENGTH;
	Sint32 todo=total;

	data.mixdown_pos=0;

	int zero_count=data.mix_stereo?(total*2):total;
	
	if (data.oversampling) {
		
		data.oversample_current=true;
		data.mixfreq_current=data.mix_freq<<1;
		zero_count*=2;
	
	} else {
		
		data.oversample_current=false;
		data.mixfreq_current=data.mix_freq;
		
	}

	for (int i=0;i<zero_count;i++) {
		data.mixdown_buffer[i]=0;	
	}
	
	while (todo) {


		
		if (!callback.frames_remaining) {
			if (callback.func)
				callback.func(callback.userdata);
			Sint64 interval=callback.interval;
			interval*=data.mix_freq;
			interval/=1000000;
			
			callback.frames_remaining=(Sint32)interval;
			callback.frames_total=callback.frames_remaining;
			
		}
		
		Sint32 subtodo=(todo>callback.frames_remaining)?callback.frames_remaining:todo;

	
		process_internal(subtodo);
		data.mixdown_pos+=subtodo*(data.mix_stereo?2:1)*(data.oversample_current?2:1);	
		todo-=subtodo;
		callback.frames_remaining-=subtodo;
		
	}
	if (data.oversample_current)
		downsample_buffer( total );
	return total;
}


/* Voice Stuff */

void Mixer_Soft::Voice::clear() {

	active=false;
	ZERO_STRUCT( sample );
	sample.sample=Sample_ID(); /* empty sample, invalid */
	ZERO_STRUCT( amp );
	ZERO_STRUCT( fx );
	ZERO_STRUCT( realtime );
	firsttime=true;

}


Mixer_Soft::Voice::Voice() {

	clear();
}


void Mixer_Soft::setup_voice(int p_voice_index,Sample_ID p_sample_id,Sint32 p_start_index) {

	
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	if (!sample_manager->check(p_sample_id)) {
		ERR_PRINT("Invalid Sample ID!");
		return;
	}
		
	if (p_start_index<0) {
         	ERR_PRINT("start index < 0???");
		return;
	}

	if (voice_array[p_voice_index].active)
		stop_voice( p_voice_index );
	
	Voice &v=voice_array[p_voice_index];
	v.sample.sample=p_sample_id;
	v.sample.c5_freq=sample_manager->get_c5_freq(v.sample.sample);
	v.realtime.vol_l=VOL_MAX;
	v.realtime.vol_r=VOL_MAX;
	v.realtime.backwards_play=false;
	v.realtime.current_index_fp=(Sint64)p_start_index<<MIXING_FRAC_BITS;
	v.realtime.increment_index_fp=0;

	Sint32 size=sample_manager->get_size(v.sample.sample);
	
		
	if (p_start_index>=size) {

		printf("wtf ssize %i, p_start at %i\n",size,p_start_index);
		if (sample_manager->get_loop_type(v.sample.sample)==LOOP_NONE) {
			return;
		} else if (sample_manager->get_loop_type(v.sample.sample)==LOOP_FORWARD) {
			p_start_index=sample_manager->get_loop_begin( v.sample.sample );
		} else {
			
			//?		
		}
	}

	
	v.realtime.filter.h1[0]=0;
	v.realtime.filter.h2[0]=0;
	v.realtime.filter.h1[1]=0;
	v.realtime.filter.h2[1]=0;
	v.fx.filter.enabled=false;
	v.fx.filter.old_enabled=false;
	v.fx.filter.old_c1=0;
	v.fx.filter.old_c2=0;
	v.fx.filter.old_c3=0;
	v.active=true;
	v.firsttime=true;
	//printf("start_voice %i\n",p_voice_index);
	
	
}

void Mixer_Soft::stop_voice(int p_voice_index) {
	
	if (!voice_array[p_voice_index].active)
		return;
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	
	if (data.declicker_enabled && voice_array[p_voice_index].realtime.increment_index_fp!=0) {
		for (int i=0;i<(DECLICKER_FADE_SIZE*2);i++) {
			data.declicker_fade[i]=0;
			
		}
		
		process_voice( p_voice_index, DECLICKER_FADE_SIZE,&data.declicker_fade[0]);
		
		for (int i=0;i<DECLICKER_FADE_SIZE;i++) {
			
			int dpos=((data.declicker_pos+i) & DECLICKER_BUFFER_MASK) << 1;
			int inv=DECLICKER_FADE_SIZE-i;

			
			data.declicker_rbuffer[ dpos ] += (data.declicker_fade [ i<<1] >> (DECLICKER_FADE_BITS))*inv;
			data.declicker_rbuffer[ dpos +1] += (data.declicker_fade [ (i<<1) +1] >> DECLICKER_FADE_BITS)*inv;
		}
	}
	
	voice_array[p_voice_index].active=false;
	//printf("end voice %i (stop)\n",p_voice_index);
}
void Mixer_Soft::set_voice_frequency(int p_voice_index,Sint32 p_freq) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	voice_array[p_voice_index].sample.current_freq_fp=p_freq;
	
}
void Mixer_Soft::set_voice_panning(int p_voice_index,int p_pan)  {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	if (p_pan!=PAN_SURROUND) {
		//something causes clicks at the topmost pans, and i can't find out why
		if (p_pan<(PAN_LEFT+1))
			p_pan=(PAN_LEFT+1);
		else if (p_pan>(PAN_RIGHT-1))
			p_pan=PAN_RIGHT-1;
	} else {
		

	}
		
		
	
	voice_array[p_voice_index].amp.pan=p_pan;
}
void Mixer_Soft::set_voice_volume(int p_voice_index,int p_vol) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	
	
	ERR_FAIL_INDEX(p_vol,VOL_MAX+1);
	
	//if (p_vol>VOL_MAX) //@ TODO must recompute this or something :( working with ints kinda sucks..
	//printf("volume for %i is %i\n",p_voice_index,p_vol);
	Uint32 vol=p_vol;

	voice_array[p_voice_index].amp.volume=vol;	
}
/*
void Mixer_Soft::set_voice_filter(int p_voice_index,Sint32 p_coeff1,Sint32 p_coeff2,Sint32 p_coeff3,bool p_enabled) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	voice_array[p_voice_index].fx.filter.enabled=p_enabled;
	voice_array[p_voice_index].fx.filter.c1=p_coeff1;
	voice_array[p_voice_index].fx.filter.c2=p_coeff2;
	voice_array[p_voice_index].fx.filter.c3=p_coeff3;
	
}
				   */
void Mixer_Soft::set_voice_reverb_send(int p_voice_index,int p_reverb) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	voice_array[p_voice_index].fx.reverb_send=p_reverb;
	
}
void Mixer_Soft::set_voice_chorus_send(int p_voice_index,int p_chorus) { /* 0 - 255 */
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return;
	}
	
	voice_array[p_voice_index].fx.chorus_send=p_chorus;
}

/* Voice Info retrieving */	

Sint32 Mixer_Soft::get_voice_sample_pos_index(int p_voice_index) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return -1;
	}

	if (!voice_array[p_voice_index].active) {
		ERR_PRINT("Voice not configured!");
		return -1;
	}
			
	return voice_array[p_voice_index].realtime.current_index_fp>>MIXING_FRAC_BITS;
}

int Mixer_Soft::get_voice_panning(int p_voice_index) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return -1;
	}
	
	if (!voice_array[p_voice_index].active) {
		ERR_PRINT("Voice not configured!");
		return -1;
	}
	
	return voice_array[p_voice_index].amp.pan;
}

int Mixer_Soft::get_voice_volume(int p_voice_index) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return -1;
	}
	
	if (!voice_array[p_voice_index].active) {
		ERR_PRINT("Voice not configured!");
		return -1;
	}
	
	return voice_array[p_voice_index].amp.volume;
}

Sample_ID Mixer_Soft::get_voice_sample_id(int p_voice_index) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return Sample_ID();
	}
	
	if (!voice_array[p_voice_index].active) {
		ERR_PRINT("Voice not configured!");
		return Sample_ID();
	}
	
	return voice_array[p_voice_index].sample.sample;
}

bool Mixer_Soft::is_voice_active(int p_voice_index) {
	
	if (p_voice_index<0 || p_voice_index>=MAX_VIRTUAL_VOICES) {
		ERR_PRINT("Invalid voice index!");
		return false;
	}
	
	return (voice_array[p_voice_index].active);
}

int Mixer_Soft::get_active_voice_count() {

	return data.voices_used;
}

int Mixer_Soft::get_total_voice_count() {
	
	return MAX_VIRTUAL_VOICES;	
}

void Mixer_Soft::set_use_declicker(bool p_use) {
	
	data.declicker_enabled=p_use;
	if (p_use) {
		
			
	}
}
bool Mixer_Soft::use_declicker() {
	
	return data.declicker_enabled;
}


void Mixer_Soft::set_use_volume_ramp(bool p_use) {
	
	data.ramp_volume=p_use;
}
bool Mixer_Soft::use_volume_ramp() {
	
	return data.ramp_volume;
}

void Mixer_Soft::set_use_oversampling(bool p_use) {
	
	data.oversampling=p_use;
	
}
bool Mixer_Soft::use_oversampling() {
	
	return data.oversampling;
	
}

void Mixer_Soft::set_use_clipping(bool p_use) {
	
	data.clipping_enabled=p_use;
	
}
bool Mixer_Soft::use_clipping() {
	
	return data.clipping_enabled;
	
}

void Mixer_Soft::set_use_effects(bool p_use) {
	
	data.use_effects=p_use;
	
}
bool Mixer_Soft::use_effects() {
	
	return data.use_effects;
	
}

void Mixer_Soft::set_use_filters(bool p_use) {
	
	data.filters=p_use;
}
bool Mixer_Soft::use_filters() {
	
	return data.filters;
}

void Mixer_Soft::set_reverb_mode(ReverbMode p_mode) {
	
	reverb.set_mode(p_mode);	
}



void Mixer_Soft::set_chorus_params(unsigned int p_delay_ms,unsigned int p_separation_ms,unsigned int p_depth_ms10,unsigned int p_speed_hz10) {
	
	chorus.delay_ms=p_delay_ms;
	chorus.separation_ms=p_separation_ms;
	chorus.depth_ms10=p_depth_ms10;
	chorus.speed_hz10=p_speed_hz10;
	
}

void Mixer_Soft::set_global_volume(Uint16 p_global) {
	
	data.global_volume=p_global;
}
Uint16 Mixer_Soft::get_global_volume() {
	
	return data.global_volume;
}

Mixer_Soft::Mixer_Soft() {

	sample_manager=SampleManager::get_singleton();
		
	callback.userdata=0;
	callback.func=0;
	callback.interval=128;
	callback.frames_remaining=0;
	
	data.mix_freq=44100;
	data.mix_stereo=true;
	data.mix_16bit=true;
	data.interpolation_type=INTERPOLATION_LINEAR;
	data.voices_used=0;
	data.preamp=255;
	data.ramp_volume=true;
	data.filters=true;
	data.oversampling=false;
	data.oversample_current=false;
	data.mixfreq_current=false;
	data.use_effects=true;
	data.declicker_enabled=true;
	data.global_volume=127;
	data.declicker_pos=0;
	data.clipping_enabled=true;
	
	for (int i=0;i<(MIXDOWN_BUFFER_SIZE*2);i++) {
		
		data.mixdown_buffer[i]=0;
	}
	for (int i=0;i<(CHORUS_RINGBUFF_SIZE*2);i++) {
		
		chorus.ringbuffer[i]=0;
	}
	for (int i=0;i<(DECLICKER_BUFFER_SIZE*2);i++) {
		
		data.declicker_rbuffer[i]=0;
	}
	
	chorus.ringbuffer_pos=0;
	chorus.osc_pos=0;
	chorus.delay_ms=6;
	chorus.separation_ms=3;
	chorus.depth_ms10=6,
	chorus.speed_hz10=5;
	
	init_cubic_lut();	
}


Mixer_Soft::~Mixer_Soft()
{
}

