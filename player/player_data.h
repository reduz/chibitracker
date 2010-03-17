/***************************************************************************
                          player_data.h  -  description
                             -------------------
    begin                : Wed Jan 31 2001
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

#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "config.h"
#include "song/song.h"
#include "mixer/mixer.h"
#include "mixer/audio_lock.h"
#include "player/tables.h"

/**Player Data
  *@author Juan Linietsky
  */

/******************************
 player_data.h
 ------------------------

The player and its data.
I hope you dont get sick reading this
********************************/

//Default pan values
#define PAN_SURROUND 512
#define PAN_RIGHT    255
#define PAN_LEFT    0
#define PAN_CENTER    128



class Player {

	Song *song;

	Mixer *mixer;

        struct Filter_Control {

		Sint32 it_reso;
		Sint32 it_cutoff;
		Sint32 envelope_cutoff;
		Sint32 final_cutoff;
		
		void process();
		void set_filter_parameters(int *p_cutoff,Uint8 *p_reso);

        };

	//tells you if a channel is doing
	//noteoff/notekill/notefade/etc
	enum {
	
		END_NOTE_NOTHING=0,
		END_NOTE_OFF=1,
		END_NOTE_FADE=2,
		END_NOTE_KILL=4
	};
	
	//Tells you what should a channel restart
	enum {
	
		KICK_NOTHING,
		KICK_NOTE,
		KICK_NOTEOFF,
		KICK_ENVELOPE
	};

	enum {
		
		MAX_VOICES=256
	};
	
	struct Channel_Control;

        struct Voice_Control {

		struct Envelope_Control {

			int pos_index;
			int status;
			int value;
			bool sustain_looping;
			bool looping;
			bool terminated;
			bool active;
			bool kill;

		};

                Filter_Control filter;
                Uint16 reverb_send;
                Uint16 chorus_send;

		Instrument* instrument_ptr;
		Sample*     sample_ptr;

//		Sample_Data *sample_data;

		Uint32 period;		

		Sint32 sample_start_index;        /* The starting byte index in the sample */

		bool has_master_channel;
		int master_channel_index;
                int instruement_index;

		int instrument_index;
		int sample_index;
		Sint8 NNA_type;

		int note_end_flags;

		Uint8       sample;       /* which instrument number */

 		Sint16 output_volume;       /* output volume (vol + sampcol + instvol) */
		Sint8  channel_volume;      /* channel's "global" volume */
		Uint16 fadeout_volume;      /* fading volume rate */
		Uint32 total_volume;     /* total volume of channel (before global mixings) */
		Uint8       kick;         /* if true = sample has to be restarted */
	
		Uint8       note;         /* the audible note (as heard, direct rep of period) */

		Sint16       panning;      /* panning position */
		
		Uint8       nna;          /* New note action type + master/slave flags */
		Uint8       volflg;       /* volume envelope settings */
		Uint8       panflg;       /* panning envelope settings */
		Uint8       pitflg;       /* pitch envelope settings */
		Uint8       keyoff;       /* if true = fade out and stuff */
		Sint16       handle;       /* which sample-handle */
		Sint32       start;        /* The start byte index in the sample */
	
	/* Below here is info NOT in MP_CONTROL!! */
		//ENVPR       venv;
		//ENVPR       penv;
		//ENVPR       cenv;

		Envelope_Control volume_envelope_ctrl;
		Envelope_Control panning_envelope_ctrl;
		Envelope_Control pitch_envelope_ctrl;
	
		Uint16       auto_vibrato_pos;      /* autovibrato pos */
		Uint16       auto_vibrato_sweep_pos;      /* autovibrato sweep pos */
		
		Sint16       masterchn;
		Uint16       masterperiod;
	
		Channel_Control* master_channel;       /* index of "master" effects channel */

		void start_envelope(Envelope *p_envelope,Envelope_Control *p_envelope_ctrl,Envelope_Control *p_from_env);
		bool process_envelope(Envelope *p_envelope,Envelope_Control *p_envelope_ctrl);

		Uint16 display_volume;
		
		Voice_Control() {
			
			reset();
		}

		void reset();
	        void update_info_from_master_channel();
		

	};


	struct Channel_Control {

		/* NOTE info */
		Uint8  note;                      /* the audible note as heard, direct rep of period */	
		Uint8  real_note;                 /* the note that indexes the audible */
		Sint32 sample_start_index;        /* The starting byte index in the sample */
		Uint8  old_note;

		Uint8 kick;
		
                Filter_Control filter;
                Uint16 reverb_send;
                Uint16 chorus_send;

		
		int note_end_flags;

		/* INSTRUMENT INFO */

		Instrument* instrument_ptr;
		Sample*     sample_ptr;

		Uint8  instrument_index;
		Uint8  sample_index;
		bool new_instrument;

