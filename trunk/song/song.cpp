
#include "song.h"
#include "error_macros.h"

void Song::set_name(const char *p_name) {

	if (p_name==NULL) {
		variables.name[0]=0;
		return;
	}
		

	bool done=false;
        for (int i=0;i<MAX_SONG_NAME;i++) {

	        
	        variables.name[i]=done?0:p_name[i];
		if (!done && p_name[i]==0)
			done=true;
	}

	variables.name[MAX_SONG_NAME-1]=0; /* just in case */
}

const char * Song::get_name() {

	return variables.name;

}

void Song::set_message(const char *p_message) {

	if (p_message==NULL) {
		variables.message[0]=0;
		return;
	}

	bool done=false;
	for (int i=0;i<MAX_MESSAGE_LEN;i++) {

	        
		variables.message[i]=done?0:p_message[i];
		if (!done && p_message[i]==0)
			done=true;
	}

	variables.message[MAX_MESSAGE_LEN-1]=0; /* just in case */
}

const char * Song::get_message() {

	return variables.message;

}

void Song::set_row_highlight_minor(int p_hl_minor) {

	variables.row_highlight_minor=p_hl_minor;
}
int Song::get_row_highlight_minor() {

	return variables.row_highlight_minor;
}

void Song::set_row_highlight_major(int p_hl_major) {

	variables.row_highlight_major=p_hl_major;


} /* 0 .. 256 */
int Song::get_row_highlight_major() {

	return variables.row_highlight_major;


} /* 0 .. 256 */

void Song::set_mixing_volume(int p_mix_volume) {


	variables.mixing_volume=p_mix_volume;
} /* 0 .. 128 */
int Song::get_mixing_volume() {

	return variables.mixing_volume;

} /* 0 .. 128 */

void Song::set_global_volume(int p_global_volume) {


	initial_variables.global_volume=p_global_volume;

} /* 0 .. 128 */
int Song::get_global_volume() {

	return initial_variables.global_volume;

} /* 0 .. 128 */

void Song::set_stereo_separation(int p_separation) {

	variables.stereo_separation=p_separation;

} /* 0 .. 128 */
int Song::get_stereo_separation() {

	return variables.stereo_separation;
} /* 0 .. 128 */

void Song::set_stereo(bool p_stereo) {

	variables.use_stereo=p_stereo;

}
bool Song::is_stereo() {

	return variables.use_stereo;
}

void Song::set_instruments(bool p_instruments) {

	variables.use_instruments=p_instruments;


}
bool Song::has_instruments() {


	return variables.use_instruments;

}

void Song::set_linear_slides(bool p_linear_slides) {

	variables.use_linear_slides=p_linear_slides;


}
bool Song::has_linear_slides() {

	return variables.use_linear_slides;


}

void Song::set_old_effects(bool p_old_effects) {

	variables.old_effects=p_old_effects;


}
bool Song::has_old_effects() {

	return variables.old_effects;
}

void Song::set_compatible_gxx(bool p_compatible_gxx) {


	variables.compatible_gxx=p_compatible_gxx;
}
bool Song::has_compatible_gxx() {

	return variables.compatible_gxx;

}

void Song::set_speed(int p_speed) {

	ERR_FAIL_COND(p_speed<MIN_SPEED);
	ERR_FAIL_COND(p_speed>MAX_SPEED);
	
	initial_variables.speed=p_speed;

} /* 1 .. 255 */
int Song::get_speed() {

	return initial_variables.speed;

} /* 1 .. 255 */

void Song::set_tempo(int p_tempo) {

	ERR_FAIL_COND( p_tempo<MIN_TEMPO );
	ERR_FAIL_COND( p_tempo>MAX_TEMPO );
	
	initial_variables.tempo=p_tempo;

} /* MIN_TEMPO .. MAX_TEMPO */
int Song::get_tempo() {

	return initial_variables.tempo;


} /* MIN_TEMPO .. MAX_TEMPO */

void Song::set_channel_pan(int p_channel,int p_pan) {

	ERR_FAIL_INDEX(p_channel,Pattern::WIDTH);
	ERR_FAIL_INDEX(p_pan,CHANNEL_MAX_PAN+1);
	
	initial_variables.channel[p_channel].pan=p_pan;

} /* 0 .. CHANNEL_MAX_PAN */
int Song::get_channel_pan(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,Pattern::WIDTH,-1);
	
	return initial_variables.channel[p_channel].pan;
}

