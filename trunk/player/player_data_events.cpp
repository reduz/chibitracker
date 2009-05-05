/***************************************************************************
                          player_data_events.cpp  -  description
                             -------------------
    begin                : Fri Feb 2 2001
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

#include "mixer/sample_manager.h"
#include <stdlib.h>
#include <stdio.h>
/*
	  setup_voices():

This will go throught all the REAL channels, if it finds a channel
that needs to be restarted or assigned a new VIRTUAL channel, then it
will just find one and do it.

*/


#define C5FREQ 261.6255653006
static const Uint32 C5FREQ_MIXER = ((Uint32)(C5FREQ*(float)(1<<Mixer::FREQUENCY_BITS)));


void Player::setup_voices() {

	int i,voice_index;

	
	

	for (i=0;i<Pattern::WIDTH;i++) {

		voice_index=-1;

		if (control.channel[i].note_delay) continue;
		

		// check if we need a new empty voice
		if (control.channel[i].kick==KICK_NOTE) {

			/* if no channel was cut above, find an empty or quiet channel
			   here */
			if ( song->has_instruments() && !control.force_no_nna) {

				if (control.channel[i].slave_voice==NULL) { // no slave??

					int newchn;
					if ((newchn=find_empty_voice())!=-1) {

						control.channel[i].slave_voice_index=newchn;
						control.channel[i].slave_voice=&voice[newchn];
					}
				}

			} else  {
				if (i<control.max_voices) {

					control.channel[i].slave_voice_index=i;
					control.channel[i].slave_voice=&voice[i];
                                } else {
					//This is a _DIRTY_ hack, but i cant think a better way.
					control.channel[i].slave_voice_index=control.max_voices-1;
					control.channel[i].slave_voice=&voice[control.max_voices-1];
				}
				
			}


			/* assign parts of MP_VOICE only done for a KICK_NOTE */
			if ( ( control.channel[i].slave_voice!=NULL ) ) {

				voice_index=control.channel[i].slave_voice_index;
				Voice_Control &v=voice[voice_index];
				
				if (v.has_master_channel && (v.master_channel!=NULL) ) {
                                        // If this voice already has a master channel, make sure to remove the reference to it.
					v.master_channel->slave_voice=NULL;

				}
				//notify the voice that the current channel is the master
				v.master_channel=&control.channel[i];
				//set the voice as slave of the current channel
				control.channel[i].slave_voice=&v;
				//master channel index of the voice
				v.master_channel_index=i;
				v.has_master_channel=true;
			}

		} else {
                // nope..
                        // so if we DO have a slave voice then use it.
			if ( control.channel[i].slave_voice!=NULL ) {
			
				voice_index=control.channel[i].slave_voice_index;
			}
		}
                //assuming this channel has a slave voice..
		if (voice_index>=0) {

			// IMPROVE: Code a method for this:
			voice[voice_index].update_info_from_master_channel();
		}

		control.channel[i].kick=KICK_NOTHING;
	}
}
void Player::Voice_Control::reset() {

	memzero(this,sizeof(*this));

	instrument_ptr=NULL;
	sample_ptr=NULL;
	has_master_channel=false;
	instrument_index=-1;
	reverb_send=0;
	chorus_send=0;
	filter.it_cutoff=255;
	filter.it_reso=0;
	display_volume=0;

	
}

void Player::Channel_Control::reset() {

	memzero(this,sizeof(*this));

	slave_voice=NULL;
	slave_voice_index=255;

	mute=false;
	old_note=255;
	real_note=255;
	instrument_index=255;
	filter.it_cutoff=255;
	filter.it_reso=0;
	reverb_send=0;
	chorus_send=0;
	reserved=false;	
	carry.maybe=false;
}

