//
// C++ Interface: loader_mod
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOADER_MOD_H
#define LOADER_MOD_H
#include "fileio/loader.h"
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class Loader_MOD : public Loader {

	FileAccessWrapper *file;
public:

	bool can_load_song() { return true; }
	bool can_load_sample() { return false; }
	bool can_load_instrument() { return false; }
	
	Error load_song(const char *p_file,Song *p_song,bool p_sampleset);
	Error load_sample(const char *p_file,Sample *p_sample) { return FILE_UNRECOGNIZED; }
	Error load_instrument(const char *p_file,Song *p_song,int p_instr_idx) { return FILE_UNRECOGNIZED; }
	
	Loader_MOD(FileAccessWrapper *p_file);
	~Loader_MOD();
};

#endif