void Song::set_channel_volume(int p_channel,int p_volume) {

	ERR_FAIL_INDEX(p_channel,Pattern::WIDTH);
	ERR_FAIL_INDEX(p_volume,CHANNEL_MAX_VOLUME+1);
	
	
	initial_variables.channel[p_channel].volume=p_volume;


} /* 0 .. CHANNEL_MAX_VOLUME */


int Song::get_channel_volume(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,Pattern::WIDTH,-1);
	
	return initial_variables.channel[p_channel].volume;

}

void Song::set_channel_chorus(int p_channel,int p_chorus) {

	ERR_FAIL_INDEX(p_channel,Pattern::WIDTH);
	ERR_FAIL_INDEX(p_chorus,CHANNEL_MAX_CHORUS+1);
	
	
	initial_variables.channel[p_channel].chorus=p_chorus;


} /* 0 .. CHANNEL_MAX_CHORUS */


int Song::get_channel_chorus(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,Pattern::WIDTH,-1);
	
	return initial_variables.channel[p_channel].chorus;

}

void Song::set_channel_reverb(int p_channel,int p_reverb) {

	ERR_FAIL_INDEX(p_channel,Pattern::WIDTH);
	ERR_FAIL_INDEX(p_reverb,CHANNEL_MAX_REVERB+1);
	
	
	initial_variables.channel[p_channel].reverb=p_reverb;


} /* 0 .. CHANNEL_MAX_CHORUS */


int Song::get_channel_reverb(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,Pattern::WIDTH,-1);
	
	return initial_variables.channel[p_channel].reverb;

}

void Song::set_channel_surround(int p_channel,bool p_surround) {

	ERR_FAIL_INDEX(p_channel,Pattern::WIDTH);
	initial_variables.channel[p_channel].surround=p_surround;

}
bool Song::is_channel_surround(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,Pattern::WIDTH,false);
	
	return initial_variables.channel[p_channel].surround;


}

void Song::set_channel_mute(int p_channel,bool p_mute) {

	ERR_FAIL_INDEX(p_channel,Pattern::WIDTH);
	
	initial_variables.channel[p_channel].mute=p_mute;

}
bool Song::is_channel_mute(int p_channel) {

	ERR_FAIL_INDEX_V(p_channel,Pattern::WIDTH,false);
	
	return initial_variables.channel[p_channel].mute;

}

/* arrays of stuff */

Pattern* Song::get_pattern(int p_pattern) {

	ERR_FAIL_INDEX_V(p_pattern,MAX_PATTERNS, NULL);
	
	return &pattern[p_pattern];

}
Sample* Song::get_sample(int p_sample) {

	ERR_FAIL_INDEX_V(p_sample,MAX_SAMPLES,NULL);

	return &sample[p_sample];
	

}
Instrument* Song::get_instrument(int p_instrument) {


	ERR_FAIL_INDEX_V(p_instrument,MAX_INSTRUMENTS,NULL);

	return &instrument[p_instrument];
	
}

int Song::get_order(int p_order) {

	ERR_FAIL_INDEX_V(p_order,MAX_ORDERS,ORDER_NONE);
	

	return order[p_order];

}
void Song::set_order(int p_order,int p_pattern) {

	ERR_FAIL_INDEX(p_order,MAX_ORDERS);

	order[p_order]=p_pattern;

}


void Song::clear_instrument_with_samples(int p_instrument) {
	
	Instrument *ins = get_instrument( p_instrument );
	if (!ins)
		return;
	
	for (int i=0;i<Note::NOTES;i++) {
		
		Sample *s=get_sample( ins->get_sample_number( i ) );
		
		if (!s)
			continue;
		
		if (s->get_sample_data().is_null())
			continue;
		
		s->reset();
	}
	ins->reset();
}

