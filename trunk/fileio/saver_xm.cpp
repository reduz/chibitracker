//
// C++ Implementation: saver_xm
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "saver_xm.h"
#include "player/tables.h"
#include <stdio.h>

static char UCASE(char p_c) {
	
	if (p_c>='a' && p_c<='z')
		p_c-='a'-'A';
	
	return p_c;
}


Saver::Error Saver_XM::save_song(const char *p_filename,Song *p_song) {

	Song *song=p_song;
	
	int file_len=0;
	while (p_filename[file_len]!=0)
		file_len++;
	
	
	if ( (file_len<4) || UCASE(p_filename[file_len-1])!='M' || UCASE(p_filename[file_len-2])!='X' ||  (p_filename[file_len-3])!='.' ) return UNKNOWN_EXTENSION;

	if (f->open(p_filename,FileAccessWrapper::WRITE)) {

		return SAVE_ERROR;
	}

	bool uses_instruments = song->has_instruments();
	if (!uses_instruments)
		song->make_instruments_from_samples();

	/** HEADER **/

	/* MAGIC STRING */
	f->store_byte_array( (Uint8*)"Extended Module: ", 17 );
	/* SONG NAME */
	for (int i=0;i<20;i++)
		f->store_byte( song->get_name()[i] );
	/* $1a */
	f->store_byte(0x1a); 
	/* TRACKER NAME */
	f->store_byte_array( (Uint8*)"Chibi Tracker XM       ",20); //plus extra zeros
	/* XM VERSION NUMBER */
	f->store_word(0x0104);	
	/* HEADER SIZE */
	f->store_dword( 276 ); /* XM Header size, 276 bytes */
	/* ORDER COUNT */
	int order_count = song->get_order_in_use_count();
	f->store_word( order_count );
	/* RESTART POS */
	f->store_word( 0 ); // IT has no restart position
	/* CHANNEL COUNT */
	int channel_count = song->get_channels_in_use();
	printf("song has %i chans\n",channel_count);
	if (channel_count&1)
		channel_count++; // channels must be even in XM
	f->store_word(channel_count);
	/* PATTERN COUNT */	
	int pattern_count=song->get_pattern_in_use_count();
	f->store_word( pattern_count );
	/* INSTRUMENT COUNT */
	int instrument_count=song->get_instrument_in_use_count();
	f->store_word( instrument_count );
	/* FLAGS */
	f->store_word( song->has_linear_slides()? 1 : 0 ); /* just that bit */
	/* TEMPO/BMP */
	f->store_word( song->get_speed() );
	f->store_word( song->get_tempo() );
	/* ORDER TABLE */
	for (int i=0;i<256;i++) {
	
		if (i>=order_count) {
			f->store_byte(0);
			continue;
		}
		
		
		f->store_byte( song->get_order( i ) );
		
	}
	
	/** PATTERNS **/
	
	for (int i=0;i<pattern_count;i++) {
	
		Pattern *pat = song->get_pattern(i);
	
		/* HEADER */
		f->store_dword(9);
		/* PACKING TYPE */
		f->store_byte(0); // always 0
		/* LENGTH */
		f->store_word( pat->get_length() );
		/* PACKED SIZE */
		int packedpos = f->get_pos();
		f->store_word(0); //packed size, temporary
		
		if (pat->is_empty())
			continue; /* no pattern data, goto next pattern */
			
		/* NOTE DATA */
		for (int r=0;r<pat->get_length();r++) {
			for (int c=0;c<channel_count;c++) {
			
				Note n = pat->get_note( c, r );	
				
				if (n.is_empty()) {
					f->store_byte(0x80);
					continue;
				}
					
				if (n.note==Note::CUT) {
				
					f->store_byte(0x84); // volume mask
					f->store_byte(0x10); // zero volume (note cut)
					continue;
				}
				
				Uint8 mask=0;
				Uint8 note=0;
				bool notecut=false;
				
				if (n.note!=Note::EMPTY) {
				
					mask|=1;
					if (n.note<12)
						n.note=0;
					else if (n.note<108)
						note=(n.note-12)+1;
					else if (n.note==Note::OFF)
						note=97;
					else
						mask=0; // no note
						
				}
				
				Uint8 instrument=0;
				
				if (n.instrument!=Note::EMPTY) {
				
					mask|=2;
					instrument=n.instrument+1;
					
				}
				
				Uint8 volume;
				
				if (n.volume!=Note::EMPTY) {
				
					mask|=4;
				
					if (n.volume<=64) {
					
						volume=n.volume+0x10;
					} else if (n.volume<75) { // finevol slide up
					
						volume=(n.volume-65)+0x90;
					} else if (n.volume<85) { //finevol slide down
					
						volume=(n.volume-75)+0x80;
					} else if (n.volume<95) { //vol slide up
					
						volume=(n.volume-85)+0x70;
					
					} else if (n.volume<105) { //vol slide down
					
						volume=(n.volume-95)+0x60;
					
					} else if (n.volume<128) { // nothing
					
						volume=0;
						mask&=~4;
					} else if (n.volume<=192) {
					
						volume=0xc0+((int)n.volume-128)*(int)0xf/(int)64;
						
					} else if (n.volume<203) {
					
						volume=0xf0+n.volume-193;
					} else if (n.volume<213) {
					
						volume=0xb0+n.volume-203;
					} else {
					
						volume=0;
						mask&=~4;					
					}
				}
				
				Uint8 command=0;
				Uint8 parameter=n.parameter;
#define XM_CMD(m_v)\
	((m_v>='A' && m_v<='F')?((m_v-'A')+0xa):( (m_v >='0' && m_v <='9')?(m_v-'0'):0 ))
				if (n.command!=Note::EMPTY) {
				
					switch(n.command+'A') {
					
						case 'A': {
						
							if (parameter<0x20) 
								command=XM_CMD('F');
							else
								parameter=0;
						} break;
						case 'B': {
						
							command=XM_CMD('B');
						} break;
						case 'C': {
						
							command=XM_CMD('D');
						} break;
						case 'D': {
						
							if ( (parameter&0xf)==0xf && (parameter&0xf0) ) {
							
								command=XM_CMD('E');
								parameter=0xA0|(parameter>>4);
							} else if ( (parameter&0xf0)==0xf0 && (parameter&0xf) ) {
							
								command=XM_CMD('E');
								parameter=0xB0|(parameter&0xF);
							} else {
								command=XM_CMD('A');
							}
								
						} break;
						case 'E': {
													
							command=XM_CMD('2');
						} break;
						case 'F': {
						
							command=XM_CMD('1');
						} break;
						case 'G': {
						
							command=XM_CMD('3');
						} break;
						case 'H': {
						
							command=XM_CMD('4');
							if (!song->has_old_effects()) {
							
								Uint8 d=parameter&0xF;
								parameter&=0xF0;
								d>>=2;
								parameter|=2;
							}
						} break;
						case 'I': {
						
							command=XM_CMD('T');
						} break;
						case 'J': {
						
							command=XM_CMD('0');
						} break;
						case 'K': {
						
							command=XM_CMD('6');
						} break;
						case 'L': {
						
							command=XM_CMD('5');
						} break;
						case 'O': {
						
							command=XM_CMD('9');
						} break;
						case 'P': {
						
							command=XM_CMD('P');
						} break;
						case 'Q': {
						
							command=XM_CMD('R');
						} break;
						case 'R': {
						
							command=XM_CMD('7');
						} break;
						case 'S': {
						
							command=XM_CMD('E');
							char val=parameter&0xF;
							switch(parameter>>4) {
							
								case 0x3: parameter=0x40|val; break;
								case 0x4: parameter=0x70|val; break;
								case 0x6: parameter=0xE0|val; break;
								case 0xD: parameter=0xD0|val; break;
								case 0xC: parameter=0xC0|val; break;
								default: command=0; parameter=0; break;
							}
							
						} break;
						case 'T': {
						
							if (parameter>=0x20) 
								command=XM_CMD('F');
							else 
								parameter=0;
						} break;
						case 'V': {
						
							command=XM_CMD('G');
						} break;
						case 'W': {
						
							command=XM_CMD('H');
						} break;
						case 'X': {
						
							command=XM_CMD('8');
						} break;
						default: {
						
							parameter=0;
						} break;
					}
					
					if (command)
						mask|=8;
					if (parameter)
						mask|=16;
/*					if (command!=0 || parameter!=0) {
					
						mask|=8;
						mask|=16;
					} */
				
				}
				
				
				if (mask!=0x1F) { // if all must not be saved, then write mask
				
					f->store_byte( 0x80 | mask );
				} 
				
				if (mask&1) // store note
					f->store_byte( note );
				if (mask&2) // store instrument
					f->store_byte( instrument );
				if (mask&4) // store volume
					f->store_byte( volume );
				if (mask&8) // store command
					f->store_byte( command );
				if (mask&16) // store parameter
					f->store_byte( parameter );
			
			}
		}
		
		int currentpos=f->get_pos();
		f->seek( packedpos );
		f->store_word( currentpos-(packedpos+2) );
		f->seek( currentpos );
		
		
	}
	
	
	/** INSTRUMENTS **/
	
	
	for (int i=0;i<instrument_count;i++) {
	
		Instrument *ins=song->get_instrument(i);
		
		char sample_table[16]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
		char note_table[96];
		int sample_count=0;

		for (int s=0;s<96;s++)
			note_table[i]=0; //init
			
		for (int n=12;n<108;n++) {
	
			int sample_idx = ins->get_sample_number( n );
			if (sample_idx>=Song::MAX_SAMPLES)
				continue;
			if (song->get_sample( sample_idx )->get_sample_data().is_null())
				continue;
				
			
			for (int t=0;t<16;t++) {
			
				if (sample_table[t]==sample_idx) {
					note_table[n-12]=t;
					break; //already have it
				}
				
				if (sample_table[t]==-1) {
					sample_table[t]=sample_idx;
					note_table[n-12]=t;
					sample_count=t+1;
					break;
				}
			}
		}
		
		/* HEADER SIZE */
		int header_pos=f->get_pos();
		f->store_dword(29); //default 29
		/* INSTRUMENT NAME */
		for (int n=0;n<22;n++)
			f->store_byte( ins->get_name()[n] );
		/* TYPE */
		f->store_byte(0); //always 0!
		
		/* NUMBER OF SAMPLES */
		f->store_word(sample_count);
		
		if (sample_count==0)
			continue;
			
		/* SAMPLE HEADER */
		
		f->store_dword(40); //sample header size (40)
		
		/* SAMPLE NUMBERS */
		f->store_byte_array((Uint8*)note_table,96);
		
		Envelope *ve = ins->get_volume_envelope();
		Envelope *pe = ins->get_pan_envelope();
		
		for (int e=0;e<12;e++) {
		
			if (e>=ve->get_node_count()) {
			
				f->store_dword(0);
				continue;
			}
			
			f->store_word( ve->get_node(e).tick_offset );
			f->store_word( ve->get_node(e).value );
		}
		
		for (int e=0;e<12;e++) {
		
			if (e>=pe->get_node_count()) {
			
				f->store_dword(0);
				continue;
			}
			
			f->store_word( pe->get_node(e).tick_offset );
			f->store_word( pe->get_node(e).value+32 );
		}
		
		int ve_points = ve->get_node_count();
		int pe_points = pe->get_node_count();
		
		f->store_byte( ve_points>12 ? 12 : ve_points );
		f->store_byte( pe_points>12 ? 12 : pe_points );
		
		f->store_byte( ve->get_sustain_loop_begin() );
		f->store_byte( ve->get_loop_begin() );
		f->store_byte( ve->get_loop_end() );
		
		f->store_byte( pe->get_sustain_loop_begin() );
		f->store_byte( pe->get_loop_begin() );
		f->store_byte( pe->get_loop_end() );
		
		f->store_byte( ((ve->is_enabled())?1:0) | ((ve->is_sustain_loop_enabled())?2:0) | ((ve->is_loop_enabled())?4:0) );
		f->store_byte( ((pe->is_enabled())?1:0) | ((pe->is_sustain_loop_enabled())?2:0) | ((pe->is_loop_enabled())?4:0) );
		f->store_dword(0); // no vibrato in instruments for IT
		f->store_word( ins->get_volume_fadeout() << 4 ); //fadeout
		f->store_word(0); //reserved
		
		for (int g=0;g<20;g++)
			f->store_byte(0); /* xm files have often 20 extra bytes of garbage */
			
		int current_pos = f->get_pos();
		f->seek(header_pos);		
		f->store_dword( current_pos - (header_pos ) );
		f->seek(current_pos);
		
		/** SAMPLE DATA HEADERS */
		
		for (int s=0;s<sample_count;s++) {
			
			SampleManager *sm = SampleManager::get_singleton();
			Sample *smp = song->get_sample(sample_table[s]);
			Sample_ID smp_id = smp->get_sample_data();
			
			int shf=sm->is_16bits( smp_id ) ? 1 : 0;
			f->store_dword( sm->get_size( smp_id ) << shf );
			f->store_dword( sm->get_loop_begin( smp_id ) << shf );
			f->store_dword( (sm->get_loop_end( smp_id ) << shf) - (sm->get_loop_begin( smp_id ) << shf) );
			f->store_byte( smp->get_default_volume() );
			
			int freq = sm->get_c5_freq( smp_id )*2;
			int note=-1,fine=0;
			bool found=false;
			
			for (int n=-128;n<127;n++) {
				/* find note */
				int rnote = 60+n;
				if (rnote<0)
					continue;
				int fr = Tables::get_linear_frequency(Tables::get_linear_period(rnote<<1,0));
								
				if (fr>freq) {
					/* find fine */
				
					for (int f=0;f>=-128;f--) {
					
						int fr = Tables::get_linear_frequency(Tables::get_linear_period(rnote<<1,f));
						
						if (fr < freq || f==-128) {
						
							note=n;
							fine=f+1;
							break;
						}
					}
				}
				
				if (note!=-1)
					break;
			
			}
			
			if (note==-1)
				note=0; // meh, not found
				
			
			f->store_byte( (signed char)fine );
			f->store_byte( sm->get_loop_type( smp_id ) | ( sm->is_16bits(smp_id)? 16 : 0));
			if (ins->is_pan_default_enabled())
				f->store_byte( (int)ins->get_pan_default_amount() * 255 / 64 );
			else
				f->store_byte( (int)smp->get_pan() * 255 / 64 );
			f->store_byte( (signed char)note );
			f->store_byte( 0 ); //reserved
			
			for (int sn=0;sn<22;sn++)
				f->store_byte( smp->get_name()[sn] );
		}
		
		/** SAMPLE DATA FRAMES **/
		
		for (int s=0;s<sample_count;s++) {
		
			
			SampleManager *sm = SampleManager::get_singleton();
			Sample *smp = song->get_sample(sample_table[s]);
			Sample_ID smp_id = smp->get_sample_data();
		
			Sint16 old=0;
			
			for (int fr=0;fr<sm->get_size( smp_id );fr++) {
			
				Sint16 sd = sm->get_data( smp_id, fr, 0 );
				
				if (sm->is_16bits( smp_id )) { 
					
					Sint32 diff =  sd - old;
					
					if (diff<-32768)
						diff=-32768;
					if (diff>32767)
						diff=32767;
					
					f->store_word( (Sint16)diff );
					old+=diff;
				} else {
				
					sd>>=8;
				
					Sint32 diff =  sd - old;
					
					if (diff<-128)
						diff=-128;
					if (diff>127)
						diff=127;
					
					f->store_byte( (Sint8)diff );
					old+=diff;
				
				}
				
				
			}
		}
	}

	f->close();
	
	if (!uses_instruments)
		song->set_instruments(false);
	
	return SAVE_OK;
	
}
Saver::Error Saver_XM::save_sample(const char *p_filename,Sample *p_sample) {

	return UNKNOWN_EXTENSION;
}
Saver::Error Saver_XM::save_instrument(const char *p_filename,Song *p_song,int p_instrument_index) {

	return UNKNOWN_EXTENSION;

}


Saver_XM::Saver_XM(FileAccessWrapper *p_file)
{
	f=p_file;
}


Saver_XM::~Saver_XM()
{
}


