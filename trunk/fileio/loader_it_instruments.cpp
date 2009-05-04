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
                          loader_it_instruments.cpp  -  description
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

#include <stdio.h>
enum EnvFlags {
	ENV_ON=1,
	ENV_LOOP=2,
	ENV_SUSLOOP=4,
	ENV_CARRY=8,
	ENV_FILTER=128
};

void Loader_IT::load_envelope(Envelope *p_envelope,bool*p_has_filter_flag) { 

	Uint8 flags=file->get_byte();
	Uint8 points=file->get_byte();
	Uint8 begin=file->get_byte();
	Uint8 end=file->get_byte();
	Uint8 susbegin=file->get_byte();
	Uint8 susend=file->get_byte();
	
	p_envelope->reset();
	
	for (int i=0;i<25;i++) {

		Uint8 height=file->get_byte();
		Sint8 &signed_height=(Sint8&)height;
		Uint16 tick=file->get_word();
		
		if (i>=points)
			continue;
		p_envelope->add_position( tick, signed_height );
	
	}

	p_envelope->set_enabled( flags & ENV_ON );
	p_envelope->set_carry_enabled( flags & ENV_CARRY);
	
	p_envelope->set_loop_enabled( flags & ENV_LOOP );
	p_envelope->set_loop_begin( begin );
	p_envelope->set_loop_end( end );
	
	p_envelope->set_sustain_loop_enabled( flags & ENV_SUSLOOP );
	p_envelope->set_sustain_loop_begin( susbegin );
	p_envelope->set_sustain_loop_end( susend );
	
	if (p_has_filter_flag)
		*p_has_filter_flag=flags&ENV_FILTER;
	
	file->get_byte(); //zerobyte
	
	//fill with stuff if the envelope hass less than 2 points
	while(p_envelope->get_node_count()<2) {
		
		p_envelope->add_position( 30*p_envelope->get_node_count(), p_envelope->get_min()==0 ? 64 : 0, false );
	}
}


Loader::Error Loader_IT::load_instrument(Instrument *p_instrument,int *p_samples) {



	char aux_header[4];
	
	file->get_byte_array((Uint8*)aux_header,4);	

	
	if (	aux_header[0]!='I' ||
		       aux_header[1]!='M' ||
		       aux_header[2]!='P' ||
		       aux_header[3]!='I') {
		ERR_PRINT("IT Loader Instrument: Failed Identifier");
	
		return FILE_UNRECOGNIZED;
	}
		
	

	// Ignore deprecated 8.3 filename field
	for (int i=0;i<12;i++) file->get_byte();
	
	//Ignore zerobyte
	file->get_byte();		/* (byte) Instrument type (always 0) */
	
	switch( file->get_byte() ) { /* New Note Action [0,1,2,3] */
		case 0: p_instrument->set_NNA_type( Instrument::NNA_NOTE_CUT ) ; break;
		case 1: p_instrument->set_NNA_type( Instrument::NNA_NOTE_CONTINUE ) ; break;
		case 2: p_instrument->set_NNA_type( Instrument::NNA_NOTE_OFF ) ; break;
		case 3: p_instrument->set_NNA_type( Instrument::NNA_NOTE_FADE ) ; break;
	};
	switch( file->get_byte() ) { // Duplicate Check Type
		case 0: p_instrument->set_DC_type( Instrument::DCT_DISABLED ); break ;		
		case 1: p_instrument->set_DC_type( Instrument::DCT_NOTE ); break ;		
		case 2: p_instrument->set_DC_type( Instrument::DCT_SAMPLE ); break ;		
		case 3: p_instrument->set_DC_type( Instrument::DCT_INSTRUMENT ); break ;		
	}
	switch( file->get_byte() ) { //Duplicate Check Action
		case 0: p_instrument->set_DC_action( Instrument::DCA_NOTE_CUT ); break ;
		case 1: p_instrument->set_DC_action( Instrument::DCA_NOTE_OFF ); break ;
		case 2: p_instrument->set_DC_action( Instrument::DCA_NOTE_FADE ); break ;
	}
	
	int fade = file->get_word();
	//intf("AFADE: %i\n",fade);
	if (fade>Instrument::MAX_FADEOUT) //needs to be clipped because of horrible modplug doings
		fade=Instrument::MAX_FADEOUT;
	
	p_instrument->set_volume_fadeout( fade );
	p_instrument->set_pan_pitch_separation( file->get_byte() );
	p_instrument->set_pan_pitch_center( file->get_byte() );
	p_instrument->set_volume_global_amount( file->get_byte() );
	Uint8 pan=file->get_byte();
	p_instrument->set_pan_default_amount(pan&0x7F);
	p_instrument->set_pan_default_enabled( !(pan&0x80) );
	p_instrument->set_volume_random_variation( file->get_byte() );
	p_instrument->set_pan_random_variation( file->get_byte() );
	
	
	
	file->get_word(); //empty (version)
	Uint8 samples=file->get_byte();
	if (p_samples)
		*p_samples=samples;
	file->get_byte(); //empty
	char aux_name[26];	
	file->get_byte_array((Uint8*)aux_name,26);
	p_instrument->set_name(aux_name);
	
	Uint8 cutoff=file->get_byte();
		
	p_instrument->set_filter_default_cutoff(cutoff&0x7F);
	p_instrument->set_filter_use_default_cutoff(cutoff&0x80); 
	
	Uint8 resonance=file->get_byte();
		
	p_instrument->set_filter_default_resonance(resonance&0x7F);
	p_instrument->set_filter_use_default_resonance(resonance&0x80); 
	
	file->get_dword(); //MIDI, IGNORED!
	
	/* Note -> Sample table */
	for (Uint8 i=0;i<Note::NOTES;i++) {
		
		
		Uint8 note=file->get_byte();
		if (note>=Note::NOTES)
			note=0;
		p_instrument->set_note_number(i,note);		
		
		Uint8 samp=file->get_byte();
		if (samp==0 || samp>99)
			samp=Note::EMPTY;
		else 
			samp--;
		
		
		p_instrument->set_sample_number(i,samp);
	

	}

	
	load_envelope( p_instrument->get_volume_envelope() );
	load_envelope( p_instrument->get_pan_envelope() );
	bool use_as_filter;
	load_envelope( p_instrument->get_pitch_filter_envelope(), &use_as_filter );
	p_instrument->set_pitch_use_as_filter( use_as_filter );

	return FILE_OK;

}


Loader::Error Loader_IT::load_instruments() {


	for (int i=0;i<header.insnum;i++) {

		
		file->seek(0xC0+header.ordnum+i*4);
		Uint32 final_location=file->get_dword();
		file->seek( final_location );
		
		Error err=load_instrument( song->get_instrument( i ) );
		if (err)
			return err;

	}

	return FILE_OK;

	if (file->eof_reached() || file->get_error())
		return FILE_CORRUPTED;
}


