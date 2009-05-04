/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : coding@reduz.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


/***********p****************************************************************
                          loader_s3m.cpp  -  description
                             -------------------
    begin                : Tue Apr 24 2001
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

#include "loader_s3m.h"
#include "error_macros.h"

#define BITBOOL(m_exp) ((m_exp)?1:0)
#include <stdio.h>


Loader::Error Loader_S3M::load_header() {

	int i;

	
        file->get_byte_array((Uint8*)header.songname,28);
        header.t1a=file->get_byte();
        header.type=file->get_byte();
        file->get_byte_array((Uint8*)header.unused1,2);
        header.ordnum=file->get_word();
        header.insnum=file->get_word();
        header.patnum=file->get_word();
        header.flags=file->get_word();
        header.tracker=file->get_word();
        header.fileformat=file->get_word();
        file->get_byte_array((Uint8*)header.scrm,4);
        header.scrm[4]=0;
	
	if (header.scrm[0]!='S' || header.scrm[1]!='C' || header.scrm[2]!='R' || header.scrm[3]!='M')
		return FILE_UNRECOGNIZED;

        header.mastervol=file->get_byte();
        header.initspeed=file->get_byte();
        header.inittempo=file->get_byte();
        header.mastermult=file->get_byte();
        header.ultraclick=file->get_byte();
        header.pantable=file->get_byte();
        file->get_byte_array((Uint8*)header.unused2,8);
        header.special=file->get_word();
        file->get_byte_array((Uint8*)header.channels,32);

        file->get_byte_array((Uint8*)header.orderlist,header.ordnum);

        header.scrm[4]=0;
	if (header.scrm[0]!='S' || header.scrm[1]!='C' || header.scrm[2]!='R' || header.scrm[3]!='M') //again?
		return FILE_UNRECOGNIZED;
	//sample parapointers
	for (i=0;i<header.insnum;i++) {

		int parapointer;
		parapointer=file->get_word();
		parapointer=(parapointer*16);
		sample_parapointers[i]=parapointer;
	}
	//pattern
	for (i=0;i<header.patnum;i++) {

		int parapointer;
		parapointer=file->get_word();
		parapointer=(parapointer*16);
		pattern_parapointers[i]=parapointer;
	}

        if (header.pantable==252) {

	        file->get_byte_array((Uint8*)header.pannings,32);
	}

	return FILE_OK;


}


void Loader_S3M::set_header() {
	
	


	song->set_name( header.songname );
//	song->variables.filename=
	
	song->set_row_highlight_minor( 4 );
	song->set_row_highlight_major( 16 );
	song->set_mixing_volume( header.mastervol );
	song->set_linear_slides( false );
	song->set_old_effects( !(header.flags&64) );
	song->set_compatible_gxx( true );

	song->set_global_volume( header.mastermult );
	song->set_speed( header.initspeed );
	song->set_tempo( header.inittempo );

	//[TODO] Set Panning Positions.. ?

	for (int i=0;i<header.ordnum;i++) song->set_order(i,header.orderlist[i]);
	
}

Loader::Error  Loader_S3M::load_sample(Sample *p_sample) {



	        int type=file->get_byte();
		
		char filename[13];
                file->get_byte_array((Uint8*)filename,12);
		filename[12]=0;
		
		
		Uint32 samplepos=(Uint32)file->get_byte() << 16;
                samplepos|=file->get_word();
		samplepos*=16;	
//		printf("sample at %i\n",samplepos);
		/**/
                int sample_size=file->get_dword();
		
	
		int loop_begin=file->get_dword();
		int loop_end=file->get_dword();

		int def_volume=file->get_byte();;
		int dsk=file->get_byte();
		int pack=file->get_byte();
		
		int flags=file->get_byte();
		int c2speed=file->get_dword();
		
		file->get_dword(); //useless crap
		file->get_dword();
		file->get_dword();
		
		
		char name[29];
                file->get_byte_array((Uint8*)name,28);
		name[28]=0;
		
		p_sample->set_default_volume(def_volume);
		p_sample->set_name(name);
		
		char scrs[4];
		file->get_byte_array((Uint8*)scrs,4);
		scrs[4]=0;

		

                bool data_is_16bits=flags&4;
		bool data_is_stereo=flags&2;

		if (type==0) {
			//empty sample
			return FILE_OK;
		}
			

		if ((type!=1) || scrs[0]!='S' || scrs[1]!='C' || scrs[2]!='R' || scrs[3]!='S' ) {
//			printf("type: %i, %c%c%c%c\n",type,scrs[0],scrs[1],scrs[2],scrs[3]);
			ERR_PRINT("Not an S3M Sample!");
			return FILE_CORRUPTED;
		}

		//p_sample->data.set_c5_freq(p_sample->c2spd<<1);

		file->seek(samplepos);

		int real_sample_size=sample_size<<BITBOOL(data_is_16bits);
		real_sample_size<<=BITBOOL(data_is_stereo);

		SampleManager *sm=SampleManager::get_singleton();
		
		Sample_ID id =sm->create( data_is_16bits, data_is_stereo, sample_size );
		
		if (id.is_null())
			return FILE_OUT_OF_MEMORY;
		
		for (int c=0;c<(data_is_stereo?2:1);c++) {
			for (int i=0;i<sample_size;i++) {
				
				if (data_is_16bits) {
						
					Uint16 s=file->get_word();
					s-=32768; //toggle sign
					
					Sint16 *v=(Sint16*)&s;
					sm->set_data(id,i,*v,c);
				} else {
						
						
					Sint8 *v;
					Uint8 s=file->get_byte();
					s-=128; //toggle sign
					v=(Sint8*)&s;
					Sint16 v16=*v;
					v16<<=8;
					sm->set_data(id,i,v16,c);
						
				}
				
				
			}
			
		}
						  
		sm->set_loop_begin( id, loop_begin );
		sm->set_loop_end( id, loop_end );
		sm->set_loop_type( id, (flags&1) ? LOOP_FORWARD : LOOP_NONE );
		sm->set_c5_freq( id, c2speed << 1 );
		p_sample->set_sample_data(id);
		
		/* Scream tracker previous to 3.10 seems to be buggy, as in, wont save what is after the sample loop, including the loop end point. Because of this I must fix it by habd */
		if (flags&1) {
			
			for (int c=0;c<(data_is_stereo?2:1);c++) {
				sm->set_data( id, loop_end, sm->get_data( id, loop_begin,c ),c );
				
			}
		}
				  

		return FILE_OK;

}


