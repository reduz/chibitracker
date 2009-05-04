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
                          loader_it_patterns.cpp  -  description
                             -------------------
    begin                : Wed Apr 11 2001
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
#include "loader_it.h"


Loader::Error Loader_IT::load_patterns() {


	for (int i=0;i<header.patnum;i++) {

		if (i>=Song::MAX_PATTERNS)
			break;
			
		/* Position where pattern offsets are stored */
		file->seek(0xC0+header.ordnum+header.insnum*4+header.smpnum*4+i*4);
		Uint32 pattern_offset=file->get_dword();
		
		if (pattern_offset==0) {

			continue;
		}
			
		Uint16 pat_size;
		Uint16 pat_length;

		int row=0,flag,channel,j;
		Uint8 aux_byte;
		Uint32 reserved;
		Uint8 chan_mask[64]; //mask cache for each
		Note last_value[64]; //last value of each

		for (j=0;j<64;j++) {

			chan_mask[j]=0;
			last_value[j].clear();
		}

		file->seek(pattern_offset);

		pat_size=file->get_word();
		pat_length=file->get_word();
		reserved=file->get_dword();

		song->get_pattern(i)->set_length( pat_length );
		
		do {

			aux_byte=file->get_byte();
			flag=aux_byte;

			if ( flag==0 ) {

				row++;
			} else {

				channel=(flag-1) & 63;

				if ( flag & 128 ) {

					aux_byte=file->get_byte();
					chan_mask[channel]=aux_byte;
				}

				Note note; //note used for reading

				if ( chan_mask[channel]&1 ) { // read note
			
					aux_byte=file->get_byte();
					
					if ( aux_byte<120 )
						note.note=aux_byte;
					else if ( aux_byte==255 ) 	
						note.note=Note::OFF;
					else if ( aux_byte==254 )
						note.note=Note::CUT;

					last_value[channel].note=note.note;
				}
					

				if ( chan_mask[channel]&2 ) {

					aux_byte=file->get_byte();
					if ( aux_byte<100 )
						note.instrument=aux_byte-1;

					last_value[channel].instrument=note.instrument;
				}
				if ( chan_mask[channel]&4 ) {

					aux_byte=file->get_byte();
					if ( aux_byte<213 )
						note.volume=aux_byte;

					last_value[channel].volume=note.volume;
				}
				if ( chan_mask[channel]&8 ) {

					aux_byte=file->get_byte();
					if ( aux_byte>0 ) 
						note.command=aux_byte-1;
					
					
					last_value[channel].command=note.command;

					note.parameter=file->get_byte();
					
					last_value[channel].parameter=note.parameter;
				}

				if ( chan_mask[channel]&16 ) {

					note.note=last_value[channel].note;
				}

				if ( chan_mask[channel]&32 ) {

					note.instrument=last_value[channel].instrument;
				}
				if ( chan_mask[channel]&64 ) {

					note.volume=last_value[channel].volume;
				}
				if ( chan_mask[channel]&128 ) {

					note.command=last_value[channel].command;
					note.parameter=last_value[channel].parameter;
				}
				
				song->get_pattern(i)->set_note(channel,row,note);
			}
			
			
		} while(row<pat_length);

	}

	return FILE_OK;
}

