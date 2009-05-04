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
                          saver_it.cpp  -  description
                             -------------------
    begin                : Sun Apr 22 2001
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

#include "saver_it.h"

#include <stdio.h>
#define BITBOOL(m_exp) ((m_exp)?1:0)


Saver_IT::Saver_IT(FileAccessWrapper *p_file){

	file=p_file;
	sample_count=0;
	pattern_count=0;
	instrument_count=0;
        orders_count=0;
	message_length=0;
}
Saver_IT::~Saver_IT(){
}

void Saver_IT::calculate_values() {

	int i;

	orders_count=1;
	for (i=Song::MAX_ORDERS-1;i>=0;i--) {

		if (song->get_order(i)!=Note::EMPTY) {
			orders_count=i+2; //for reason unknown, IT wont load the last one if i dont do this
			break;
		}
	}
	if (orders_count>Song::MAX_ORDERS) orders_count=Song::MAX_ORDERS;

	for (i=Song::MAX_SAMPLES-1;i>=0;i--) {

		sample_count=i+1;
		if ((!song->get_sample(i)->get_sample_data().is_null()) || (song->get_sample(i)->get_name()[0]!=0)) 	break;
	}
	for (i=Song::MAX_INSTRUMENTS-1;i>=0;i--) {

		int j,samples_found=0;
		instrument_count=i+1;
		for (j=0;j<Note::NOTES;j++) 
			if (song->get_instrument(i)->get_sample_number(j)!=Note::EMPTY) 		samples_found++;
		
		if (samples_found) break;
	}

	for (i=(Song::MAX_PATTERNS-1);i>=0;i--) {

		
		if (!song->get_pattern(i)->is_empty()) {
			printf("pattern %i has stuff\n",i);
			pattern_count=i+1;
			break;
		}
	}

	message_length=0; //song->variables.message.length()+1;
	while (song->get_message()[message_length]!=0)
		message_length++;
	
	if (message_length>0)
		message_length+=1;
	
	
//	message_length=0;
	/*
	message_length=song->variables.message.length()+1;
	if (message_length>8000) message_length=8000; */

}

void Saver_IT::write_header() {

	const char* file_head="IMPM";
	int i,tmpint;

	
	message_offset=0;


	file->store_byte_array((Uint8*)file_head,4);
	file->store_byte_array((Uint8*)song->get_name(),26);
	file->store_byte(song->get_row_highlight_minor());
	file->store_byte(song->get_row_highlight_major());

	file->store_word(orders_count);
	file->store_word(instrument_count);
	file->store_word(sample_count);
	file->store_word(pattern_count);
	file->store_word(0x214); // <- blex0rs
	file->store_word(0x214); // <- blex0rs
	tmpint=(BITBOOL(song->is_stereo()))|
	       (BITBOOL(song->has_instruments())<<2)| //uses instruments allways
	       (BITBOOL(song->has_linear_slides())<<3)|
	       (BITBOOL(song->has_old_effects())<<4)|
	       (BITBOOL(song->has_compatible_gxx())<<5);
	file->store_word(tmpint); // <- blex0rs
	file->store_word((message_length>0)?1:0);/// MESSAGE
	file->store_byte(song->get_global_volume());
	file->store_byte(song->get_mixing_volume());
	file->store_byte(song->get_speed());
	file->store_byte(song->get_tempo());
	file->store_byte(song->get_stereo_separation());
	file->store_byte(0);
	file->store_word(message_length);

	// Let's store the message just after the end of the header, so we dont have to deal with
	// coming back to rewrite the header
	file->store_dword(0); //message offset will go here
	file->store_byte_array( (Uint8*)"CHBI",4); // CHIBITRACKER uses this
	
	//file->store_dword(0);
	for (i=0;i<Pattern::WIDTH;i++) {

		if (song->is_channel_surround( i) ) {

			file->store_byte(100);
		} else if (song->is_channel_mute( i)) {

			file->store_byte(128);
		} else {

			file->store_byte(song->get_channel_pan( i));
		}
	}
	for (i=0;i<Pattern::WIDTH;i++) {

		file->store_byte(song->get_channel_volume( i));
	}

	write_orders();
	// Clear those ugly parapointers


	for (i=0;i<(instrument_count+sample_count+pattern_count);i++) file->store_dword(0);

	file->store_byte(0); //extended info version = 0
	
	/* Chibitracker Extended info */
	//reverb
	file->store_byte(song->get_reverb_mode());
	//chorus
	file->store_byte(song->get_chorus_speed_hz10());
	file->store_byte(song->get_chorus_delay_ms());
	file->store_byte(song->get_chorus_depth_ms10());
	file->store_byte(song->get_chorus_separation_ms());
	for (int i=0;i<Pattern::WIDTH;i++) {
		file->store_byte(song->get_channel_reverb(i) );
	}
	for (int i=0;i<Pattern::WIDTH;i++) {
		file->store_byte(song->get_channel_chorus(i) );
	}

}
void Saver_IT::write_sample(Sample *p_sample) {

	write_sample_internal(p_sample,true);
}

