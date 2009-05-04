//
// C++ Interface: saver_wav
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAVER_WAV_H
#define SAVER_WAV_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

#include "fileio/saver.h"

class Saver_WAV : public Saver {

	FileAccessWrapper *file;
public:

	Error save_song(const char *p_filename,Song *p_song);
	Error save_sample(const char *p_filename,Sample *p_sample);
	Error save_instrument(const char *p_filename,Song *p_song,int p_instrument_index);
	
	Saver_WAV(FileAccessWrapper *p_file);
	
	~Saver_WAV();

};

#endif
