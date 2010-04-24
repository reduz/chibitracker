//
// C++ Interface: loader
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOADER_H
#define LOADER_H


#include "file_access_wrapper.h"
#include "song/song.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Loader {

public:
	
	enum Error {
		FILE_OK,
		FILE_UNRECOGNIZED,
		FILE_CANNOT_OPEN,
		FILE_CORRUPTED,
		FILE_OUT_OF_MEMORY,
	};
	
	
	virtual bool can_load_song()=0;
	virtual bool can_load_sample()=0;
	virtual bool can_load_instrument()=0;
	
	virtual Error load_song(const char *p_file,Song *p_song,bool p_sampleset)=0;
	virtual Error load_sample(const char *p_file,Sample *p_sample)=0;
	virtual Error load_instrument(const char *p_file,Song *p_song,int p_instr_idx)=0;
	
	
	virtual ~Loader() {}

};

#endif