void Player::Voice_Control::update_info_from_master_channel() {

	instrument_ptr=master_channel->instrument_ptr;
	sample_ptr=master_channel->sample_ptr;

	instrument_index=master_channel->instrument_index;
	sample_index=master_channel->sample_index;

	note=master_channel->note;
	output_volume=master_channel->output_volume;

	channel_volume=master_channel->channel_volume;

	panning=master_channel->panning;

	kick=master_channel->kick;
	note_end_flags=master_channel->note_end_flags;
	period=master_channel->period;

	volume_envelope_ctrl.active=master_channel->volume_envelope_on;
	panning_envelope_ctrl.active=master_channel->panning_envelope_on;
	pitch_envelope_ctrl.active=master_channel->pitch_envelope_on;


	NNA_type=master_channel->NNA_type;
	reverb_send=master_channel->reverb_send;
	chorus_send=master_channel->chorus_send;

//	last_note_type=master_channel->last_note_type;

	sample_start_index=master_channel->sample_start_index;
	filter=master_channel->filter;

}


void Player::update_mixer() {

	int tmp_mixer_period;
	Sint32 tmp_vibrato_value,tmp_vibrato_depth,tmp_volenv_value;
	Uint64 tmpvol; // 64bits should be the only way to avoid getting notes raped out
	int i;


	control.voices_used=0;

	for (i=0;i<control.max_voices;i++) {

		
	         int filter_env=-1;
		 Voice_Control &v=voice[i];
		
		 if ( !((v.kick==KICK_NOTE)||(v.kick==KICK_NOTEOFF)) && !is_voice_active(i))
			 continue;
		 
		//if voice doesnt have a sample set or size is 0.. forget it
		if ( v.sample_ptr==NULL) continue;
			

		//TODO set limits somewhere else

		if (v.period<40) {
		
			v.period=40;

		} else if (v.period>50000) {

			v.period=50000;
		}


		if ((v.kick==KICK_NOTE)||(v.kick==KICK_NOTEOFF)) {

			int real_start_index;

			if (v.sample_start_index==-1) {

				real_start_index=0;

			} else {

				real_start_index=v.sample_start_index;
			}

			mixer->setup_voice(i,v.sample_ptr->get_sample_data(),real_start_index);
			v.fadeout_volume=1024; //IT Docs it is 1024 internally
			v.auto_vibrato_sweep_pos=0;
	
		

		}


		/* Start Envelopes */	
               	if ( song->has_instruments() && ((v.kick==KICK_NOTE)||(v.kick==KICK_ENVELOPE))) {
	
//			Voice_Control *carry=0;
			
			
			if (v.has_master_channel && v.master_channel->carry.maybe) {
				
				v.start_envelope(v.instrument_ptr->get_volume_envelope(),&v.volume_envelope_ctrl,&v.master_channel->carry.vol);
				v.start_envelope(v.instrument_ptr->get_pan_envelope(),&v.panning_envelope_ctrl,&v.master_channel->carry.pan);
				v.start_envelope(v.instrument_ptr->get_pitch_filter_envelope(),&v.pitch_envelope_ctrl,&v.master_channel->carry.pitch);
				
			} else {
				
				v.start_envelope(v.instrument_ptr->get_volume_envelope(),&v.volume_envelope_ctrl,NULL);
				v.start_envelope(v.instrument_ptr->get_pan_envelope(),&v.panning_envelope_ctrl,NULL);
				v.start_envelope(v.instrument_ptr->get_pitch_filter_envelope(),&v.pitch_envelope_ctrl,NULL);
				
			}


		}

		v.kick=KICK_NOTHING;

               	if (song->has_instruments()) {

			if (!v.process_envelope(v.instrument_ptr->get_volume_envelope(),&v.volume_envelope_ctrl))
				v.volume_envelope_ctrl.value=64;

			if (!v.process_envelope(v.instrument_ptr->get_pan_envelope(),&v.panning_envelope_ctrl))
				v.panning_envelope_ctrl.value=0;
			
			if (!v.process_envelope(v.instrument_ptr->get_pitch_filter_envelope(),&v.pitch_envelope_ctrl))
				v.pitch_envelope_ctrl.value=0;
				

			if (v.volume_envelope_ctrl.terminated) {

				if (v.volume_envelope_ctrl.kill) {
	
					v.fadeout_volume=0;
				} else {

					v.note_end_flags|=END_NOTE_FADE;
                                }
			}
				
		}

		if (song->has_instruments()) {

			tmp_volenv_value=v.volume_envelope_ctrl.value;
		} else {

			tmp_volenv_value=64;

		}

		
		tmpvol=(Uint64)v.fadeout_volume;    /* max 1024 - 10 bits */
		tmpvol*=(Uint64)v.channel_volume;    /* * max 64 - 6 bits */
		tmpvol*=(Uint64)v.output_volume;     /* * max 256 - 8 bits */
		tmpvol*=(Uint64)tmp_volenv_value; /* max 64 - 6 bits*/
		v.display_volume=tmpvol>>22; //volume used for display purposes , 0 -- 256
		
		tmpvol*=(Uint64)song->get_mixing_volume(); /* max 128 - 7 bits */
		tmpvol*=(Uint64)control.global_volume;   /* max 128 - 7 bits*/
				/* total 10+6+8+6+7+7=44 bits */
		Uint64 preshift=tmpvol;
		tmpvol>>=35; /* Move back to 8 bits range , 44-19=35*/
		
		/*
		if (!tmpvol && preshift>>35)
			tmpvol=1; // even if the volume should be inaudible, some people WANTS to hear this.
		*/
		if (tmpvol>VOL_MAX)
			tmpvol=VOL_MAX;
		
		//printf("volume check - fade %i, channel %i, output %i, env %i, mix %i, global %i -- final %i\n",v.fadeout_volume, v.channel_volume,v.output_volume,tmp_volenv_value, song->get_mixing_volume(),control.global_volume,tmpvol);
		
		v.total_volume=tmpvol;

		if ((v.master_channel!=NULL) && song->is_channel_mute( v.master_channel_index ) && !v.master_channel->reserved) {

			mixer->set_voice_volume(i,0);
		} else {
			mixer->set_voice_volume(i,tmpvol);
			if (v.fadeout_volume>0) control.voices_used++;
		}

        	
		if (!song->is_stereo()) {

			mixer->set_voice_panning(i,PAN_CENTER);

		} else if (v.panning==PAN_SURROUND) {

			mixer->set_voice_panning(i,PAN_SURROUND);
		} else if (song->has_instruments()) {

			int newpan,real_modifier;

			
			real_modifier=(v.panning_envelope_ctrl.value*(PAN_CENTER-abs(v.panning-PAN_CENTER)))/32;

			newpan=v.panning+real_modifier;

			newpan=(newpan<PAN_LEFT)?PAN_LEFT:(newpan>PAN_RIGHT)?PAN_RIGHT:newpan;
			//printf("panenv val: %i, finalpan val %i\n",v.panning_envelope_ctrl.value,newpan);
			
			mixer->set_voice_panning(i,newpan);
		} else {
			mixer->set_voice_panning(i,v.panning);
		}



		/* VIBRATO */

		if ( (v.period>0) && (v.sample_ptr->get_vibrato_depth()>0) ) {

			switch (v.sample_ptr->get_vibrato_type()) {
				case Sample::VIBRATO_SINE:
					tmp_vibrato_value=auto_vibrato_table[v.auto_vibrato_pos&127];
					if (v.auto_vibrato_pos & 0x80) tmp_vibrato_value=-tmp_vibrato_value;
					break;
				case Sample::VIBRATO_SQUARE:
					tmp_vibrato_value=64;
					if (v.auto_vibrato_pos & 0x80) tmp_vibrato_value=-tmp_vibrato_value;
					break;
				case Sample::VIBRATO_SAW:
					tmp_vibrato_value=63-(((v.auto_vibrato_pos+128)&255)>>1);
					break;
				default:
					tmp_vibrato_value=(((v.auto_vibrato_pos+128)&255)>>1)-64;
					break;
			}
		} else {

			tmp_vibrato_value=0;
		}

		if ((v.auto_vibrato_sweep_pos>>8)<v.sample_ptr->get_vibrato_depth()) {

			v.auto_vibrato_sweep_pos+=v.sample_ptr->get_vibrato_speed(); //FIXME - speed? i think so
			tmp_vibrato_depth=v.auto_vibrato_sweep_pos;

		} else {

			tmp_vibrato_depth=v.sample_ptr->get_vibrato_depth()<<8;
		}
			
		tmp_vibrato_value=(tmp_vibrato_value*tmp_vibrato_depth)>>16;
		if (song->has_linear_slides())
			tmp_vibrato_value>>=1;
		v.period-=tmp_vibrato_value;


		/* update vibrato position */
		v.auto_vibrato_pos=(v.auto_vibrato_pos+v.sample_ptr->get_vibrato_rate())&0xff;
	
		
		/* process pitch envelope */
		tmp_mixer_period=v.period;

		if (v.pitch_envelope_ctrl.active) {

			long aux_pitch_diff;
			int pe_value=v.pitch_envelope_ctrl.value;
			
			if (!v.instrument_ptr->is_pitch_use_as_filter()) {
			
				
				if (((Uint16)v.note<<1)+pe_value<=0) 
					pe_value=-(v.note<<1);

				int smp_c5=SampleManager::get_singleton()->get_c5_freq( v.sample_ptr->get_sample_data());
				
				int base=get_period(((Uint16)v.note<<1),smp_c5);
				int env=get_period(((Uint16)v.note<<1)+pe_value,smp_c5);
				/*
				int env_next=(pe_value<0)?get_period(((Uint16)(v.note-1)<<1)+pe_value,smp_c5):get_period(((Uint16)(v.note+1)<<1)+pe_value,smp_c5);
				
				env=env+(abs(v.pitch_envelope_ctrl.value)&((1<<Envelope::FX_HEIGHT_BITS)-1))*(env_next-env)/(1<<Envelope::FX_HEIGHT_BITS);
				
				printf("env %i\n",env);
				*/				
				aux_pitch_diff=env-base;


				if ( ((int)tmp_mixer_period-aux_pitch_diff)<0 ) aux_pitch_diff=0;
				
				tmp_mixer_period+=aux_pitch_diff;

			} else {

				filter_env=pe_value+32; //max 64
//			        printf("pitch envelope at %i",filter_env);
			
			}
		}

		if (v.fadeout_volume==0 || (v.note_end_flags & END_NOTE_KILL)) { /* check for a dead note (fadevol=0) */

			mixer->stop_voice(i);

		} else {

			
			Uint32 freq=get_frequency(tmp_mixer_period);
			Uint32 tracker_c5=get_frequency(get_period(60<<1,SampleManager::get_singleton()->get_c5_freq( v.sample_ptr->get_sample_data())));
			
			freq=(Uint32)((Uint64)freq*(Uint64)C5FREQ_MIXER/(Uint64)tracker_c5); //numbers may become very high
			mixer->set_voice_frequency(i,freq);

			/* if keyfade, start substracting fadeoutspeed from fadevol: */
			if ((song->has_instruments())&&(v.note_end_flags & END_NOTE_FADE)) {

				if (v.fadeout_volume>=(v.instrument_ptr->get_volume_fadeout())) {

					v.fadeout_volume-=(v.instrument_ptr->get_volume_fadeout());
				} else {

					v.fadeout_volume=0;
				}
			}
			
			/*FILTARSSSSSSSS*/


					
			v.filter.envelope_cutoff=filter_env;
			v.filter.process();
			
	                if ((v.filter.final_cutoff<0xFF) && (control.filters)) {
	
				//int final_cutoff;
				//Uint8 final_reso;
				
				//v.filter.set_filter_parameters( &final_cutoff, &final_reso );
				
				mixer->set_voice_filter(i,true,v.filter.final_cutoff,v.filter.it_reso);
	                } else {
	
				
				mixer->set_voice_filter(i,false,0,0);
	                }
	
	                /* RAIVERV */
	
       			mixer->set_voice_reverb_send(i,v.reverb_send);

          		/* CHAURUZ */

       			mixer->set_voice_chorus_send(i,v.chorus_send);

                }
	}
	
	
	switch(song->get_reverb_mode()) {
		
		case Song::REVERB_MODE_ROOM: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_ROOM );
		} break;
		case Song::REVERB_MODE_STUDIO_SMALL: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_STUDIO_SMALL );
			
		} break;
		case Song::REVERB_MODE_STUDIO_MEDIUM: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_STUDIO_MEDIUM );
			
		} break;
		case Song::REVERB_MODE_STUDIO_LARGE: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_STUDIO_LARGE );
			
		} break;
		case Song::REVERB_MODE_HALL: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_HALL );
			
		} break;
		case Song::REVERB_MODE_SPACE_ECHO: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_SPACE_ECHO );
			
		} break;
		case Song::REVERB_MODE_ECHO: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_ECHO );
			
		} break;
		case Song::REVERB_MODE_DELAY: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_DELAY );
			
		} break;
		case Song::REVERB_MODE_HALF_ECHO: {
			
			mixer->set_reverb_mode( Mixer::REVERB_MODE_HALF_ECHO );
			
		} break;
	
	}
	
	mixer->set_chorus_params(song->get_chorus_delay_ms(),song->get_chorus_separation_ms(),song->get_chorus_depth_ms10(),song->get_chorus_speed_hz10() );
}








