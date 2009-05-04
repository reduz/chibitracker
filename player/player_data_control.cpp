/***************************************************************************
                          player_data_control.cpp  -  description
                             -------------------
    begin                : Sun Mar 18 2001
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

#include "player_data.h"

#include <stdio.h>

void Player::play_start_pattern(int p_pattern) {

	play_start(p_pattern,-1,-1);
}

void Player::play_start_song() {

	play_start(-1,-1,-1);
}

void Player::play_start_song_from_order(int p_order) {

	play_start(-1,p_order,-1);
}

void Player::play_start_song_from_order_and_row(int p_order,int p_row) {

	play_start(-1,p_order,p_row);
}

void Player::play_start(int p_pattern, int p_order, int p_row,bool p_lock) {


	if (control.play_mode!=PLAY_NOTHING) play_stop();

	if (p_lock)
		AudioLock::begin();

	reset();

        if (p_pattern!=-1) {

		control.play_mode=PLAY_PATTERN;
		control.position.current_pattern=p_pattern;
		control.position.current_row=(p_row!=-1)?p_row:0;

	} else {
	
		control.position.current_order=get_song_next_order_idx(song,(p_order==-1)?p_order:p_order-1);
		if (control.position.current_order!=-1) {

			control.play_mode=PLAY_SONG;
			control.position.current_pattern=song->get_order(control.position.current_order);
			control.position.current_row=(p_row!=-1)?p_row:0;
		} 
	}


	control.reached_end=(control.play_mode==PLAY_NOTHING);
	
	
	if (p_lock)
		AudioLock::end();
}

void Player::play_stop() {

	int i;

	AudioLock::begin();

	control.play_mode=PLAY_NOTHING;

	for (i=0;i<control.max_voices;i++) {

		voice[i].reset();
		mixer->stop_voice(i);
	}

	for (i=0;i<Pattern::WIDTH;i++) {

		control.channel[i].reset();
	}

	reset();
	AudioLock::end();
}

void Player::play_note(int p_channel,Note note,bool p_reserve) {


	AudioLock::begin();

        if (control.play_mode==PLAY_NOTHING) {

		control.ticks_counter=0;
	}

	/*control.channel[p_channel].reset();
	control.channel[p_channel].channel_volume=song->get_channel_volume(p_channel);
	control.channel[p_channel].channel_panning=((int)song->get_channel_pan( p_channel)*255/64);*/
	if (p_reserve) {
		control.channel[p_channel].mute=false;
		control.channel[p_channel].reserved=true;
	} else {
		
		control.channel[p_channel].reserved=false;
		
	}
	process_note(p_channel,note);


	AudioLock::end();
}


int Player::get_voice_volume(int p_voice) {
	
	return voice[p_voice].display_volume;
}


int Player::get_voice_envelope_pos(int p_voice,Envelope *p_envelope) {

	int i,tmp_index=-1;

	i=p_voice;


	AudioLock::begin();


	if ((song->has_instruments()) && (voice[i].instrument_ptr!=NULL) && (voice[i].fadeout_volume>0)) {

		if ((p_envelope==voice[i].instrument_ptr->get_volume_envelope()) && (voice[i].instrument_ptr->get_volume_envelope()->is_enabled())) {

			tmp_index=voice[i].volume_envelope_ctrl.pos_index;
		}

		if ((p_envelope==voice[i].instrument_ptr->get_pan_envelope()) && (voice[i].instrument_ptr->get_pan_envelope()->is_enabled())) {

			tmp_index=voice[i].panning_envelope_ctrl.pos_index;
		}

		if ((p_envelope==voice[i].instrument_ptr->get_pitch_filter_envelope()) && (voice[i].instrument_ptr->get_pitch_filter_envelope()->is_enabled())) {
		

			tmp_index=voice[i].pitch_envelope_ctrl.pos_index;
		}

	}

	AudioLock::end();

	return tmp_index;
}

void Player::goto_next_order() {


	if (control.play_mode!=PLAY_SONG) return;

	AudioLock::begin();

	control.position.current_row=0;


	control.position.current_order=get_song_next_order_idx(song, control.position.current_order);



	if (control.position.current_order==-1) {

         	reset();	
	}

	control.position.current_pattern=song->get_order(control.position.current_order); 	
	AudioLock::end();

}
void Player::goto_previous_order() {

	if (control.play_mode!=PLAY_SONG) return;

	AudioLock::begin();

	int next_order,current_order;

	control.position.current_row=0;

	current_order=control.position.current_order;

	next_order=get_song_next_order_idx(song, current_order);

	while ((next_order!=control.position.current_order) && (next_order!=-1)) {

		current_order=next_order;
		next_order=get_song_next_order_idx(song, current_order);
	}

	if (next_order==-1) {

         	reset();	
	} else {

		control.position.current_order=current_order;
	        control.position.current_pattern=song->get_order(control.position.current_order);

	}

	AudioLock::end();

}

int Player::get_channel_voice(int p_channel) {

	if (control.channel[p_channel].slave_voice==NULL) return -1;
	else return control.channel[p_channel].slave_voice_index;
}

const char* Player::get_voice_sample_name(int p_voice) {

	const char *name;

	AudioLock::begin();

	if (!voice[p_voice].sample_ptr) name=voice[p_voice].sample_ptr->get_name();

	AudioLock::end();

	return name;

}


bool Player::is_voice_active(int p_voice) {

	return !( ((voice[p_voice].kick==KICK_NOTHING)||(voice[p_voice].kick==KICK_ENVELOPE))&&!mixer->is_voice_active(p_voice) ); 
		
}		
		


int Player::get_voice_envelope_pos(int p_voice,Instrument::EnvelopeType p_env_type) {
	
	if (!is_voice_active(p_voice))
		return -1;
	
	Voice_Control::Envelope_Control *env=0;
	
	switch (p_env_type) {
		
		case Instrument::VOLUME_ENVELOPE: env=&voice[p_voice].volume_envelope_ctrl; break;
		case Instrument::PAN_ENVELOPE: env=&voice[p_voice].panning_envelope_ctrl; break;
		case Instrument::PITCH_ENVELOPE: env=&voice[p_voice].pitch_envelope_ctrl; break;
	
	}
	
	if (!env)
		return -1;
	
	if (!env->active || env->terminated)
		return -1;
	
	return env->pos_index;
}


Envelope* Player::get_voice_envelope(int p_voice,Instrument::EnvelopeType p_env_type) {
	
	Instrument *ins=voice[p_voice].instrument_ptr;
	
	if (!ins)
		return 0;
	
	switch( p_env_type ) {
		
		
		case Instrument::VOLUME_ENVELOPE: return ins->get_volume_envelope(); 
		case Instrument::PAN_ENVELOPE: return ins->get_pan_envelope(); 
		case Instrument::PITCH_ENVELOPE: return ins->get_pitch_filter_envelope();
	};
	
	return 0;
		
}

const char * Player::get_voice_instrument_name(int p_voice) {



	const char *name;

	AudioLock::begin();

	if (voice[p_voice].instrument_ptr!=NULL) name=voice[p_voice].instrument_ptr->get_name();

	AudioLock::end();

	return name;

}
void Player::set_filters_enabled(bool p_enable){

	control.filters=p_enable;
}

int Player::get_voice_sample_index(int p_voice) {

	return voice[p_voice].sample_index;
}
