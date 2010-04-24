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
                          file_format_manager.h  -  description
                             -------------------
    begin                : Thu Apr 12 2001
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

#ifndef FILE_FORMAT_MANAGER_H
#define FILE_FORMAT_MANAGER_H

#include "fileio/loader.h"
#include "fileio/saver.h"

/**Collection of loaders/savers/etc
  *@author Juan Linietsky
  */

/******************************
 file_format_manager.h
 ----------

Abstracts file access, so it can handle
multiple file formats for one kind
of operation.
********************************/




class FileFormatManager {


	enum {

		MAX_LOADERS=10,
		MAX_SAVERS=10
	};

	
	Loader *loaders[MAX_LOADERS];
	int loader_count;
	Saver *savers[MAX_SAVERS];
	int saver_count;
public:

	void register_loader(Loader *p_loader); ///< Register a loader, will get deleted on destructor
	Loader::Error load_song(const char *p_file,Song *p_song,bool p_sampleset=false);
	Loader::Error load_sample(const char *p_file,Sample* p_sample);
	Loader::Error load_instrument(const char *p_file,Song *p_song, int p_instrument_idx);


	void register_saver(Saver *p_saver); ///< Register a saver, will get deleted on destructor
	Saver::Error save_song(const char *p_file,Song *p_song);
	Saver::Error save_sample(const char *p_file, Sample *p_sample);
	Saver::Error save_instrument(const char *p_file, Song *p_song, int p_instrument_idx);

	FileFormatManager();
	~FileFormatManager();
};



#endif
