/***************************************************************************
                          player_data.cpp  -  description
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

#include "player_data.h"

Player::Player(Mixer *p_mixer,Song *p_song){

	song=p_song;
	mixer=p_mixer;
	control.max_voices=p_mixer->get_total_voice_count()-1; //leave one for the sample
	control.force_no_nna=false;
	control.external_vibrato=false;
	control.filters=true;
	control.random_seed=128364; //anything
	control.play_mode=0;
	set_virtual_channels(p_mixer->get_total_voice_count());
	mixer->set_callback( &Player::callback_function, this );
	
	reset();
}
Player::~Player(){
}

void Player::set_virtual_channels(int p_amount) {

	if (p_amount<1) return;
	if (p_amount>mixer->get_total_voice_count())
		return;
	
	control.max_voices=p_amount;

}


void Player::callback_function(void *p_userdata) {
	
	Player*pd=(Player*)p_userdata;
	pd->process_tick();

}

void Player::process_tick() {

	handle_tick();
	mixer->set_callback_interval( 2500000/control.tempo );
}

void Player::reset() {

	if ( mixer==NULL ) return ;
	if ( song==NULL ) return ;

	int i;

	for (i=0;i<control.max_voices;i++) {

         	voice[i].reset();
		mixer->stop_voice(i);
	}
	
	for (i=0;i<Pattern::WIDTH;i++) {

         	control.channel[i].reset();
		control.channel[i].channel_volume=song->get_channel_volume(i);
		control.channel[i].channel_panning=((int)song->get_channel_pan( i)*PAN_RIGHT/64);
		if (song->is_channel_surround(i))
			control.channel[i].channel_panning=PAN_SURROUND;
		control.channel[i].mute=song->is_channel_mute( i );
		control.channel[i].chorus_send=song->get_channel_chorus(i)*0xFF/64;
		control.channel[i].reverb_send=song->get_channel_reverb(i)*0xFF/64;
	}


	control.speed=song->get_speed();
	control.tempo=song->get_tempo();
	control.global_volume=song->get_global_volume();

	control.position.current_pattern=0;
	control.position.current_row=0;
	control.position.current_order=0;
        control.position.force_next_order=-1;
	control.ticks_counter=control.speed;
        control.position.forbid_jump=false;
	
}


bool Player::reached_end_of_song() {

	return control.reached_end;

}
void Player::set_force_external_vibratos(bool p_force) {

	control.external_vibrato=p_force;
}
void Player::set_force_no_nna(bool p_force) {

	control.force_no_nna=p_force;
}