Loader::Error Loader_S3M::load_pattern(Pattern *p_pattern) {

        int row=0,flag,ch;
	Note n;
	int length,accum=0;

	length=file->get_word();
        p_pattern->set_length(64);

        /* clear pattern data */
        while((row<64) && (accum<=length) ) {
                flag=file->get_byte();
		accum++;

		n.clear();
                if(flag) {
                       // ch=remap[flag&31];
//                        ch=remap[flag&31];
//                        if(ch!=-1)
//                                n=s3mbuf[(64U*ch)+row];
//                        else
//                                n=&dummy;

			ch=flag&31;

                        if(flag&32) {
                                n.note=file->get_byte();
				if (n.note==255) {

					n.note=Note::EMPTY;
				} else if (n.note==254) {

					n.note=Note::CUT;
				} else {
				
                                	n.note=((n.note>>4)*12)+(n.note&0xF);
				}

                                n.instrument=file->get_byte()-1;
				accum+=2;

                        }
                        if(flag&64) {
                                n.volume=file->get_byte();
                                if (n.volume>64) n.volume=64;
				accum++;

                        }
                        if(flag&128) {
                                n.command=file->get_byte()-1;
                                n.parameter=file->get_byte();
				accum+=2;
                        }
		
			p_pattern->set_note(ch,row,n);
                } else row++;
        }
        return FILE_OK;


}

Loader::Error Loader_S3M::load_sample(const char *p_file,Sample *p_sample) {
	
	return FILE_UNRECOGNIZED;
}
Loader::Error Loader_S3M::load_instrument(const char *p_file,Song *p_song,int p_instr_idx) {
	
	return FILE_UNRECOGNIZED;
	
}


Loader::Error Loader_S3M::load_samples() {

	int i;

	for(i=0;i<header.insnum;i++) {

		file->seek(sample_parapointers[i]);
		load_sample(song->get_sample(i));
		sample_count++;
	}

	return FILE_OK;
}

Loader::Error Loader_S3M::load_patterns() {

	int i;

	Error err;
	for(i=0;i<header.patnum;i++) {

		file->seek(pattern_parapointers[i]);
		
		err=load_pattern(song->get_pattern(i) );
		ERR_FAIL_COND_V(err,err);

		
		pattern_count++;
	}
	return FILE_OK;

}

Loader::Error Loader_S3M::load_song(const char *p_file,Song *p_song) {

	song=p_song;

	if (file->open(p_file,FileAccessWrapper::READ)) {
		//printf("Can't open file! %s\n",p_file);
		return FILE_CANNOT_OPEN;
	};
	
        sample_count=0;
	pattern_count=0;

	//printf("LOADING HEADER\n");
	Loader::Error err;
	if ((err=load_header())) {
		file->close();
		ERR_FAIL_COND_V(err,err);
		
	}		

	song->reset(); //file type recognized, reset song!
	
	set_header();
	
	//printf("LOADING SAMPLES\n");
	
	if ((err=load_samples())) {
		file->close();

		ERR_FAIL_COND_V(err,err);
	}		

	//printf("LOADING PATTERNS\n");
	
	if (err=load_patterns()) {

		file->close();
		return err;
	}		

	file->close();

	return FILE_OK;
}



Loader_S3M::Loader_S3M(FileAccessWrapper *p_file){

	file=p_file;

}
Loader_S3M::~Loader_S3M(){
}

