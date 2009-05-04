//
// C++ Implementation: instrument
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "instrument.h"
#include "song.h"
#include "note.h"
#include "error_macros.h"


const char *Instrument::get_name() {

	return name;

}
void Instrument::set_name(const char *p_name) {
	
	
	if (p_name==NULL) {
		name[0]=0;
		return;
	}
	
	
	bool done=false;
	for (int i=0;i<MAX_NAME_LEN;i++) {
		
		
		name[i]=done?0:p_name[i];
		if (!done && p_name[i]==0)
			done=true;
	}
	
	name[MAX_NAME_LEN-1]=0; /* just in case */
	
	
}

void Instrument::set_sample_number(Uint8 p_note,Uint8 p_sample_id) {
	
	ERR_FAIL_COND(p_note>=Note::NOTES);
	ERR_FAIL_COND(p_sample_id>Song::MAX_SAMPLES && p_sample_id!=Note::EMPTY);
	data.sample_number[p_note]=p_sample_id;
	
	
}
Uint8 Instrument::get_sample_number(Uint8 p_note) {
	
	ERR_FAIL_COND_V(p_note>=Note::NOTES,0);
	return data.sample_number[p_note];
}

void Instrument::set_note_number(Uint8 p_note,Uint8 p_note_id) {
	
	ERR_FAIL_COND(p_note>=Note::NOTES);
	ERR_FAIL_COND(p_note_id>=Note::NOTES && p_note_id!=Note::EMPTY);
	data.note_number[p_note]=p_note_id;
	
}
Uint8 Instrument::get_note_number(Uint8 p_note) {
	
	ERR_FAIL_COND_V(p_note>=Note::NOTES,0);
	return data.note_number[p_note];
	
}

void Instrument::set_NNA_type(NNA_Type p_NNA_type) {
	
	data.NNA_type=p_NNA_type;
}
Instrument::NNA_Type Instrument::get_NNA_type() {
	
	return data.NNA_type;
}

void Instrument::set_DC_type(DC_Type p_DC_type) {
	
	data.DC_type=p_DC_type;
}
Instrument::DC_Type Instrument::get_DC_type() {
	
	return data.DC_type;
	
}
	
void Instrument::set_DC_action(DC_Action p_DC_action) {
	
	data.DC_action=p_DC_action;
}
Instrument::DC_Action Instrument::get_DC_action() {
	
	return data.DC_action;
}

/* Volume */	

void Instrument::set_volume_global_amount(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>MAX_VOLUME);
	data.volume.global_amount=p_amount;
	
}
Uint8 Instrument::get_volume_global_amount() {
	
	return data.volume.global_amount;
}

void Instrument::set_volume_fadeout(Uint16 p_amount) {
	ERR_FAIL_COND(p_amount>MAX_FADEOUT);
	data.volume.fadeout=p_amount; 
}
Uint16 Instrument::get_volume_fadeout() {
	
	return data.volume.fadeout;
}
void Instrument::set_volume_random_variation(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>MAX_VOLUME_RANDOM);
	data.volume.random_variation=p_amount;
}
Uint8 Instrument::get_volume_random_variation() {
	
	return data.volume.random_variation;
}
	
/* Panning */

void Instrument::set_pan_default_amount(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>MAX_PAN);
	data.pan.default_amount=p_amount;
}
Uint8 Instrument::get_pan_default_amount() {
	
	return data.pan.default_amount;
}

void Instrument::set_pan_default_enabled(bool p_enabled) {
	
	data.pan.use_default=p_enabled;
}
bool Instrument::is_pan_default_enabled() {
	
	return data.pan.use_default;
	
}

void Instrument::set_pan_pitch_separation(Sint8 p_amount) {
	
	ERR_FAIL_COND(p_amount<-32);
	ERR_FAIL_COND(p_amount>32);
	data.pan.pitch_separation=p_amount;
}
Sint8 Instrument::get_pan_pitch_separation() {
	
	return data.pan.pitch_separation;
}

