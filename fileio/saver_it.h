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
                          saver_it.h  -  description
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

#ifndef SAVER_IT_H
#define SAVER_IT_H

#include "fileio/saver.h"

/**
  *@author Juan Linietsky
  */
/******************************
 saver_it.h
 --------

Impulse Tracker Module Saving..
it seems to work, but modules
saved with this crash xmp...
********************************/





class Saver_IT : public Saver {

	Song *song;

	FileAccessWrapper *file;

	Uint32 sample_offsets[Song::MAX_SAMPLES];
	Uint32 pattern_offsets[Song::MAX_PATTERNS];
	Uint32 instrument_offsets[Song::MAX_PATTERNS];

	int sample_count;
	int pattern_count;
	int instrument_count;
	int orders_count;
        int message_length;

	int message_offset;

	void calculate_values();
	void write_header();
	void write_sample(Sample *p_sample);
	void write_sample_internal(Sample *p_sample,bool p_write_data);
	void write_instrument(int p_instrument_index);
 	void write_instrument_internal(Instrument *p_aux_instr);
	void write_sample_data(Sample_ID p_data);
	
	void write_orders();
	void write_message();
        void write_pattern(int p_pattern);
        void write_song();
public:

	virtual Error save_song(const char *p_filename,Song *p_song);
	virtual Error save_sample(const char *p_filename,Sample *p_sample);
	virtual Error save_instrument(const char *p_filename,Song *p_song,int p_instrument_index);

	Saver_IT(FileAccessWrapper *p_file);
	~Saver_IT();
};



#endif