		/* SAMPLE SPECIFIC INFO */
		Uint32 base_speed;        /* what finetune to use */

		/* INSTRUMENT SPECIFIC INFO */

		Sint8 NNA_type;
		Sint8 duplicate_check_type;
		Sint8 duplicate_check_action;

		bool volume_envelope_on;
		bool panning_envelope_on;
		bool pitch_envelope_on;

                bool has_own_period;

		bool row_has_note;

		/* VOLUME COLUMN */

		Sint16  volume;       /* amiga volume (0 t/m 64) to play the sample at */
		Sint16 aux_volume;
		bool has_own_volume;
		bool mute;
		Sint16 random_volume_variation; /* 0-100 - 100 has no effect */

		/* VOLUME/PAN/PITCH MODIFIERS */

		Sint8	default_volume; // CHANNEL default volume (0-64)
		Sint16  channel_volume; // CHANNEL current volume //chanvol - current!
		Sint16  output_volume; /* output volume (vol + sampcol + instvol) //volume */

		Uint16  fadeout_volume;      /* fading volume rate */

		Uint32 period;       /* period to play the sample at */
		
		/* PAN */

		Sint16  panning;   /* panning position */
                Sint16  channel_panning;
		Sint8   sliding;

		Uint16 aux_period;    /* temporary period */



		/* TIMING */
		Uint8 note_delay;    /* (used for note delay) */		

		/* Slave Voice Control */

		Voice_Control *slave_voice;        /* Audio Slave of current effects control channel */
		
		struct Carry {
			
			Voice_Control::Envelope_Control vol;
			Voice_Control::Envelope_Control pan;
			Voice_Control::Envelope_Control pitch;
			bool maybe;

		} carry;


	
		Uint8       slave_voice_index;     /* Audio Slave of current effects control channel */

		Uint8*      row;          /* row currently playing on this channel */

		/* effect memory variables */

		Uint8 current_command;
		Uint8 current_parameter;
		Uint8 current_volume_command;
		Uint8 current_volume_parameter;
		Uint8 volcol_volume_slide;

		/* Sample Offset */

		Uint32 lo_offset;
		Uint32 hi_offset;

		/* Panbrello waveform */
		Uint8       panbrello_type;     /* current panbrello waveform */
		Uint8       panbrello_position;      /* current panbrello position */
		Sint8       panbrello_speed;      /* "" speed */
		Uint8       panbrello_depth;    /* "" depth */
                Uint8	    panbrello_info;
                /* Arpegio */

		Uint8	     arpegio_info;
                /* Pattern Loop */

		int pattern_loop_position;
		Sint8 pattern_loop_count;

                /* Vibrato */
                bool doing_vibrato;
		Sint8       vibrato_position;       /* current vibrato position */
		Uint8       vibrato_speed;       /* "" speed */
		Uint8       vibrato_depth;     /* "" depth */
                Uint8	    vibrato_type;	
		/* Tremor */
		Sint8       tremor_position;
		Uint8       tremor_speed;    /* s3m tremor ontime/offtime */
		Uint8	    tremor_depth;
		Uint8	    tremor_info;

		/* Tremolo */
		Sint8       tremolo_position;
		Uint8       tremolo_speed;    /* s3m tremor ontime/offtime */
		Uint8	    tremolo_depth;
		Uint8	    tremolo_info;
		Uint8	    tremolo_type;

		/* Retrig */
		Sint8       retrig_counter;  /* retrig value (0 means don't retrig) */
		Uint8       retrig_speed;  /* last used retrig speed */
		Uint8       retrig_volslide;  /* last used retrig slide */

		/* Sample Offset */
		Uint32       sample_offset_hi;     /* last used high order of sample offset */
		Uint16       sample_offset;      /* last used low order of sample-offset (effect 9) */
		Uint16       sample_offset_fine;    /* fine sample offset memory */

		/* Portamento */
		Uint16 slide_to_period; /* period to slide to (with effect 3 or 5) */
		Uint8 portamento_speed;

	        /* Volume Slide */

		Uint8 volume_slide_info;

                /* Channel Volume Slide */

		Uint8 channel_volume_slide_info;

                /* Global Volume Slide */

                Uint8 global_volume_slide_info;

                /* Channel Pan Slide */

		Uint8 channel_pan_slide_info;

		/* Pitch Slide */

		Uint8 pitch_slide_info;
		/* Tempo Slide */

		Uint8 tempo_slide_info;

                /* S effects memory */

                Uint8 current_S_effect;
                Uint8 current_S_data;

		/* Volume column memory */

		Uint8       volume_column_effect_mem;
		Uint8       volume_column_data_mem;
	
		bool reserved;
		
		void reset();
	};
	
	struct Control_Variables { // control variables (dynamic version) of initial variables

