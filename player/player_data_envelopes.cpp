/***************************************************************************
                          player_data_envelopes.cpp  -  description
                             -------------------
    begin                : Fri Mar 30 2001
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


void Player::Voice_Control::start_envelope(Envelope *p_envelope,Envelope_Control *p_envelope_ctrl,Envelope_Control *p_from_env) {


	if (p_from_env && p_envelope->is_carry_enabled() && !p_from_env->terminated) {

		
		*p_envelope_ctrl=*p_from_env;
	} else {
		p_envelope_ctrl->pos_index=0;
		p_envelope_ctrl->status=1;
		p_envelope_ctrl->sustain_looping=p_envelope->is_sustain_loop_enabled();
		p_envelope_ctrl->looping=p_envelope->is_loop_enabled();
		p_envelope_ctrl->terminated=false;
		p_envelope_ctrl->kill=false;
		p_envelope_ctrl->value=p_envelope->get_height_at_pos(p_envelope_ctrl->pos_index);
	}
}

bool Player::Voice_Control::process_envelope(Envelope *p_envelope,Envelope_Control *p_envelope_ctrl) {

	if (!p_envelope_ctrl->active) 
		return false;

	if (note_end_flags&END_NOTE_OFF) p_envelope_ctrl->sustain_looping=false;

	p_envelope_ctrl->value=p_envelope->get_height_at_pos(p_envelope_ctrl->pos_index);
	if (p_envelope_ctrl->value==Envelope::NO_POINT)
		return false;
	

	p_envelope_ctrl->pos_index++;

	if (p_envelope_ctrl->sustain_looping) {

		if (p_envelope_ctrl->pos_index>p_envelope->get_node(p_envelope->get_sustain_loop_end()).tick_offset) {

			p_envelope_ctrl->pos_index=p_envelope->get_node(p_envelope->get_sustain_loop_begin()).tick_offset;
		}

	} else if (p_envelope_ctrl->looping) {

		if (p_envelope_ctrl->pos_index>p_envelope->get_node(p_envelope->get_loop_end()).tick_offset) {

			p_envelope_ctrl->pos_index=p_envelope->get_node(p_envelope->get_loop_begin()).tick_offset;
		}

	}

	if (p_envelope_ctrl->pos_index>p_envelope->get_node(p_envelope->get_node_count()-1).tick_offset) {

		p_envelope_ctrl->terminated=true;
		p_envelope_ctrl->pos_index=p_envelope->get_node(p_envelope->get_node_count()-1).tick_offset;
		if (p_envelope->get_node(p_envelope->get_node_count()-1).value==0) p_envelope_ctrl->kill=true;
	}

	return true;
}
