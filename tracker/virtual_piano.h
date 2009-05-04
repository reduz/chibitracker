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
                          player_rt_keyboard.h  -  description
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

#ifndef VIRTUAL_PIANO_H
#define VIRTUAL_PIANO_H

#include "player/player_data.h"
#include "mixer/mixer.h"


/**realtime keyboard for testing
stuff
  *@author Juan Linietsky
  */

/******************************
 virtual_piano.h
 ------------------------

this is cool because, besides being cool,
allows you to abstract that keyboard-playing
note-testing things of IT.
********************************/

class VirtualPiano {

	bool key_pressed[Note::NOTES];
	int channel_per_note[Pattern::WIDTH];

	Player *player;
	Mixer *mixer;

        int find_note_in_channel(int p_note);
	int find_empty_channel();

	int current_instrument;
	Sample_ID sample;
	bool multichannel_set;
        int latest_key_pressed;
	int sample_voice;
	bool sample_playing;
public:

	void instrument_set(int p_instrument_index);
	void instrument_press_key(int p_note, int p_volume);
	void instrument_stop_key(int p_note);
	void instrument_stop_all();

	void sample_set(Sample_ID p_sample);
	void sample_press_key(int p_note);
	void sample_stop(int p_note);
	void sample_stop_all();
	


	VirtualPiano(Player *p_player,Mixer *p_mixer);
	~VirtualPiano();
};

#endif
