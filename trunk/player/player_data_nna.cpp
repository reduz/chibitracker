/***************************************************************************
                          player_data_nna.cpp  -  description
                             -------------------
    begin                : Fri Apr 6 2001
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
void Player::process_NNAs() {

	int i;

	if (!song->has_instruments()) return;

	for (i=0;i<Pattern::WIDTH;i++) {

		Channel_Control *aux_chn_ctrl = &control.channel[i];

		if (aux_chn_ctrl->kick==KICK_NOTE) {

			bool k=false;

			if (aux_chn_ctrl->slave_voice!=NULL) {

				Voice_Control *aux_voc_ctrl;

				aux_voc_ctrl=aux_chn_ctrl->slave_voice;
				
				if (aux_chn_ctrl->instrument_index==aux_chn_ctrl->slave_voice->instrument_index) { //maybe carry
					
					aux_chn_ctrl->carry.pan=aux_chn_ctrl->slave_voice->panning_envelope_ctrl;
					aux_chn_ctrl->carry.vol=aux_chn_ctrl->slave_voice->volume_envelope_ctrl;
					aux_chn_ctrl->carry.pitch=aux_chn_ctrl->slave_voice->pitch_envelope_ctrl;
					aux_chn_ctrl->carry.maybe=true;
				} else 
					aux_chn_ctrl->carry.maybe=false;
				
				if (aux_voc_ctrl->NNA_type != Instrument::NNA_NOTE_CUT) {
					/* Make sure the old MP_VOICE channel knows it has no
					   master now ! */
					

					
					aux_chn_ctrl->slave_voice=NULL;
					/* assume the channel is taken by NNA */
					aux_voc_ctrl->has_master_channel=false;

					switch (aux_voc_ctrl->NNA_type) {
						case Instrument::NNA_NOTE_CONTINUE: {

						} break;
						case Instrument::NNA_NOTE_OFF: {


							aux_voc_ctrl->note_end_flags|=END_NOTE_OFF;

							if (!aux_voc_ctrl->volume_envelope_ctrl.active || aux_voc_ctrl->instrument_ptr->get_volume_envelope()->is_loop_enabled()) {
								aux_voc_ctrl->note_end_flags|=END_NOTE_FADE;
							}
						} break;
						case Instrument::NNA_NOTE_FADE: {

							aux_voc_ctrl->note_end_flags|=END_NOTE_FADE;
						} break;
					}
				} 
			}

			if (aux_chn_ctrl->duplicate_check_type!=Instrument::DCT_DISABLED) {
				int i;

				for (i=0;i<control.max_voices;i++) {
					if (!mixer->is_voice_active(i)||
					   (voice[i].master_channel!=aux_chn_ctrl) ||
					   (aux_chn_ctrl->instrument_index!=voice[i].instrument_index))
						continue;

					Voice_Control *aux_voc_ctrl;

					aux_voc_ctrl=&voice[i];

					k=false;
					switch (aux_chn_ctrl->duplicate_check_type) {
						case Instrument::DCT_NOTE:
							if (aux_chn_ctrl->note==aux_voc_ctrl->note)
								k=true;
							break;
						case Instrument::DCT_SAMPLE:
							if (aux_chn_ctrl->sample_ptr==aux_voc_ctrl->sample_ptr)
								k=true;
							break;
						case Instrument::DCT_INSTRUMENT:
							k=true;
							break;
					}
					if (k) {
						switch (aux_chn_ctrl->duplicate_check_action) {
							case Instrument::DCA_NOTE_CUT: {
								aux_voc_ctrl->fadeout_volume=0;
							} break;
							case Instrument::DCA_NOTE_OFF: {

								aux_voc_ctrl->note_end_flags|=END_NOTE_OFF;

								if (!aux_voc_ctrl->volume_envelope_ctrl.active || aux_chn_ctrl->instrument_ptr->get_volume_envelope()->is_loop_enabled()) {

									aux_voc_ctrl->note_end_flags|=END_NOTE_FADE;
								}

							} break;
							case Instrument::DCA_NOTE_FADE: {
								aux_voc_ctrl->note_end_flags|=END_NOTE_FADE;
							} break;
						}
					}	
				}

			}	
		} /* if (aux_chn_ctrl->kick==KICK_NOTE) */
	}
}
