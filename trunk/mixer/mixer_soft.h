//
// C++ Interface: mixer_soft
//
// Description: 
//
//                                                                                                           s
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MIXER_SOFT_H
#define MIXER_SOFT_H

#include "mixer/mixer.h"
#include "sample_defs.h"
#include "mixer/sample_manager.h"
#include "mixer/reverb.h"

/**
@author Juan Linietsky
*/
class Mixer_Soft : public Mixer {
protected:
	
	struct Voice {

		/* Control Variables */
			
		bool active;

		struct Sample {
			
			Sample_ID sample;
			Sint32 current_freq_fp;
			Sint32 c5_freq;
		} sample;
		
		struct Amp {
			
			int volume; /* Voice Volume */
			int pan; /* Pannig */
		
		} amp;
		
                struct FX {
			
			
			struct FilterKernel {
				Uint8 cutoff;
				Uint8 resonance;
				int cutoff_cache;
				Uint8 resonance_cache;
				Sint32 mixfreq_cache;
				
				Sint32 old_c1,old_c2,old_c3; //old fixed point filter stuff
				Sint32 c1,c2,c3; //fixed point filter stuff
				bool enabled;
				bool old_enabled;
			} filter;
		
			int reverb_send;
			int chorus_send;
			
		} fx;
		
		/* Realtime Variables */
		
		struct RealTime { /* Info used for realtime processing... */
			
			Sint32 vol_l;
			Sint32 old_vol_l;
			Sint32 vol_r;
			Sint32 old_vol_r;

			/* fixed point */
			bool backwards_play;
			Sint64 current_index_fp;
			Sint64 increment_index_fp;

			struct Filter { //history (stereo)
				Sint64 h1[2],h2[2];
			} filter;
			Sint32 last_frame_l;
			Sint32 last_frame_r;
			
			bool first_sample_rampup;
			
		} realtime;
		
		bool firsttime;
		void clear();
		Voice();
	};

	Voice voice_array[MAX_VIRTUAL_VOICES];	

	struct Callback {
		
		void (*func)(void*);
		void *userdata;
		Sint32 interval;
		Sint32 frames_total;
		Sint32 frames_remaining; //frames remaining before interval completed
	} callback;
	
	struct MixData {
		
		Sint32 mix_freq;
		bool mix_stereo;
		bool mix_16bit;
		InterpolationType interpolation_type;
		bool ramp_volume;
		Sint32 mixdown_buffer[MIXDOWN_BUFFER_SIZE*2]; //oversampling-able
		Sint32 reverb_buffer[MIXDOWN_BUFFER_SIZE*2]; //oversampling-able
		Sint32 declicker_rbuffer[DECLICKER_BUFFER_SIZE*2];
		Sint32 declicker_fade[DECLICKER_FADE_SIZE*2];
		Uint32 declicker_pos;
		
		Uint16 global_volume;
		
		Uint32 mixdown_pos;
		int voices_used;
		Uint16 preamp;	
		bool oversampling;
		bool filters;
		
		bool use_effects;
		Sint32 mixfreq_current;
		bool oversample_current;
		
		bool clipping_enabled;
		bool declicker_enabled;
		
		struct OversampleHistory {
			
			Sint32 h1,h2;
			OversampleHistory() { h1=h2=0; }
		} ovrs_h[2]; //stereo
	} data;
	
	struct Chorus {
		
		Sint32 ringbuffer[CHORUS_RINGBUFF_SIZE*2];		
		unsigned int ringbuffer_pos;
		unsigned int osc_pos;
		
		unsigned int delay_ms;
		unsigned int separation_ms;
		unsigned int depth_ms10;
		unsigned int speed_hz10;
		
	} chorus;
	
	Reverb reverb;
	
	SampleManager *sample_manager;

/* Protected interface for inheritance */
	
	/* External sound driver control */

	
	/* INHERIT AND OVERRIDE THIS FOR IMPLEMENTATION-SPECIFIC OPTIMIZATIONS!! */
	virtual void process_voice(int p_voice_index,Sint32 p_frames,Sint32 *p_auxbuff=0);
	
	void process_internal(Sint32 p_frames);
	void process_filter(int p_voice );
	void prepare_chorus(int p_frames );
	void process_chorus(int p_frames );
	
	void downsample_buffer(int p_frames);//frames that must be NORMALLY processed , not oversampled

public:
		
	/* Callback */	
		
	void set_callback_interval(int p_interval_us); //in usec, for tracker it's 2500000/tempo
	void set_callback(void (*p_callback)(void*),void *p_userdata);
	
	/* Voice Control */
			
	void setup_voice(int p_voice_index,Sample_ID p_sample_id,Sint32 p_start_index) ;
	void stop_voice(int p_voice_index) ;
	void set_voice_frequency(int p_voice_index,Sint32 p_freq) ;
	void set_voice_panning(int p_voice_index,int p_pan) ; /* PAN_LEFT .. PAN_RIGHT , PAN_SURROUND */
	void set_voice_volume(int p_voice_index,int p_vol) ; /* 0 - 1024 */
	
	void set_voice_filter(int p_voice,bool p_enabled,Uint8 p_cutoff, Uint8 p_resonance );
	void set_voice_reverb_send(int p_voice_index,int p_reverb); /* 0 - 255 */
	void set_voice_chorus_send(int p_voice_index,int p_chorus); /* 0 - 255 */
	
	void set_reverb_mode(ReverbMode p_mode);	
	void set_chorus_params(unsigned int p_delay_ms,unsigned int p_separation_ms,unsigned int p_depth_ms10,unsigned int p_speed_hz10);
	
	
	void set_global_volume(Uint16 p_global); // 0-5125 , 128 is normal
	Uint16 get_global_volume();
	
	
	
	/* Voice Info retrieving */	
	
	Sint32 get_voice_sample_pos_index(int p_voice_index);
	int get_voice_panning(int p_voice_index);
	int get_voice_volume(int p_voice_index);
	Sample_ID get_voice_sample_id(int p_voice_index);
	bool is_voice_active(int p_voice_index);	
	int get_active_voice_count();
	int get_total_voice_count();
	/* Processing */
		
	void set_mix_frequency(Sint32 p_mix_frequency);
	void set_mix_stereo(bool p_mix_stereo);
	void set_mix_16bits(bool p_mix_16bits);
	void set_mix_interpolation(InterpolationType p_type);
	InterpolationType get_mix_interpolation() { return data.interpolation_type; }
	
	void set_use_effects(bool p_use);
	bool use_effects();
	
	void set_use_volume_ramp(bool p_use);
	bool use_volume_ramp();
	
	void set_use_clipping(bool p_use);
	bool use_clipping();
	
	void set_use_declicker(bool p_use);
	bool use_declicker();
	
	void set_use_oversampling(bool p_use);
	bool use_oversampling();
	void set_use_filters(bool p_use);
	bool use_filters();
	
	Uint32 get_mix_frequency(); //if mixer is not software, return 0	
		
	Sint32 process(Sint32 p_frames); /* Call this to process N frames, returns how much it was processed */
	Sint32 *get_mixdown_buffer_ptr(); /* retrieve what was mixed */
	
	
	
	Mixer_Soft();
	~Mixer_Soft();

};

#endif