void Saver_IT::write_sample_internal(Sample *p_sample,bool p_write_data) {

	const char *sample_head="IMPS";
	const char *filename="-DEPRECATED-";
	char name[26];
	int tmpint,i;

	file->store_byte_array((Uint8*)sample_head,4);
	file->store_byte_array((Uint8*)filename,12);
	file->store_byte(0);
	file->store_byte(p_sample->get_global_volume());
	//FLAGS!
	SampleManager *sm=SampleManager::get_singleton();
	
	Sample_ID data=p_sample->get_sample_data();
	
	tmpint=BITBOOL(!data.is_null()) |
			(BITBOOL(data.is_null()?0:sm->is_16bits( data ))<<1)|
			(BITBOOL(data.is_null()?0:sm->is_stereo(  data ))<<2)|
     	      // (song->get_sample(p_sample_index)->data.is_stereo<<2)|
		// Flag 3 is zero - sample uncompressed
			(BITBOOL(data.is_null()?0:(sm->get_loop_type(data)!=LOOP_NONE))<<4)|
//     	       (BITBOOL(song->get_sample(p_sample_index)->data.sustain_loop_on)<<5)|
     	       (0<<5)|
			(BITBOOL(data.is_null()?0:(sm->get_loop_type(data)==LOOP_BIDI))<<6)|
     	       (0<<7);
//     	       (BITBOOL(song->get_sample(p_sample_index)->data.sustain_pingpong_loop)<<7);

	Sample *s=p_sample;
	file->store_byte(tmpint);
	file->store_byte(s->get_default_volume());
	file->store_byte_array((Uint8*)s->get_name(),26);
	file->store_byte(1); // signed!
	Uint8 defpan=s->get_pan();
	defpan|=s->is_pan_enabled()?128:0;
	file->store_byte(defpan);
	file->store_dword(data.is_null()?0:sm->get_size(data));
	file->store_dword(data.is_null()?0:sm->get_loop_begin(data));
	file->store_dword(data.is_null()?0:sm->get_loop_end(data));
	file->store_dword(data.is_null()?0:sm->get_c5_freq(data));
	file->store_dword(0);
	file->store_dword(0);
//	file->store_dword(song->get_sample(p_sample_index)->data.sustain_loop_begin);
//	file->store_dword(song->get_sample(p_sample_index)->data.sustain_loop_end);

 	file->store_dword(file->get_pos()+8);
	file->store_byte(s->get_vibrato_speed());
	file->store_byte(s->get_vibrato_depth());
	file->store_byte(s->get_vibrato_rate());
	file->store_byte(s->get_vibrato_type());

	/** SAMPLE IS STORED UNCOMPRESSED **/
 	if (p_write_data) {

		if (!data.is_null()) 
			write_sample_data(data);
		
  	}


}

void Saver_IT::write_sample_data(Sample_ID p_data) {
	
	if (p_data.is_null()) 
		return;

	
	SampleManager *sm=SampleManager::get_singleton();

	int ch=sm->is_stereo( p_data )?2:1;
	for (int c=0;c<ch;c++) {
	
		for (int i=0;i<sm->get_size(p_data);i++) {
			
			if (sm->is_16bits( p_data ) ) {
						
							
				Sint16 sample=sm->get_data( p_data,i,c );
						
				Uint16 *sp=(Uint16*)&sample;
						
				file->store_word( *sp );
					
			} else {
						
						
				Sint8 sample=sm->get_data( p_data,i,c ) >> 8;
						
				Uint8 *sp=(Uint8*)&sample;
						
				file->store_byte( *sp );
			}
		}
	}
	
}
void Saver_IT::write_instrument(int p_instrument_index) {

	write_instrument_internal(song->get_instrument(p_instrument_index));
}

