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
#ifndef FILE_ACCESS_H
#define FILE_ACCESS_H

#ifdef ANSIC_LIBS_ENABLED

#include <stdio.h>
//#include <unistd.h>		// ftruncate
#include "config.h"
#include "file_access_wrapper.h"

class FileAccess : public FileAccessWrapper {
	FILE *f;

	bool in_use;
	bool error_ocurred;
	int error_type;

	bool perform_endian_swap;

	void check_for_errors();

	template<class T>
	void endian_swap(T &a, T &b) {

		if  (perform_endian_swap) {
			T c;			
			c=a; a=b; b=c;
		}
	}

	Error current_error;
	
	int current_mode;

	char *current_filename;
	static FileAccessWrapper *create_default();
public:

	static bool exists(const char *p_filename);
	Error open(const char *p_filename, int p_mode_flags);
	void close();
	void seek(Uint32 p_position);
	void seek_end();
	Uint32 get_pos();

	bool eof_reached();

	Uint8 get_byte();
	void get_byte_array(Uint8 *p_dest,int p_elements);
	void get_word_array(Uint16 *p_dest,int p_elements);

	Uint16 get_word();
	Uint32 get_dword();


	// use this for files WRITTEN in _big_ endian machines (ie, amiga/mac)
	// It's not about the current CPU type but file formats.
	void set_endian_conversion(bool p_swap) { perform_endian_swap=p_swap; }
	bool is_open() { return in_use; };

	Error get_error();

	void store_byte(Uint8 p_dest);
	void store_byte_array(const Uint8 *p_dest,int p_elements);

	void store_word(Uint16 p_dest);
	void store_dword(Uint32 p_dest);

	void truncate_file(const char *p_filename, Uint32 p_length);


	static void set_as_default();
	FileAccess();
 	~FileAccess();

};

#endif

#endif


