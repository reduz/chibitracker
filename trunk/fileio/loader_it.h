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
                          loader_it.h  -  description
                             -------------------
    begin                : Sun Jan 21 2001
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

#ifndef LOADER_IT_H
#define LOADER_IT_H

#include "fileio/loader.h"
#include "mempool_wrapper.h"
/**
  *@author Juan Linietsky
  */

/******************************
 loader_it.h
 ----------
Impulse Tracker Module Loader!
It lacks support for old
instrument files methinks...
and some other things like
midi.
********************************/

class AuxSampleData; //used for internal crap

class Loader_IT : public Loader  {



	FileAccessWrapper *file;
	Song *song;

	struct IT_Header {
		Uint8	blank01[2];
		Uint16	ordnum;
		Uint16	insnum;
		Uint16	smpnum;
		Uint16	patnum;
		Uint16	cwt;		/* Created with tracker (y.xx = 0x0yxx) */
		Uint16	cmwt;		/* Compatible with tracker ver > than val. */
		Uint16	flags;
		Uint16	special;	/* bit 0 set = song message attached */
		Uint16	msglength;
		Uint32	msgoffset;
		bool is_chibi;
	};	

	/* Variables to store temp data */
	IT_Header header;
	
	/* Song Info Methods */
	Error load_header(bool p_dont_set);
	Error load_orders();
	Error load_message();

	/* Pattern Methods */
	Error load_patterns();
	
	/* Sample Methods */
	
	Error load_samples();
	Error load_sample(Sample *p_sample);
	Sample_ID load_sample_data(AuxSampleData& p_sample_data);

	// Sample decompression 
	
	Uint32 read_n_bits_from_IT_compressed_block(Uint8 p_bits_to_read);
	bool read_IT_compressed_block (bool p_16bits);
	void free_IT_compressed_block ();
	bool load_sample_8bits_IT_compressed(void *p_dest_buffer,int p_buffsize);
	bool load_sample_16bits_IT_compressed(void *p_dest_buffer,int p_buffsize);
	Uint32 *source_buffer; 	/* source buffer */
	Uint32 *source_position;  /* actual reading position */
	Uint8 source_remaining_bits; /* bits remaining in read dword */
	MemPool_Handle compressed_loader_memhandle;
	
        /* Instruments Methods */
	Error load_effects();
	Error load_instruments();
	Error load_instrument(Instrument *p_instrument,int *p_samples=0);
	void load_envelope(Envelope *p_envelope,bool*p_has_filter_flag=0);


public:

	
	bool can_load_song();
	bool can_load_sample();
	bool can_load_instrument();
	
	Error load_song(const char *p_file,Song *p_song, bool p_sampleset=false);
	Error load_sample(const char *p_file,Sample *p_sample);
	Error load_instrument(const char *p_file,Song *p_song,int p_instr_idx);

	Loader_IT(FileAccessWrapper *p_file);
	
};



#endif
