#ifndef INSTRUMENT_H
#define INSTRUMENT_H


#include "globals/config.h"
#include "globals/mempool_wrapper.h"
#include "song/note.h"
#include "song/envelope.h"

class Instrument {
public:


	enum NNA_Type {

		NNA_NOTE_CUT,
		NNA_NOTE_CONTINUE,
		NNA_NOTE_OFF,
		NNA_NOTE_FADE
	};

	enum DC_Type {

		DCT_DISABLED,
		DCT_NOTE,
		DCT_SAMPLE,
		DCT_INSTRUMENT
	};

	enum DC_Action
	{

		DCA_NOTE_CUT,
		DCA_NOTE_OFF,
		DCA_NOTE_FADE,
	};

	enum EnvelopeType {
          	VOLUME_ENVELOPE,
		PAN_ENVELOPE,
		PITCH_ENVELOPE
	};

	
	enum {
		MAX_NAME_LEN=26,
		MAX_ENVELOPE_NODES=25,
		ENVELOPE_FRAC_BITS=8,
		MAX_VOLUME=128,
		MAX_FADEOUT=256,
		MAX_PAN=128,
		MAX_VOLUME_RANDOM=100,			
		MAX_PAN_RANDOM=64, //what did this guy have inside his head?
		
		MAX_FILTER_CUTOFF=127,
		MAX_FILTER_RESONANCE=127
				
	};


	struct Data {


		Uint8 sample_number[Note::NOTES];
		Uint8 note_number[Note::NOTES];

		NNA_Type NNA_type;
		DC_Type DC_type;
		DC_Action DC_action;

		struct Volume {

			Envelope envelope;
			Uint8 global_amount;
			Uint16 fadeout;
			Uint8 random_variation;

		} volume;

		struct Pan {

			Envelope envelope;
			bool use_default;
			Uint8 default_amount;
			Sint8 pitch_separation;
			Uint8 pitch_center;
			Uint8 random_variation;

		} pan;

		struct Pitch {

			Envelope envelope;
			bool use_as_filter;
			bool use_default_cutoff;
			Uint8 default_cutoff;
			bool use_default_resonance;
			Uint8 default_resonance;
		} pitch;

	};
	
private:



	Data data;
	char name[MAX_NAME_LEN];
	
public:

	/* Instrument General */
	
	const char *get_name();
	void set_name(const char *p_name);
	
	void set_sample_number(Uint8 p_note,Uint8 p_sample_id);
	Uint8 get_sample_number(Uint8 p_note);
	
	void set_note_number(Uint8 p_note,Uint8 p_note_id);
	Uint8 get_note_number(Uint8 p_note);

	void set_NNA_type(NNA_Type p_NNA_type);
	NNA_Type get_NNA_type();

	void set_DC_type(DC_Type p_DC_type);
	DC_Type get_DC_type();
		
	void set_DC_action(DC_Action p_DC_action);
	DC_Action get_DC_action();

	/* Volume */	

	void set_volume_global_amount(Uint8 p_amount);
	Uint8 get_volume_global_amount();

	void set_volume_fadeout(Uint16 p_amount);
	Uint16 get_volume_fadeout();

	void set_volume_random_variation(Uint8 p_amount);
	Uint8 get_volume_random_variation();
		
	/* Panning */

	void set_pan_default_amount(Uint8 p_amount);
	Uint8 get_pan_default_amount();

	void set_pan_default_enabled(bool p_enabled);
	bool is_pan_default_enabled();
	
	void set_pan_pitch_separation(Sint8 p_amount);
	Sint8 get_pan_pitch_separation();
	
 	void set_pan_pitch_center(Uint8 p_amount);
	Uint8 get_pan_pitch_center();

	void set_pan_random_variation(Uint8 p_amount);
	Uint8 get_pan_random_variation();

	/* Pitch / Filter */

	void set_pitch_use_as_filter(bool p_enabled);
	bool is_pitch_use_as_filter();
	
	void set_filter_use_default_cutoff(bool p_enabled);
	bool filter_use_default_cutoff();

	void set_filter_default_cutoff(Uint8 p_amount);
	Uint8 get_filter_default_cutoff();
	
	void set_filter_use_default_resonance(bool p_enabled);
	bool filter_use_default_resonance();

	void set_filter_default_resonance(Uint8 p_amount);
	Uint8 get_filter_default_resonance();

	Envelope* get_volume_envelope();
	Envelope* get_pan_envelope();
	Envelope* get_pitch_filter_envelope();
		
	bool is_empty();
	
	void reset();
	Instrument();

};



#endif


