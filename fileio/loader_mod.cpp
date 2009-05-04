//
// C++ Implementation: loader_mod
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "loader_mod.h"
#include <stdio.h>

static bool tag_equal_to(const char *p_tag, const char *p_string) {

	return(	p_tag[0]==p_string[0] && 
		p_tag[1]==p_string[1] && 
		p_tag[2]==p_string[2] && 
		p_tag[3]==p_string[3]);
}
/* ProTracker period table */
Uint16 period_table[6*12] = {
	1712,1616,1524,1440,1356,1280,1208,1140,1076,1016,960,907,
	856,808,762,720,678,640,604,570,538,508,480,453,
	428,404,381,360,339,320,302,285,269,254,240,226,
	214,202,190,180,170,160,151,143,135,127,120,113,
	107,101,95,90,85,80,75,71,67,63,60,56,
	53,50,47,45,42,40,37,35,33,31,30,28
};


Loader::Error Loader_MOD::load_song(const char *p_file,Song *p_song) {

	if (file->open(p_file,FileAccessWrapper::READ)) {
	//printf("Can't open file! %s\n",p_file);
		return FILE_CANNOT_OPEN;
	};

	/* FIRST OF ALL, one needs to read the .mod file format tag */
	file->seek( 1080 ); //located at 1080
	
	char format_tag[4];
	
	file->get_byte_array( (Uint8*)format_tag, 4 );
	
	int channels=-1;
	
	/** THE PAIN!! - COMPARE TAGS */
	
	/* Classic 4-chan */
	if (tag_equal_to(format_tag,"M.K.") )
		channels=4;
	if (tag_equal_to(format_tag,"FLT4") )
		channels=4;
	if (tag_equal_to(format_tag,"M!K!") )
		channels=4;
	
	/* 8 Channel MODS */
	
	if (tag_equal_to(format_tag,"FLT8") )
		channels=2;
	
	if (tag_equal_to(format_tag,"CD81") )
		channels=2;
	
	/* Custom channel MODS */
	
	for (int i=1;i<=32;i++) {
		
		if (i<10) { // up to 9 channels mods
			
			/* Old Take Tracker */
			char old_take_tracker[4]={'T','D','Z','0'+i};
			
			if (tag_equal_to(format_tag,old_take_tracker)) {
				
				channels=i;
				break;
			}
			
			/* Contemplates many XCHN Formats */
			char xchn[4]={'0'+i,'C','H','N'};
			
			if (tag_equal_to(format_tag,xchn)) {
				
				channels=i;
				break;
			}
		}
		
		/* Fast Tracker */
		char fast_tracker[4]={'0'+(i/10),'0'+(i%10),'C','H'};
		
		if (tag_equal_to(format_tag,fast_tracker)) {
				
			channels=i;
			break;
		}
		
	}
	
	
	if (channels==-1) {
		
		file->close();
		return FILE_UNRECOGNIZED;
	}
	
	
	
	/** Load Song INFO */
	
	file->seek( 0 ); //go to begining of file
	
	file->set_endian_conversion( true );
	p_song->reset();
	p_song->set_instruments( false );
	
	char name[21];
	
	file->get_byte_array( (Uint8*)name,20);
	name[20]=0;
	
	p_song->set_name(name);
	p_song->set_old_effects( true );
	p_song->set_linear_slides( false );
	p_song->set_compatible_gxx( true );
	
	
	
	SampleManager *sm=SampleManager::get_singleton();
	
	int instruments=31;
	
	for (int i=0;i<instruments;i++) {
		
		char sample_name[23];
		file->get_byte_array( (Uint8*)sample_name,22);
		sample_name[22]=0;
		
		Uint32 sample_len=file->get_word();
		sample_len<<=1;
		
		Uint8 fine_nibble=file->get_byte()&0xF;
		
		
		//(Sint8)(fine_nibble & 7) - (Sint8)(fine_nibble & 8); //yesso's genius trick
		// boo, I can't use it :( but i leave it here because of how cool it is
		Uint8 linear_volume=file->get_byte(); //0 .. ?
		
		Uint32 loop_begin=file->get_word(); //0 .. ?
		loop_begin<<=1;		
		Uint32 loop_end=file->get_word(); //0 .. ?
		loop_end<<=1;		
		
		if (sample_len>0) {
			
			Sample_ID sid=sm->create( false, false, sample_len );
			
			if (sid.is_null()) {
				
				file->close();
				return FILE_OUT_OF_MEMORY;
			}
			
			if (loop_end>2) {
				sm->set_loop_begin( sid, loop_begin );
				sm->set_loop_end( sid, loop_end+loop_begin );
				sm->set_loop_type( sid,LOOP_FORWARD );
			}
			static const Uint16 fine_to_freq[16]={
				8363,8413,8463,8529,8581,8651,8723,8757,
				7895,7941,7985,8046,8107,8169,8232,8280
			};
			
			sm->set_c5_freq( sid, fine_to_freq[fine_nibble] );
			p_song->get_sample(i)->set_sample_data(sid);
		}
		
		p_song->get_sample(i)->set_name(sample_name);
		p_song->get_sample(i)->set_default_volume( linear_volume );
		
		
		
	}
	
	/* pan for MODs */
	for (int i=0;i<channels;i++)
		p_song->set_channel_pan( i, (((i&3)==1) || ((i&3)==2)) ? 0: 64);

		
	Uint8 order_count=file->get_byte();
	Uint8 loop_to=file->get_byte();
	
	
	int pattern_count=0;
	
	for (int i=0;i<128;i++) {
		
		Uint8 order=file->get_byte();
		
		
		if (i<order_count) {
			p_song->set_order(i,order);
			
			/* Determine the amount of patterns */
			if ((order+1)>pattern_count)
				pattern_count=order+1;
		} else
			p_song->set_order( i, ORDER_NONE );
	}
	
	if (instruments==31)
		file->get_dword(); // identiefier, now skip it
	
	for (int i=0;i<pattern_count;i++) {
		
		for(int line=0;line<64;line++) {
		
			for(int column=0;column<channels;column++) {
				
				Uint32 note_w=file->get_dword();
				
				Note note;
				
				note.instrument=(note_w>>12)&0xF;
				note.instrument|=(note_w>>24)&0xF0;
				
				if (note.instrument==0)
					note.instrument=Note::EMPTY;
				else
					note.instrument--;
				
				note.parameter=note_w&0xFF;
				
				int cmd=(note_w>>8)&0xF;
				
				Uint32 period=(note_w>>16)&0xFFF;
				
				if (period>0 && period<0xFFF) {
					
					//period>>=2;
					//period<<=1;
					for (int n=0; n<6*12; n++) {
						
						if (period >= period_table[n]) {
							
							if ((period!=period_table[n]) && (n))
							{
								Uint32 p1 = period_table[n-1];
								Uint32 p2 = period_table[n];
								if (p1 - period < (period - p2)) {
									
									note.note=n+36;
									break;
								}
							}
							note.note=n+1+36;
							break;
						}
					}
					if (note.note==Note::EMPTY)
						note.note=6*12+36;
					
					note.note--;
				} 	
				
				
				switch(cmd) {
					
					case 0x0: {
						
						if (note.parameter>0)
							note.command='J'-'A';
					} break;
					case 0x1: {
						note.command='F'-'A';
					} break;
					case 0x2: {
						
						note.command='E'-'A';
					} break;
					case 0x3: {
						
						note.command='G'-'A';
					} break;
					case 0x4: {
						
						note.command='H'-'A';
					} break;
					case 0x5: {
						note.command='L'-'A';
					} break;
					case 0x6: {
						
						note.command='K'-'A';
					} break;
					case 0x7: {
						note.command='R'-'A';
					} break;
					case 0x8: {
						
						note.command='X'-'A';
					} break;
					case 0x9: {
						
						note.command='O'-'A';
						
					} break;
					case 0xA: {
						
						note.command='D'-'A';
						
					} break;
					case 0xB: {
						
						note.command='B'-'A';
						
					} break;
					case 0xC: {
						
						note.volume=note.parameter;
						if (note.volume>64)
							note.volume=64;
						note.parameter=0;
						
					} break;
					case 0xD: {
						
						note.command='C'-'A';
						note.parameter=(note.parameter>>4)*10 + note.parameter&0xF;
						
					} break;
					case 0xE: { //SPECIAL EFFECT!
						
						note.command='S'-'A';
						
						switch(note.parameter>>4) {
							
							case 0x1: {
								
								note.command='F'-'A';
								note.parameter=0xF0|(note.parameter&0xF);
							} break;
							case 0x2: {
								
								note.command='E'-'A';
								note.parameter=0xF0|(note.parameter&0xF);
							} break;
							case 0x4: {
								
								note.command='S'-'A';
								note.parameter=0x30|(note.parameter&0x3);
								
							} break;
							case 0x6: {
								
								note.command='S'-'A';
								note.parameter=0xB0|(note.parameter&0xF);
								
							} break;
							case 0x7: {
								note.command='S'-'A';
								note.parameter=0x40|(note.parameter&0x3);
								
							} break;
							case 0x8: {
								
								note.command='S'-'A'; // wow, it's the same!
								
							} break;
							case 0x9: {
								note.command='Q'-'A';
								note.parameter=(note.parameter&0xF);
								
							} break;
							case 0xA: {
								
								note.command='D'-'A';
								note.parameter=0xF|((note.parameter&0xF)<<4);
								
							} break;
							case 0xB: {
								note.command='D'-'A';
								note.parameter=0xF0|(note.parameter&0xF);
								
							} break;
							case 0xC:
							case 0xD: {
								
								note.command='S'-'A'; //wow, they are the same!
														
							} break;
							case 0xE: {
								note.command='S'-'A';
								note.parameter=0x60|(note.parameter&0xF);
								
							} break;
							
							default: {
								
								note.command=Note::EMPTY;
								note.parameter=0;
							} break;
							
						}
					} break;
					case 0xF: {
						
						if (note.parameter<32)
							note.command='A'-'A';
						else
							note.command='T'-'A';
						
					} break;
				}
				
				p_song->get_pattern(i)->set_note( column,line, note );
			}
		}
	}
	
	
	
	for (int i=0;i<instruments;i++) {
		
		Sample_ID sid=p_song->get_sample(i)->get_sample_data();
		if (sid.is_null()) {
			continue; //empty sample, not stored?
		}
		
		int len=sm->get_size(sid);
		for (int s=0;s<len;s++) {
			
			Uint8 d=file->get_byte();
			//d-=128; //convert to signed
			Sint8*ds=(Sint8*)&d;
			Sint16 d16=*ds;
			d16<<=8;
			sm->set_data( sid, s, d16 );
			
		}
	}
	
	file->close();
	
	return FILE_OK;
	

}


Loader_MOD::Loader_MOD(FileAccessWrapper *p_file) {
	
	file=p_file;
}


Loader_MOD::~Loader_MOD()
{
}


