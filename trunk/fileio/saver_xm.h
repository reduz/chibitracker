//
// C++ Interface: saver_xm
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAVER_XM_H
#define SAVER_XM_H

#include "fileio/saver.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Saver_XM : public Saver {

	FileAccessWrapper *f;
public:

	virtual Error save_song(const char *p_filename,Song *p_song);
	virtual Error save_sample(const char *p_filename,Sample *p_sample);
	virtual Error save_instrument(const char *p_filename,Song *p_song,int p_instrument_index);

	Saver_XM(FileAccessWrapper *p_file);
	~Saver_XM();

};

#endif