void Song::make_instrument_from_sample(int p_sample) {
	
	if (!has_instruments())
		return;
	ERR_FAIL_COND(!get_sample( p_sample ));
	
	for (int i=0;i<MAX_INSTRUMENTS;i++) {
		
		
		Instrument *ins=get_instrument(i);
		
		bool empty_slot=true;
		for (int n=0;n<Note::NOTES;n++) {
			
			if (ins->get_sample_number(n)<MAX_SAMPLES) {
				
				empty_slot=false;
				break;
			}
		}
		
		if (!empty_slot)
			continue;
		
		for (int n=0;n<Note::NOTES;n++) {
			
			ins->set_sample_number(n,p_sample);
			ins->set_note_number(n,n);
		}
		
		ins->set_name( get_sample( p_sample )->get_name() );
		break;
	}
	
}

void Song::make_instruments_from_samples() {
	
	for (int i=0;i<MAX_SAMPLES;i++) {
		
		Instrument *ins=get_instrument( i );
		
		if (!ins)
			continue;
		
		ins->reset();
		
		Sample *s=get_sample( i );
		
		if (!s)
			continue;
		
		ins->set_name( s->get_name() );
		
		if (s->get_sample_data().is_null())
			continue;
		
		
		
		
		for(int j=0;j<Note::NOTES;j++) 
			ins->set_sample_number( j, i );
		
		
		
	}
}

void Song::reset(bool p_clear_patterns,bool p_clear_samples,bool p_clear_instruments,bool p_clear_variables) {
	
	if (p_clear_variables) {
		variables.name[0]=0;
		variables.message[0]=0;
		variables.row_highlight_major=16;
		variables.row_highlight_minor=4;
		variables.mixing_volume=48;
		variables.old_effects=false;
		if (p_clear_instruments) //should not be cleared, if not clearing instruments!!
			variables.use_instruments=false;
		variables.stereo_separation=128;
		variables.use_linear_slides=true;
		variables.use_stereo=true;
		
		initial_variables.global_volume=128;
		initial_variables.speed=6;
		initial_variables.tempo=125;
		
		for (int i=0;i<Pattern::WIDTH;i++) {
			
			initial_variables.channel[i].pan=32;
			initial_variables.channel[i].volume=CHANNEL_MAX_VOLUME;
			initial_variables.channel[i].mute=false;
			initial_variables.channel[i].surround=false;
			initial_variables.channel[i].chorus=0;
			initial_variables.channel[i].reverb=0;
			
		}
		
		effects.chorus.delay_ms=6;
		effects.chorus.separation_ms=3;
		effects.chorus.depth_ms10=6,
		effects.chorus.speed_hz10=5;
		effects.reverb_mode=REVERB_MODE_ROOM;
	}
	
	if (p_clear_samples) {
		for (int i=0;i<MAX_SAMPLES;i++)
			get_sample(i)->reset();
	}
		
	if (p_clear_instruments) {
		for (int i=0;i<MAX_INSTRUMENTS;i++)
			get_instrument(i)->reset();
	}
	
	if (p_clear_patterns) {
		for (int i=0;i<MAX_PATTERNS;i++)
			get_pattern(i)->clear();
	
		for (int i=0;i<MAX_ORDERS;i++)
			set_order( i, ORDER_NONE );
	}
	
			
}


Song::ReverbMode Song::get_reverb_mode() {
	
	return effects.reverb_mode;
}
void Song::set_reverb_mode(ReverbMode p_mode) {
	
	effects.reverb_mode=p_mode;
}

void Song::set_chorus_delay_ms(int p_amount) {
	
	effects.chorus.delay_ms=p_amount;
}
void Song::set_chorus_separation_ms(int p_amount) {
	
	effects.chorus.separation_ms=p_amount;

}
void Song::set_chorus_depth_ms10(int p_amount) {
	
	effects.chorus.depth_ms10=p_amount;
	
}
void Song::set_chorus_speed_hz10(int p_amount) {
	
	effects.chorus.speed_hz10=p_amount;
	
}

int Song::get_chorus_delay_ms() {
	
	return effects.chorus.delay_ms;
	
}
int Song::get_chorus_separation_ms() {
	
	return effects.chorus.separation_ms;
}
int Song::get_chorus_depth_ms10() {
	
	return effects.chorus.depth_ms10;
	
}
int Song::get_chorus_speed_hz10() {
	
	return effects.chorus.speed_hz10;
	
}