void Saver_IT::write_instrument_internal(Instrument *p_aux_instr) {

	char *instrument_head="IMPI";
	char *filename="-DEPRECATED-";
	int tmpint,i;
	bool sample_used[Song::MAX_SAMPLES];
	int samples_used=0;
        Uint8 auxbyte;
        Instrument *instr=p_aux_instr;

	for (int i=0;i<Song::MAX_SAMPLES;i++)
		sample_used[i]=0;

	
	file->store_byte_array((Uint8*)instrument_head,4);
	file->store_byte_array((Uint8*)filename,12);
	file->store_byte(0);
	file->store_byte(instr->get_NNA_type());
	file->store_byte(instr->get_DC_type());
	file->store_byte(instr->get_DC_action());
	file->store_word(instr->get_volume_fadeout());
	file->store_byte(instr->get_pan_pitch_separation());
	file->store_byte(instr->get_pan_pitch_center());
	file->store_byte(instr->get_volume_global_amount());
	file->store_byte(instr->get_pan_default_amount() | (BITBOOL(!instr->is_pan_default_enabled())<<7));

	file->store_byte(instr->get_volume_random_variation());
	file->store_byte(instr->get_pan_random_variation());
	file->store_word(0x214);

	for(i=0;i<Note::NOTES;i++) {

		if (instr->get_sample_number(i)<Song::MAX_SAMPLES) {

			sample_used[instr->get_sample_number(i)]=true;
		}
	}
	for(i=0;i<Song::MAX_SAMPLES;i++) {

		if (sample_used[i]) samples_used++;
	}

	file->store_byte(samples_used);
	file->store_byte(0);
	file->store_byte_array((Uint8*)instr->get_name(),26);

	auxbyte=instr->get_filter_default_cutoff()*2;
	if (auxbyte>0x7F)
		auxbyte=0x7F;

	auxbyte|=instr->filter_use_default_cutoff()?0x80:0;
	file->store_byte(auxbyte); // filter.. NOT implemented

	auxbyte=instr->get_filter_default_resonance();
	auxbyte|=instr->filter_use_default_resonance()?0x80:0;
	file->store_byte(auxbyte); // filter.. NOT implemented

	file->store_byte(0); // midi channel.. NOT implemented
	file->store_byte(0); // midi program.. NOT implemented
	file->store_word(0); // midi bank.. NOT implemented

	for (i=0;i<Note::NOTES;i++) {

		file->store_byte(instr->get_note_number(i));
		file->store_byte(instr->get_sample_number(i)+1);
	}

	/* VOLUME ENVELOPE */

	tmpint=(BITBOOL(instr->get_volume_envelope()->is_enabled())) |
               (BITBOOL(instr->get_volume_envelope()->is_loop_enabled())<<1) |
               (BITBOOL(instr->get_volume_envelope()->is_sustain_loop_enabled())<<2);
	file->store_byte(tmpint);
	file->store_byte(instr->get_volume_envelope()->get_node_count());
	file->store_byte(instr->get_volume_envelope()->get_loop_begin());
	file->store_byte(instr->get_volume_envelope()->get_loop_end());
	file->store_byte(instr->get_volume_envelope()->get_sustain_loop_begin());
	file->store_byte(instr->get_volume_envelope()->get_sustain_loop_end());

	for (i=0;i<25;i++) {

		if (i>=instr->get_volume_envelope()->get_node_count()) {
			file->store_byte(0);	
			file->store_word(0);	
			continue;
		}
		file->store_byte(instr->get_volume_envelope()->get_node(i).value);
		file->store_word(instr->get_volume_envelope()->get_node(i).tick_offset);
	}
        file->store_byte(0);

	/* PANNING ENVELOPE */

	tmpint=(BITBOOL(instr->get_pan_envelope()->is_enabled())) |
               (BITBOOL(instr->get_pan_envelope()->is_loop_enabled())<<1) |
               (BITBOOL(instr->get_pan_envelope()->is_sustain_loop_enabled())<<2);
	file->store_byte(tmpint);
	file->store_byte(instr->get_pan_envelope()->get_node_count());
	file->store_byte(instr->get_pan_envelope()->get_loop_begin());
	file->store_byte(instr->get_pan_envelope()->get_loop_end());
	file->store_byte(instr->get_pan_envelope()->get_sustain_loop_begin());
	file->store_byte(instr->get_pan_envelope()->get_sustain_loop_end());

	for (i=0;i<25;i++) {

		if (i>=instr->get_pan_envelope()->get_node_count()) {
			file->store_byte(0);	
			file->store_word(0);	
			continue;
		}
		
		file->store_byte(instr->get_pan_envelope()->get_node(i).value);
		file->store_word(instr->get_pan_envelope()->get_node(i).tick_offset);

	}
        file->store_byte(0);

	/* PITCH ENVELOPE */

	tmpint=(BITBOOL(instr->get_pitch_filter_envelope()->is_enabled())) |
               (BITBOOL(instr->get_pitch_filter_envelope()->is_loop_enabled())<<1) |
               (BITBOOL(instr->get_pitch_filter_envelope()->is_sustain_loop_enabled())<<2) |
               (BITBOOL(instr->is_pitch_use_as_filter())<<7);

	file->store_byte(tmpint);
	file->store_byte(instr->get_pitch_filter_envelope()->get_node_count());
	file->store_byte(instr->get_pitch_filter_envelope()->get_loop_begin());
	file->store_byte(instr->get_pitch_filter_envelope()->get_loop_end());
	file->store_byte(instr->get_pitch_filter_envelope()->get_sustain_loop_begin());
	file->store_byte(instr->get_pitch_filter_envelope()->get_sustain_loop_end());

	for (i=0;i<25;i++) {

		if (i>=instr->get_pitch_filter_envelope()->get_node_count()) {
			file->store_byte(0);	
			file->store_word(0);	
			continue;
		}
		
		Sint8 height=(instr->get_pitch_filter_envelope()->get_node(i).value+32);
		if (height>64)
			height=64;
		height-=32;
		file->store_byte(height);
		file->store_word(instr->get_pitch_filter_envelope()->get_node(i).tick_offset);
	}

        file->store_byte(0);

	//wasted.. eh?
        for (i=0;i<4;i++) file->store_byte(0);

}

