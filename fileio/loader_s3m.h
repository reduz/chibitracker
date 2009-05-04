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



/***************************************************************************
                          loader_s3m.h  -  description
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

#ifndef LOADER_S3M_H
#define LOADER_S3M_H

#include "fileio/loader.h"

/**
  *@author Juan Linietsky
  */
/******************************
 loader_s3m.h
 ----------
Scream Tracker Module Loader!
It lacks support for
individual sample loading
and reorganizing the columns.
********************************/




class Loader_S3M : public Loader  {

	struct S3M_Header {
	        char  songname[28];
	        Uint8 t1a;
	        Uint8 type;
	        Uint8 unused1[2];
	        Uint16 ordnum;
	        Uint16 insnum;
	        Uint16 patnum;
	        Uint16 flags;
	        Uint16 tracker;
	        Uint16 fileformat;
	        char  scrm[5];
	        Uint8 mastervol;
	        Uint8 initspeed;
	        Uint8 inittempo;
	        Uint8 mastermult;
	        Uint8 ultraclick;
	        Uint8 pantable;
	        Uint8 unused2[8];
	        Uint16 special;
	        Uint8 channels[32];
		Uint8 pannings[32];
		Uint8 orderlist[300];
	};

	
	int sample_parapointers[Song::MAX_SAMPLES];
	int pattern_parapointers[Song::MAX_PATTERNS];
	
	Error load_header();
	void set_header();
	Error load_sample(Sample *p_sample);
	Error load_pattern(Pattern *p_pattern);
	Error load_patterns();

	Error load_samples();
	
	S3M_Header header;
        int sample_count;
	int pattern_count;
	
	FileAccessWrapper *file;
	Song *song;
public:

	bool can_load_song() { return true; }
	bool can_load_sample() { return false; }
	bool can_load_instrument() { return false; }
	
	Error load_song(const char *p_file,Song *p_song);
	Error load_sample(const char *p_file,Sample *p_sample);
	Error load_instrument(const char *p_file,Song *p_song,int p_instr_idx);
	
	Loader_S3M(FileAccessWrapper *p_file);
	~Loader_S3M();
};



#endif