void Song::cleanup_unused_patterns() {
	
	for (int i=0;i<MAX_PATTERNS;i++) {
		
		bool used=false;
		if (get_pattern(i)->is_empty())
			continue;
		
		for (int j=0;j<MAX_ORDERS;j++) {
			
			if (get_order(j)==i) {
				used=true;
				
			}
		}
		
		if (!used)
			get_pattern(i)->clear();
	}
	
}
void Song::cleanup_unused_instruments(){
	
	if (!has_instruments())
		return;
	
	bool instr_found[MAX_INSTRUMENTS];
	for (int i=0;i<MAX_INSTRUMENTS;i++)
		instr_found[i]=false;
	
	for (int i=0;i<MAX_PATTERNS;i++) {
		
		if (get_pattern(i)->is_empty())
			continue;
		
		for (int row=0;row<get_pattern(i)->get_length();row++) {
			
			
			for (int col=0;col<Pattern::WIDTH;col++) {
			
				Note n;
				n=get_pattern(i)->get_note( col,row );
				
				if (n.instrument<MAX_INSTRUMENTS)
					instr_found[n.instrument]=true;
			
			}
			
		}
		
	}
	
	for (int i=0;i<MAX_INSTRUMENTS;i++)
		if (!instr_found[i])
			get_instrument(i)->reset();
	
	
}
void Song::cleanup_unused_samples(){
	
	if (!has_instruments())
		return;
	
	bool sample_found[MAX_SAMPLES];
	for (int i=0;i<MAX_INSTRUMENTS;i++)
		sample_found[i]=false;
	
	for (int i=0;i<MAX_PATTERNS;i++) {
		
		if (get_pattern(i)->is_empty())
			continue;
		
		
		for (int row=0;row<get_pattern(i)->get_length();row++) {
			
			
			for (int col=0;col<Pattern::WIDTH;col++) {
			
				Note n;
				n=get_pattern(i)->get_note( col,row );
				
				if (n.instrument>=MAX_SAMPLES)
					continue;
				
				if (has_instruments()) {
							
					for (int nt=0;nt<Note::NOTES;nt++) {
						
						int smp=get_instrument(n.instrument)->get_sample_number(nt);
						if (smp<MAX_SAMPLES)
							sample_found[smp]=true;
					}
					
				} else {
					if (n.instrument<MAX_SAMPLES)
						sample_found[n.instrument]=true;
				}
			
			}
			
		}
		
	}
	
	for (int i=0;i<MAX_SAMPLES;i++)
		if (!sample_found[i])
			get_sample(i)->reset();
	
}
void Song::cleanup_unused_orders(){
	
	bool finito=false;
	for (int j=0;j<MAX_ORDERS;j++) {
			
		
		if (get_order(j)==ORDER_NONE)
			finito=true;
		if (finito)
			set_order(j,ORDER_NONE);
		
	}
	
}

void Song::clear_all_default_pan() {
	
	for (int i=0;i<MAX_INSTRUMENTS;i++)
		get_instrument(i)->set_pan_default_enabled( false ); //die!
	
	for (int i=0;i<MAX_SAMPLES;i++)
		get_sample(i)->set_pan_enabled( false ); //die!
	
}


void Song::clear_all_default_vol(){
	
	for (int i=0;i<MAX_SAMPLES;i++)
		get_sample(i)->set_default_volume( 64 ); //die!
	for (int i=0;i<MAX_INSTRUMENTS;i++)
		get_instrument(i)->set_volume_global_amount( Instrument::MAX_VOLUME );
	
}


int Song::get_order_in_use_count() {


	int order_count = 0;
	
	for (int i=(MAX_ORDERS-1);i>=0;i--) {

		
		if (get_order(i)!=ORDER_NONE) {
			order_count=i+1;
			break;
		}
	}
	
	return order_count;
}
int Song::get_pattern_in_use_count() {

	
	int pattern_count=0;
	
	for (int i=(Song::MAX_PATTERNS-1);i>=0;i--) {

		
		if (!get_pattern(i)->is_empty()) {
			pattern_count=i+1;
			break;
		}
	}

	return pattern_count;
}

