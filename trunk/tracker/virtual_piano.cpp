/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : reduzio@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
                          VirtualPiano.cpp  -  description
                             -------------------
    begin                : Sun Apr 1 2001
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

#include "virtual_piano.h"
#include <math.h>

int VirtualPiano::find_empty_channel() {

	int i;

	for (i=(Pattern::WIDTH-1);i>=0;i--) {

         	if (channel_per_note[i]==-1) return i;
	}

	return 0; // let's empty channel 0
}

int VirtualPiano::find_note_in_channel(int p_note) {

	int i;

	for (i=0;i<Pattern::WIDTH;i++) {

         	if (channel_per_note[i]==p_note) return i;
	}

	return -1;
}


void VirtualPiano::instrument_set(int p_instrument_index) {

	current_instrument=p_instrument_index;
}

void VirtualPiano::instrument_press_key(int p_note,int p_volume) {

	if ((p_note>=Note::NOTES) || (p_note<0)) return;

        if (key_pressed[p_note]) return; // avoid extra keypresses

	key_pressed[p_note]=true;

	int channel;

	Note note;

	note.clear();
	note.note=p_note;
        note.volume=p_volume;
	note.instrument=current_instrument;
	//printf("using instrument %i\n",current_instrument);


        channel=find_empty_channel();

	channel_per_note[channel]=p_note;

	player->play_note(channel,note,true);

}

void VirtualPiano::instrument_stop_key(int p_note) {

	if ((p_note>=Note::NOTES) || (p_note<0)) return;

	int channel;
	Note note;

	note.clear();
	note.note=Note::OFF;

	key_pressed[p_note]=false;

	if ((channel=find_note_in_channel(p_note))!=-1) {

		channel_per_note[channel]=-1;
		player->play_note(channel,note,true);
	}
}

void VirtualPiano::instrument_stop_all() {

	int i;

	for (i=0;i<Pattern::WIDTH;i++) {

		if (channel_per_note[i]!=-1) {

			Note note;
			note.clear();
			note.note=Note::OFF;

			player->play_note(i,note,true);
			channel_per_note[i]=-1;

		}
	}

       	for (i=0;i<Note::NOTES;i++) key_pressed[i]=false;
}

void VirtualPiano::sample_set(Sample_ID p_sample) {

	sample_stop_all();

	sample=p_sample;
}

void VirtualPiano::sample_press_key(int p_note) {


	if (key_pressed[p_note]) return; // avoid extra keypresses

	key_pressed[p_note]=true;

	latest_key_pressed=p_note;

	if (sample.is_null())
		return;
	
	
	AudioLock::begin();
	
	if (sample_playing && sample_voice>=0) {
		
		mixer->stop_voice( sample_voice );
		sample_playing=0;
		sample_voice=-1;
	}
	
	sample_voice=mixer->get_total_voice_count()-1;
	/*
	for (int i=0;i<mixer->get_total_voice_count();i++) {
		
		if (!mixer->is_voice_active( i )) {
			sample_voice=i;
			break;
		}
	}
	if (sample_voice==-1) {
		AudioLock::end();
		return;
	} */
	float freq = 440.0 * powf(2,((p_note-69.0)/12.0));
	
	mixer->setup_voice( sample_voice, sample, 0 );
	mixer->set_voice_frequency( sample_voice, (int)(freq*(float)(1<<Mixer::FREQUENCY_BITS)) );
	mixer->set_voice_volume( sample_voice, VOL_MAX );
	mixer->set_voice_panning( sample_voice, PAN_CENTER );
	sample_playing=true;

	AudioLock::end();
	
}


void VirtualPiano::sample_stop(int p_note) {

	key_pressed[p_note]=false;

	if (latest_key_pressed!=p_note) return;

	//sample_stop_all();

}

void VirtualPiano::sample_stop_all() {

	int i;

	if (sample.is_null() || sample_voice<0)
		return;
	
	AudioLock::begin();
	
	mixer->stop_voice( sample_voice );
	sample_voice=-1;
	sample_playing=false;

       	for (i=0;i<Note::NOTES;i++) key_pressed[i]=false;
        
	AudioLock::end();
	
}



VirtualPiano::VirtualPiano(Player *p_player,Mixer *p_mixer){

	int i;

	for (i=0;i<Pattern::WIDTH;i++) channel_per_note[i]=-1;
	for (i=0;i<Note::NOTES;i++) key_pressed[i]=false;
	sample_playing=false;
	player=p_player;
	mixer=p_mixer;
	sample_voice=-1;
}
VirtualPiano::~VirtualPiano(){
}
