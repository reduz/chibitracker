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
                          saver.h  -  description
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

#ifndef SAVER_H
#define SAVER_H

#include "file_access_wrapper.h"
#include "song/song.h"

/**saves module file

  *@author Juan Linietsky
  */

/******************************
 saver.h
 --------

abstract class for sample saving
********************************/





class Saver {
public:

	enum Error {

		SAVE_OK,
		SAVE_ERROR = -1,
		UNKNOWN_EXTENSION = -2
	};

	virtual Error save_song(const char *p_filename,Song *p_song)=0;
	virtual Error save_sample(const char *p_filename,Sample *p_sample)=0;
	virtual Error save_instrument(const char *p_filename,Song *p_song,int p_instrument_index)=0;

	Saver();
	virtual ~Saver();
};



#endif