void Saver_IT::write_orders() {

	int i;

	for (i=0;i<orders_count;i++) {

         	if (song->get_order(i)==Note::EMPTY) {

			file->store_byte(255);
		} else if (song->get_order(i)==ORDER_BREAK) {

			file->store_byte(254);

		} else {

			file->store_byte(song->get_order(i));
		}
	}
}

void Saver_IT::write_message() {

	if (message_length==0) return;

	message_offset=file->get_pos();
	

	for (int i=0;i<message_length;i++) {

		char c=song->get_message()[i];
		if (c=='\n')
			c=='\r';
		file->store_byte( c );

	}
	//file->store_byte_array((Uint8*)song->variables.message.c_str(),message_length);
}



void Saver_IT::write_pattern(int p_pattern) {

	Note note_cache[Pattern::WIDTH];
	bool first_parameter[Pattern::WIDTH];
	Uint8 mask_cache[Pattern::WIDTH];

	Note empty_note;
        int i,j;
	int length;
	Uint32 begin_pos;
	Uint32 end_pos;


	enum {

		WRITE_NOTE=(1),
		WRITE_INSTRUMENT=(1<<1),
		WRITE_VOLUME=(1<<2),
		WRITE_COMMAND=(1<<3),
		WRITE_NOTE_MASK=(1<<4),
		WRITE_INSTRUMENT_MASK=(1<<5),
		WRITE_VOLUME_MASK=(1<<6),
		WRITE_COMMAND_MASK=(1<<7)
	};

	for(j=0;j<Pattern::WIDTH;j++) {

		first_parameter[j]=true;
		mask_cache[j]=0;
	}


	begin_pos=file->get_pos();

	file->store_word(0);
	file->store_word(song->get_pattern(p_pattern)->get_length());
	file->store_dword(0);

        length=0;

	for (i=0;i<song->get_pattern(p_pattern)->get_length();i++) {

		for(j=0;j<Pattern::WIDTH;j++) {

			if (!(empty_note==song->get_pattern(p_pattern)->get_note(j,i))) {
	                        // we have some data, boss

				int flags=0;
				Note src_note=song->get_pattern(p_pattern)->get_note(j,i);

				//Check for what changed, and fill cache!

				if (src_note.note!=Note::EMPTY) {

					if (src_note.note!=note_cache[j].note) {

						flags|=WRITE_NOTE;
						note_cache[j].note=src_note.note;

					} else {

						flags|=WRITE_NOTE_MASK;
					}
				}

				if (src_note.instrument!=Note::EMPTY) {

					if (src_note.instrument!=note_cache[j].instrument) {

						flags|=WRITE_INSTRUMENT;
						note_cache[j].instrument=src_note.instrument;

					} else {

						flags|=WRITE_INSTRUMENT_MASK;
					}
				}


				if (src_note.volume!=Note::EMPTY) {

					if (src_note.volume!=note_cache[j].volume) {

						flags|=WRITE_VOLUME;
						note_cache[j].volume=src_note.volume;

					} else {

						flags|=WRITE_VOLUME_MASK;
					}
				}

				if ((src_note.command!=Note::EMPTY) || (src_note.parameter!=0)){

					if ((src_note.command!=note_cache[j].command) || (src_note.parameter!=note_cache[j].parameter)) {

						flags|=WRITE_COMMAND;
						note_cache[j].command=src_note.command;
						note_cache[j].parameter=src_note.parameter;
						if ((note_cache[j].command==('Z'-'A')) && (note_cache[j].parameter<0x7F)) {
							note_cache[j].parameter*=2;
							if (note_cache[j].parameter>=0x7F)
								note_cache[j].parameter=0x7F;
						}


					} else {

						flags|=WRITE_COMMAND_MASK;
					}
				}

				//Now let's write the note

				//first the flags
				if (flags!=mask_cache[j]) {

					int tmp_chanmark;
					tmp_chanmark=(j+1)|128;
					file->store_byte(tmp_chanmark);
					file->store_byte(flags);
					length+=2;
					mask_cache[j]=flags;
				} else {
                                        int tmp_chanmark;
					tmp_chanmark=(j+1);
					file->store_byte(tmp_chanmark);
					length++;
				}

				if (flags&WRITE_NOTE) {

					Uint8 dest_note;

					if (src_note.note==Note::EMPTY) {

                                         	dest_note=0;
					} else if (src_note.note==Note::OFF) {

                                         	dest_note=255;
					} else if (src_note.note==Note::CUT) {

                                         	dest_note=254;
					} else {

                                         	dest_note=src_note.note;
                                        }

					file->store_byte(dest_note);
					length++;
				}

				if (flags&WRITE_INSTRUMENT) {

					file->store_byte(src_note.instrument+1);
					length++;

				}

				if (flags&WRITE_VOLUME) {

					file->store_byte(src_note.volume);
					length++;
				}

				if (flags&WRITE_COMMAND) {

					file->store_byte(src_note.command+1);
					file->store_byte(src_note.parameter);
					length+=2;
				}

			}

		}
		file->store_byte(0);
		length++;
	}

        end_pos=file->get_pos();
        file->seek(begin_pos);
	file->store_word(length);
        file->seek(end_pos);
}