void Instrument::set_pan_pitch_center(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>=Note::NOTES);
	data.pan.pitch_center=p_amount;
}
Uint8 Instrument::get_pan_pitch_center() {
	
	return data.pan.pitch_center;
}

void Instrument::set_pan_random_variation(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>MAX_PAN_RANDOM);
	data.pan.random_variation=p_amount;
}
Uint8 Instrument::get_pan_random_variation() {
	
	return data.pan.random_variation;
}

/* Pitch / Filter */

void Instrument::set_pitch_use_as_filter(bool p_enabled) {
	
	data.pitch.use_as_filter=p_enabled;
}
bool Instrument::is_pitch_use_as_filter() {
	
	return data.pitch.use_as_filter;
}

void Instrument::set_filter_use_default_cutoff(bool p_enabled) {
	
	data.pitch.use_default_cutoff=p_enabled;

}
bool Instrument::filter_use_default_cutoff() {
	
	return data.pitch.use_default_cutoff;
}

void Instrument::set_filter_default_cutoff(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>MAX_FILTER_CUTOFF);
	data.pitch.default_cutoff=p_amount;
}
Uint8 Instrument::get_filter_default_cutoff() {
	
	return data.pitch.default_cutoff;
}

void Instrument::set_filter_use_default_resonance(bool p_enabled) {
	
	data.pitch.use_default_resonance=p_enabled;
}
bool Instrument::filter_use_default_resonance() {
	
	return data.pitch.use_default_resonance;
}

void Instrument::set_filter_default_resonance(Uint8 p_amount) {
	
	ERR_FAIL_COND(p_amount>MAX_FILTER_RESONANCE);
	data.pitch.default_resonance=p_amount;
	
}
Uint8 Instrument::get_filter_default_resonance() {
	
	return data.pitch.default_resonance;
}

/* Envelopes */


Envelope* Instrument::get_volume_envelope() {
	
	return &data.volume.envelope;
}
Envelope* Instrument::get_pan_envelope() {
	
	return &data.pan.envelope;	
}
Envelope* Instrument::get_pitch_filter_envelope() {
	
	return &data.pitch.envelope;	
	
	
}


void Instrument::reset() {
	
	name[0]=0;
	
	data.NNA_type=NNA_NOTE_CUT;
	data.DC_action=DCA_NOTE_CUT;
	data.DC_type=DCT_DISABLED;
	
	for (int i=0;i<Note::NOTES;i++) {
		
		data.sample_number[i]=Note::EMPTY;
		data.note_number[i]=i;
	}
	
	data.volume.envelope.reset();
	data.volume.envelope.set_max(64);
	data.volume.envelope.set_min(0);
	data.volume.envelope.add_position(0,64,false);
	data.volume.envelope.add_position(30,64,false);
	
	data.volume.global_amount=MAX_VOLUME;
	data.volume.fadeout=0;
	data.volume.random_variation=0;
	
	data.pan.envelope.reset();
	data.pan.envelope.set_max(32);
	data.pan.envelope.set_min(-32);
	data.pan.envelope.add_position(0,0,false);
	data.pan.envelope.add_position(30,0,false);
	
	data.pan.default_amount=32;
	data.pan.pitch_center=48;
	data.pan.pitch_separation=0;
	data.pan.use_default=false;
	data.pan.random_variation=0;
	
	
	data.pitch.envelope.reset();
	data.pitch.envelope.set_max(32);
	data.pitch.envelope.set_min(-32);
	data.pitch.envelope.add_position(0,0,false);
	data.pitch.envelope.add_position(30,0,false);
	data.pitch.use_as_filter=false;
	data.pitch.use_default_cutoff=false;
	data.pitch.use_default_resonance=false;
	data.pitch.default_cutoff=0;
	data.pitch.default_resonance=0;
	
}

bool Instrument::is_empty() {
	
	bool has_sample=false;
	
	for (int i=0;i<Note::NOTES;i++) {
		
		if (data.sample_number[i]!=Note::EMPTY) {
			
			has_sample=true;
			break;
		}
	}

	return !has_sample;
}

Instrument::Instrument() {
	
	reset();
	
}

