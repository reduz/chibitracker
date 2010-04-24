//
// C++ Interface: loader_wav
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOADER_WAV_H
#define LOADER_WAV_H


#include "fileio/loader.h"
#include "file_access_wrapper.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Loader_WAV : public Loader{

	FileAccessWrapper *file;
public:

	virtual bool can_load_song() { return false; };
	virtual bool can_load_sample() { return true; };
	virtual bool can_load_instrument() { return false; };
	
	virtual Error load_song(const char *p_file,Song *p_song,bool p_sampleset) { return FILE_UNRECOGNIZED; }
	virtual Error load_sample(const char *p_file,Sample *p_sample);
	virtual Error load_instrument(const char *p_file,Song *p_song,int p_instr_idx) { return FILE_UNRECOGNIZED; }
	
	Loader_WAV(FileAccessWrapper *p_file);
	~Loader_WAV();

};

#endif