		bool reached_end;
				
		char play_mode;
                bool filters;
		int global_volume;
		int speed;
		int tempo;

		int ticks_counter;

		int pattern_delay_1;
		int pattern_delay_2;

		Channel_Control channel[Pattern::WIDTH];

		int max_voices;

		int voices_used; /* reference value */

		bool force_no_nna;
		bool external_vibrato;
		
		struct Position {

			int current_order;
			int current_pattern;
			int current_row;
			int force_next_order;
			bool forbid_jump;
		};

		Uint32 random_seed;

		Position position;
		Position previous_position;

	};


	Voice_Control voice[MAX_VOICES];
		
	Control_Variables control;

	/* VOICE SETUP */
	
	void setup_voices();

	/* MIXER SETUP */
	void handle_tick();
	void update_mixer();
	
	/* NOTE / INSTRUMENT PROCESSING */

	void process_new_note(int p_track,Uint8 p_note);
	bool process_new_instrument(int p_track,Uint8 p_instrument);
	bool process_note_and_instrument(int p_track,int p_note,int p_instrument);

	/* EFFECT PROCESSING */
	void do_effect_S(int p_track);
	void do_panbrello(int p_track);
	void do_global_volume_slide(int p_track);
        void do_tremolo(int p_track);
        void do_retrig(int p_track);
	void do_pan_slide(int p_track);
        void do_channel_volume_slide(int p_track);
        void do_volume_slide(int p_track,int inf);
	void do_pitch_slide_down(int p_track,Uint8 inf);
	void do_pitch_slide_up(int p_track,Uint8 inf);
        void do_tremor(int p_track);
        void do_vibrato(int p_track,bool fine);
        void do_pitch_slide_to_note(int p_track);
        void run_effects(int p_track);
        void run_volume_column_effects(int p_track);
        void pre_process_effects();
        void do_arpegio(int p_track);
	/* NNA */

	void process_NNAs();


	/* MISC UTILS */


        int find_empty_voice();
	void process_volume_column(int p_track,Uint8 p_volume);
        void process_note(int p_track,Note p_note);

        /* TABLES */
        static Uint8 auto_vibrato_table[128];
        static Uint8 vibrato_table[32];
        static Sint8 panbrello_table[256];

	static void callback_function(void *p_userdata);

public:
        //Play modes

	enum {
	
		PLAY_NOTHING =0,
		PLAY_PATTERN =1,
		PLAY_SONG =2
	};


        Uint32 get_frequency(Uint32 period);
	Uint32 get_period(Uint16 note,Sint32 p_c5freq);
	

	int get_current_tempo() { return control.tempo; };
	int get_current_speed() { return control.speed; };

        int get_voices_used() { return control.voices_used;};
	int get_voice_envelope_pos(int p_voice,Envelope *p_envelope);
	int get_voice_amount_limit() { return control.max_voices; };
	void set_voice_amount_limit(int p_limit);
	void set_reserved_voices(int p_amount);
	int get_reserved_voices_amount();
	
	bool is_voice_active(int p_voice);
	int get_channel_voice(int p_channel);
	const char* get_voice_sample_name(int p_voice);
	const char* get_voice_instrument_name(int p_voice);
	Envelope* get_voice_envelope(int p_voice,Instrument::EnvelopeType p_env_type);
	int get_voice_envelope_pos(int p_voice,Instrument::EnvelopeType p_env_type);
	int get_voice_volume(int p_voice);
	
	int get_voice_sample_index(int p_voice);

	void set_virtual_channels(int p_amount);
	int get_virtual_channels() { return control.max_voices; };


	/* Play Info/Position */
        bool is_playing() { return (control.play_mode>0); };
	int get_play_mode() {return (control.play_mode);};
	int get_current_order() { return control.position.current_order; };
	int get_current_row() { return control.position.current_row; };
	int get_current_pattern() { return control.position.current_pattern; };

        void goto_next_order();
        void goto_previous_order();

	void process_tick();


	Mixer* get_mixer_ptr() {

		return mixer;		
	}


	void reset();

	
	
	/* External player control - editor - */

        void play_start_pattern(int p_pattern);
	void play_start_song();
	void play_start_song_from_order(int p_order);
	void play_start_song_from_order_and_row(int p_order,int p_row);
        void play_start(int p_pattern, int p_order, int p_row,bool p_lock=true);

        void play_stop();
        void play_note(int p_channel,Note note,bool p_reserve=false);

        bool reached_end_of_song();
	
        void set_force_no_nna(bool p_force);
        void set_force_external_vibratos(bool p_force);

        void set_filters_enabled(bool p_enable);
        bool are_filters_enabled() { return control.filters; }
        Song *get_song() { return song; };


	Player(Mixer *p_mixer,Song *p_song);
	~Player();
};

#endif