void Saver_IT::write_song() {

	int i;
	calculate_values();
	write_header();
	write_message();



	for (i=0;i<instrument_count;i++) {

		instrument_offsets[i]=file->get_pos();


		write_instrument(i);
	}



	for (i=0;i<sample_count;i++) {

		sample_offsets[i]=file->get_pos();


		write_sample(song->get_sample(i));
	}



	for (i=0;i<pattern_count;i++) {

		pattern_offsets[i]=file->get_pos();


		write_pattern(i);
	}

	file->seek(0xC0+orders_count);




	for (i=0;i<instrument_count;i++) {


		file->store_dword(instrument_offsets[i]);
	}
	for (i=0;i<sample_count;i++) {


		file->store_dword(sample_offsets[i]);
	}
	for (i=0;i<pattern_count;i++) {


		file->store_dword(pattern_offsets[i]);
	}



	file->seek(0x38);
	file->store_dword(message_offset);


}

static char UCASE(char p_c) {
	
	if (p_c>='a' && p_c<='z')
		p_c-='a'-'A';
	
	return p_c;
}

Saver::Error Saver_IT::save_song(const char *p_filename,Song *p_song) {

	song=p_song;
	
	int file_len=0;
	while (p_filename[file_len]!=0)
		file_len++;
	
	
	if ( (file_len<4) || UCASE(p_filename[file_len-1])!='T' || UCASE(p_filename[file_len-2])!='I' ||  (p_filename[file_len-3])!='.' ) return UNKNOWN_EXTENSION;

	if (file->open(p_filename,FileAccessWrapper::WRITE)) {

		return SAVE_ERROR;
	}

	write_song();

	//I guess i should check for errors here...


	file->close();

	return SAVE_OK;

}