int Song::get_instrument_in_use_count() {

	int instrument_count=0;
	
	for (int i=(Song::MAX_INSTRUMENTS-1);i>=0;i--) {

		Instrument *ins = get_instrument(i);
		bool in_use=false;
		
		for (int s = 0 ; s < Note::NOTES ; s++ ) {
		
			int smp_idx = ins->get_sample_number(s);
			if (smp_idx<0 || smp_idx>=Song::MAX_SAMPLES)
				continue;
				
			if (!get_sample(smp_idx)->get_sample_data().is_null()) {
				in_use=true;
				break;
			}
				
		}
		
		if (in_use) {
			instrument_count=i+1;
			break;
		}
	}

	return instrument_count;
}
#include <stdio.h>
int Song::get_channels_in_use() {

	int max=0;
	
	for (int p=0;p<Song::MAX_PATTERNS;p++) {
	
		Pattern *pat = get_pattern(p);
		if (pat->is_empty())
			continue;
		
		
		for (int c=(Pattern::WIDTH-1);c>=0;c--) {
		
			if (c<max)
				break;
		
			bool has_note=false;
			for (int r=0;r<pat->get_length();r++) {
			
				Note n = pat->get_note( c, r );
				if (!n.is_empty()) {
					has_note=true;
					break;
				}
			}
			
			if (has_note) {
			
				max=c+1;	
			}
		}
	}

	return max;
}


void Song::separate_in_one_sample_instruments(int p_instrument) {

	ERR_FAIL_COND( !variables.use_instruments );
	ERR_FAIL_INDEX( p_instrument, MAX_INSTRUMENTS );

	int remapped_count=0;
	
	signed char remap[MAX_SAMPLES];
	
	for (int i=0;i<MAX_SAMPLES;i++) {
	
		remap[i]=-1;
	}
	
	/* Find remaps */
	Instrument *ins=get_instrument(p_instrument);
	for (int i=0;i<Note::NOTES;i++) {
	
		int sn = ins->get_sample_number(i);
		
		// check for unusable sample
		if (sn<0 || sn>=MAX_SAMPLES || get_sample(sn)->get_sample_data().is_null())
			continue;
		printf("sample %i\n",sn);	
		if ( remap[sn] !=-1 ) {
			printf("already mapped to %i\n",remap[sn]);	
			continue;
		}
			
		printf("isn't remapped\n");	
			
		// find remap
		
		for (int j=0;j<MAX_INSTRUMENTS;j++) {
	
			if (!get_instrument(j)->is_empty()) 
				continue;
			
			printf("map to %i\n",j);	
			
			//copy
			*get_instrument(j)=*ins;
			
			// assign samples
			for (int k=0;k<Note::NOTES;k++) {
			
				get_instrument(j)->set_note_number(k,k);
				get_instrument(j)->set_sample_number(k,sn);
			}					
			remap[sn]=j;
			remapped_count++;
			break;
		}
		
		ERR_FAIL_COND(remap[sn]==-1); // no more free instruments
	}
	
	printf("remapped %i\n",remapped_count);	
	
	if (remapped_count<2) {
		//undo if only one is remapped
		for (int i=0;i<MAX_SAMPLES;i++) {
		
			if (remap[i]!=-1) {
			
				get_instrument(remap[i])->reset();
			}
		}
		return;
	}
	
	/* remap all song */
	
	for (int p=0;p<Song::MAX_PATTERNS;p++) {
	
		Pattern *pat = get_pattern(p);
		if (pat->is_empty())
			continue;
		
		
		for (int c=0;c<Pattern::WIDTH;c++) {
		
			for (int r=0;r<pat->get_length();r++) {
			
				Note n = pat->get_note(c,r);
				if (n.note<Note::NOTES && n.instrument==p_instrument) {
				
					int sn = ins->get_sample_number(n.note);
					if (remap[sn]==-1)
						pat->set_note(c,r,Note());
					else {
					
						n.instrument=remap[sn];
						pat->set_note(c,r,n);
					}
				}
			}
		}
	}
	
	ins->reset();
	
}


Song::Song() {
	
	reset();
}
Song::~Song() {
	
}




int get_song_next_order_idx(Song *p_song, int p_order_idx) {

	int baseorder,order_counter;

	order_counter=-1;

	baseorder=p_order_idx;

	do {

		baseorder++;
		if ( baseorder>(Song::MAX_ORDERS-1) ) baseorder=0;
		order_counter++;

	} while ( (p_song->get_order(baseorder)>=(Song::MAX_PATTERNS) ) && (order_counter<Song::MAX_ORDERS) );


	if (order_counter==Song::MAX_ORDERS) {

		return -1;

	} else {

		return baseorder;
	}

}