void Player::handle_tick() {

	int i;


	if ( mixer==NULL ) return;
	if ( song==NULL ) return;


	/* update time counter (sngtime is in milliseconds (in fact 2^-10)) */

	if (control.ticks_counter>=control.speed) { // time to process... ***THE ROW***!

		/* process pattern-delay. pf->patdly2 is the counter and pf->patdly is
		   the command memory. */

//		if (control.pattern_delay_1) {

//			control.pattern_delay_2=control.pattern_delay_1;
//			control.pattern_delay_1=0;
//		}
//		if (control.pattern_delay_2) {
//			 patterndelay active
//			if (--control.pattern_delay_2)
//				 so turn back pf->patpos by 1
//				if (pf->patpos) pf->patpos--;
//		}

		if (control.play_mode!=PLAY_NOTHING) {

			control.ticks_counter=0;
	

			if (control.position.force_next_order>=0) {

				control.position.current_order=control.position.force_next_order;				
			}

			control.position.force_next_order=-1;			

			control.previous_position=control.position; // for those special cases...
                        control.position.forbid_jump=false;

			for (i=0;i<Pattern::WIDTH;i++) {

				process_note(i,song->get_pattern(control.position.current_pattern)->get_note(i,control.position.current_row));
			}

			control.position.current_row++;
		
			if ( control.position.current_row>=song->get_pattern(control.position.current_pattern)->get_length() ) {

				if (control.play_mode==PLAY_SONG) {

					int next_order;

					next_order=get_song_next_order_idx(song,control.position.current_order);

					if (next_order!=-1) {
						// Do we have a "next order?"
						control.position.current_pattern=song->get_order(next_order);
						if (next_order<=control.position.current_order) 
							control.reached_end=true;
						control.position.current_order=next_order;
					
					} else {
						// no, probably the user deleted the orderlist.
						control.play_mode=PLAY_NOTHING;
						reset();
						control.reached_end=true;
					}
				}
				control.position.current_row=0;
			}

		}


	}



	pre_process_effects();
	process_NNAs();
	setup_voices();

	/* now set up the actual hardware channel playback information */
	update_mixer();

	control.ticks_counter++;
}
