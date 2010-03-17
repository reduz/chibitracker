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
//
//
// C++ Interface: sample_editor_effects
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SAMPLE_EDITOR_EFFECTS_H_
#define SAMPLE_EDITOR_EFFECTS_H_

#include "mixer/sample_manager.h"
#include "tracker/selection.h"

class SampleEditorEffects {

	Selection *selection;
	Sample_ID sample;
	Sample_ID clipboard;
	
	int get_begin();
	int get_end();
	int get_size();
	SampleManager *sm;
public:
	
	
	enum CommandList {
		
		CMD_COPY,
		CMD_CUT,
		CMD_PASTE,
		CMD_TRIM,
		CMD_PRELOOP_CUT,
		CMD_POSTLOOP_CUT,
		CMD_SEL_TO_LOOP,
		CMD_LOOP_TO_BOUNDARY,
		CMD_LOOP_UNROLL,
		CMD_TOGGLE_SIGN,
		CMD_TOGGLE_DEPTH,
		CMD_TOGGLE_STEREO,
		CMD_SWAP_STEREO_CHANS,
		CMD_AMPLIFY,
		CMD_FADE_IN,
		CMD_FADE_OUT,
		CMD_CENTER,
		CMD_REVERSE,
		CMD_FIX_LOOPING,
		CMD_RESAMPLE_FREQ,
		CMD_RESAMPLE,
		CMD_LEFT_CHAN_DELAY,
		CMD_ADD_SILENCE_AT_END,
		CMD_MATCH_LOOP_END_BY_NOTE,
		CMD_MAX,
		
	};
			
		
	void command(int p_command,int p_parameter=-1);
	bool command_has_parameter(int p_command);
	void command_set_parameter_range(int p_command,int& p_min, int &p_max, int &p_default);
	
	void set_selection(Selection *p_selection);
	void set_sample(Sample_ID p_sample);
	

	SampleEditorEffects();
};



#endif