Saver::Error Saver_IT::save_sample(const char *p_filename,Sample *p_sample) {

	
	int file_len=0;
	while (p_filename[file_len]!=0)
		file_len++;
	
	
	if ( (file_len<4) || UCASE(p_filename[file_len-1])!='S' || UCASE(p_filename[file_len-2])!='T' || UCASE(p_filename[file_len-3])!='I' ||  (p_filename[file_len-4])!='.' ) return UNKNOWN_EXTENSION;

	Sample * smp=p_sample;

	if (smp->get_sample_data().is_null()) return SAVE_ERROR;

	if (file->open(p_filename,FileAccessWrapper::WRITE)) return SAVE_ERROR;

	write_sample(smp);
 	file->close();

	return SAVE_OK;
}
Saver::Error Saver_IT::save_instrument(const char *p_filename,Song *p_song,int p_instrument_index) {

	
	int file_len=0;
	while (p_filename[file_len]!=0)
		file_len++;
	
	
	if ( (file_len<4) || UCASE(p_filename[file_len-1])!='I' || UCASE(p_filename[file_len-2])!='T' || UCASE(p_filename[file_len-3])!='I' ||  (p_filename[file_len-4])!='.' ) return UNKNOWN_EXTENSION;
	
	song=p_song;
	
  	int i,j;
 	
	char samples_used[Song::MAX_SAMPLES];
	Uint32 data_offset[Song::MAX_SAMPLES];
	int samples_used_count=0;
  	int current_offset;


	Instrument instr=*song->get_instrument(p_instrument_index);

	if (file->open(p_filename,FileAccessWrapper::WRITE)) {

		return SAVE_ERROR;
	}

        //replace sample stuff for 1->x stuff
	for (i=0;i<Note::NOTES;i++) {

		int sampnumb=instr.get_sample_number( i );
     		int newnumb;
                bool has_it=false;

                //if sample slot is empty or points to an ununsed sample, skip it
                if (sampnumb==Note::EMPTY) continue;
		if (song->get_sample(sampnumb)->get_sample_data().is_null()) {

			instr.set_sample_number(i,Note::EMPTY);
   			continue;
                }


                //check if we have it cached
                for (j=0;j<samples_used_count;j++)
                	if (samples_used[j]==sampnumb) {
                   		newnumb=j;
                 		has_it=true;
                   	}

                //if not, cache it
 		if (!has_it) {
     			samples_used[samples_used_count++]=sampnumb;
			newnumb=samples_used_count-1;


          	}


		//and assign the new relative-to-zero number
		instr.set_sample_number( i, newnumb );
   	}

    	//and write the thing down!
	write_instrument_internal(&instr);

 	//now let's write samples..

	for (i=0;i<samples_used_count;i++) {



		write_sample_internal(song->get_sample( samples_used[i]),false);
  		current_offset=file->get_pos()-8;
	 	data_offset[i]=current_offset;
   	}
 		
	for (i=0;i<samples_used_count;i++) {

   		int prev_offset,aux_offset;

     		prev_offset=file->get_pos();
		
		write_sample_data( song->get_sample( samples_used[i] )->get_sample_data() );
		//file->store_byte_array((Uint8*)song->get_sample(samples_used[i])->data.get_data_16(),song->get_sample(samples_used[i])->data.is_16bit()?song->get_sample(samples_used[i])->data.get_size()*2:song->get_sample(samples_used[i])->data.get_size());

  		aux_offset=file->get_pos();

		file->seek(data_offset[i]);
  		file->store_dword(prev_offset);

		file->seek(aux_offset);
  	}


	file->close();

	return SAVE_OK;

}



