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
                          loader_it_info.cpp  -  description
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
#include "error_macros.h"




Loader::Error Loader_IT::load_header(bool p_dont_set) {

	
	char aux_songname[26];		
		
	file->get_byte_array((Uint8*)aux_songname,26);
	if (!p_dont_set)
		song->set_name( aux_songname );
	
	Uint8 aux_hlmin=file->get_byte();
	Uint8 aux_hlmaj=file->get_byte();

	if (aux_hlmin==0) aux_hlmin=4;
	if (aux_hlmaj==0) aux_hlmaj=16;

	if (!p_dont_set) {
		song->set_row_highlight_minor( aux_hlmin );
		song->set_row_highlight_major( aux_hlmaj );
	}

	header.ordnum=file->get_word();
	header.insnum=file->get_word();
	header.smpnum=file->get_word();
	header.patnum=file->get_word();
	
	header.cwt=file->get_word();		/* Created with tracker (y.xx = 0x0yxx) */
	header.cmwt=file->get_word();		/* Compatible with tracker ver > than val. */
	header.flags=file->get_word();
	
	if (!p_dont_set) {
		song->set_stereo( header.flags & 1 );
		song->set_linear_slides( header.flags & 8 );
		song->set_old_effects( header.flags & 16 );
		song->set_compatible_gxx( header.flags & 32 );
		song->set_instruments( header.flags & 4 );
	}
	
	
	header.special=file->get_word();
	if (!p_dont_set) {

		song->set_global_volume( file->get_byte() );
		song->set_mixing_volume( file->get_byte() );
		song->set_speed( file->get_byte() );
		song->set_tempo( file->get_byte() );
		song->set_stereo_separation( file->get_byte() );

	} else {

		file->get_byte(); // skip
		file->get_byte(); // skip
		file->get_byte(); // skip
		file->get_byte(); // skip
		file->get_byte(); // skip
	}
	file->get_byte(); // ZERO Byte
	header.msglength=file->get_word();
	header.msgoffset=file->get_dword();
	char chibi[4];
	file->get_byte_array((Uint8*)chibi,4);
	header.is_chibi=(chibi[0]=='C' && chibi[1]=='H' && chibi[2]=='B' && chibi[3]=='I');
	
	for (int i=0;i<64;i++) {
		
		Uint8 panbyte=file->get_byte();
		
		Uint8 pan_dst=(panbyte<65) ? panbyte : 32;
		bool surround_dst=(panbyte==100);
		bool mute_dst=(panbyte>=128);

		if (!p_dont_set) {
			song->set_channel_pan( i, pan_dst );
			song->set_channel_surround( i, surround_dst );
			song->set_channel_mute( i, mute_dst );
		}
	}
	for (int i=0;i<64;i++) {
		unsigned char cv = file->get_byte();
		if (!p_dont_set)
			song->set_channel_volume( i, file->get_byte() );
	}

	ERR_FAIL_COND_V( file->eof_reached(),FILE_CORRUPTED );
	ERR_FAIL_COND_V( file->get_error(),FILE_CORRUPTED );

	return FILE_OK;
}

Loader::Error Loader_IT::load_effects() {
	
	if (!header.is_chibi)
		return FILE_OK; //no effects, regular IT file
	
	/* GOTO End of IT header */
	file->seek(0xC0+header.ordnum+header.insnum*4+header.smpnum*4+header.patnum*4);
	
	
	if (file->get_byte()>0) //not made with this version, ignore extended info
		return FILE_OK;
	
	/* Chibitracker Extended info */

	switch(file->get_byte()) {
		
		case Song::REVERB_MODE_ROOM: {
			
			song->set_reverb_mode( Song::REVERB_MODE_ROOM );
		} break;
		case Song::REVERB_MODE_STUDIO_SMALL: {
			
			song->set_reverb_mode( Song::REVERB_MODE_STUDIO_SMALL );
			
		} break;
		case Song::REVERB_MODE_STUDIO_MEDIUM: {
			
			song->set_reverb_mode( Song::REVERB_MODE_STUDIO_MEDIUM );
			
		} break;
		case Song::REVERB_MODE_STUDIO_LARGE: {
			
			song->set_reverb_mode( Song::REVERB_MODE_STUDIO_LARGE );
			
		} break;
		case Song::REVERB_MODE_HALL: {
			
			song->set_reverb_mode( Song::REVERB_MODE_HALL );
			
		} break;
		case Song::REVERB_MODE_SPACE_ECHO: {
			
			song->set_reverb_mode( Song::REVERB_MODE_SPACE_ECHO );
			
		} break;

		case Song::REVERB_MODE_ECHO: {
			
			song->set_reverb_mode( Song::REVERB_MODE_ECHO );
			
		} break;
		case Song::REVERB_MODE_DELAY: {
			
			song->set_reverb_mode( Song::REVERB_MODE_DELAY );
			
		} break;
		case Song::REVERB_MODE_HALF_ECHO: {
			
			song->set_reverb_mode( Song::REVERB_MODE_HALF_ECHO );
			
		} break;
	
	}
		
	//chorus
	song->set_chorus_speed_hz10( file->get_byte() );
	song->set_chorus_delay_ms( file->get_byte() );
	song->set_chorus_depth_ms10( file->get_byte() );
	song->set_chorus_separation_ms( file->get_byte() );
	
	for (int i=0;i<Pattern::WIDTH;i++) {
		song->set_channel_reverb(i,file->get_byte());
	}
	for (int i=0;i<Pattern::WIDTH;i++) {
		song->set_channel_chorus(i,file->get_byte());
	}
	
	return FILE_OK;
	
}

Loader::Error Loader_IT::load_message() {

	
	if (!(header.special & 1)) {

		return FILE_OK;
	}		


	file->seek(header.msgoffset);

	//(void*)tmpmsg=malloc(header.msglength+1);
	MemPool_Handle m = MemPool_Wrapper::get_singleton()->alloc_mem( header.msglength+1 );

	if (m.is_null())
		return FILE_OUT_OF_MEMORY;

	MemPool_Wrapper::get_singleton()->lock_mem( m );

	
	char *tmpmsg = (char*)MemPool_Wrapper::get_singleton()->get_mem( m );

	file->get_byte_array((Uint8*)tmpmsg,header.msglength);
	tmpmsg[header.msglength]=0;
	
	for (int i=0;i<header.msglength;i++) if (tmpmsg[i]=='\r') tmpmsg[i]='\n';

	song->set_message(tmpmsg);

	MemPool_Wrapper::get_singleton()->unlock_mem( m );
	MemPool_Wrapper::get_singleton()->free_mem( m );
	
	return FILE_OK;
}

Loader::Error Loader_IT::load_orders() {

	file->seek(0xC0);
	
	
	for (int i=0;i<header.ordnum;i++) {
		
		Uint8 aux_order=file->get_byte();
		Order order=ORDER_NONE;
		
		
		if (i>=Song::MAX_ORDERS)
			continue;
		if (aux_order==254)  {

			order=ORDER_BREAK;

		} else if (aux_order<200) {

			order=aux_order;
			//nothing!

		} 
		song->set_order(i,order);
		
	}
	
	if (file->eof_reached() || file->get_error()) {


		return FILE_CORRUPTED;

	}
	
	return FILE_OK;
}



