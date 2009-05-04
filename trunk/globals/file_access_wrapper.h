//
// C++ Interface: file_access_wrapper
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILE_ACCESS_WRAPPER_H
#define FILE_ACCESS_WRAPPER_H

#include "config.h"

class FileAccessWrapper {
public:

	enum ModeFlags  {

		READ=1,
		WRITE=2,
		READ_WRITE=3,
	};
	
	enum Error {

		OK,
		ERROR_FILE_NOT_FOUND,
		ERROR_FILE_BAD_DRIVE,
		ERROR_FILE_BAD_PATH,
		ERROR_FILE_NO_PERMISSION,
		ERROR_ALREADY_IN_USE,
		ERROR_INVALID_PARAMETERS,
		ERROR_OPENING_FILE,
		ERROR_READING_FILE,
		ERROR_WRITING_FILE
	};

	virtual Error open(const char *p_filename, int p_mode_flags)=0;
	virtual void close()=0;
	
	virtual void seek(Uint32 p_position)=0;
	virtual void seek_end()=0;
	virtual Uint32 get_pos()=0;

	virtual bool eof_reached()=0;

	virtual Uint8 get_byte()=0;
	virtual void get_byte_array(Uint8 *p_dest,int p_elements)=0;
	virtual void get_word_array(Uint16 *p_dest,int p_elements)=0;

	virtual Uint16 get_word()=0;
	virtual Uint32 get_dword()=0;

	// use this for files WRITTEN in _big_ endian machines (ie, amiga/mac)
	// It's not about the current CPU type but file formats.
	// this flags get reset to false (little endian) on each open
	virtual void set_endian_conversion(bool p_swap)=0;
	virtual bool is_open()=0;

	virtual Error get_error()=0;

	virtual void store_byte(Uint8 p_dest)=0;
	virtual void store_byte_array(const Uint8 *p_dest,int p_elements)=0;

	virtual void store_word(Uint16 p_dest)=0;
	virtual void store_dword(Uint32 p_dest)=0;

	static FileAccessWrapper*(*create)();
	
	virtual ~FileAccessWrapper(){}

};



#endif
