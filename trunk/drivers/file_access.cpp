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
#include "file_access.h"
#include "error_macros.h"

#ifdef ANSIC_LIBS_ENABLED

FileAccessWrapper *FileAccess::create_default() {

	return new FileAccess;
}
void FileAccess::set_as_default() {

	FileAccessWrapper::create=&FileAccess::create_default;
}

FileAccess::FileAccess() {

	f=0;
	in_use=false;
	perform_endian_swap=false;
	error_ocurred=false;
	current_error=OK;
	current_mode=0;
}

FileAccess::~FileAccess() {
}

bool FileAccess::exists(const char *p_filename) {

	FILE *g;
	//printf("opening file %s\n", p_fname.c_str());
	g=fopen(p_filename,"rb");
	if (g==NULL) {

		return false;
	} else {

		fclose(g);
		return true;
	}

}

void FileAccess::check_for_errors() {

	if ( !feof(f) ) {

		error_ocurred=true;
		error_type=ferror(f);
		perror("ERROR: ");
		current_error=ERROR_READING_FILE;//must specify better
	}
}


FileAccess::Error FileAccess::open(const char *p_filename, int p_mode_flags) {

	if (f) {
		ERR_PRINT("File left open, closing");
		close();
	}
		
	perform_endian_swap=false;
	const char* mode_string;
	
	if (p_mode_flags==READ)
		mode_string="rb";
	else if (p_mode_flags==WRITE)
		mode_string="wb";
	else if (p_mode_flags==READ_WRITE) 
		mode_string="wb+";
	else
		return ERROR_INVALID_PARAMETERS;
	
	ERR_FAIL_COND_V(in_use,ERROR_ALREADY_IN_USE);

	f=fopen(p_filename,mode_string);

	if (f==NULL)
		return ERROR_OPENING_FILE;
	else {
		error_ocurred=false;
		in_use=true;
		current_mode=p_mode_flags;
		return OK;
	}
	
	
	
}

void FileAccess::close() {

	ERR_FAIL_COND(!in_use);
	current_error=OK;
        in_use=false;
	fclose(f);
	f=0;
}


void FileAccess::seek(Uint32 p_position) {

	ERR_FAIL_COND(!f);
	if ( fseek(f,p_position,SEEK_SET) ) check_for_errors();
}
void FileAccess::seek_end() {
	ERR_FAIL_COND(!f);

	if ( fseek(f,0,SEEK_END) ) check_for_errors();
}

Uint32 FileAccess::get_pos() {
	ERR_FAIL_COND_V(!f,0);

	Uint32 aux_position;
	if ( !(aux_position = ftell(f)) ) {
		check_for_errors();
	};
	return aux_position;
};



bool FileAccess::eof_reached() {
	ERR_FAIL_COND_V(!f,true);

	return (feof(f)!=0);

}

FileAccess::Error FileAccess::get_error() {

	return current_error;
}

Uint8 FileAccess::get_byte() {

	ERR_FAIL_COND_V(!f,0);
	ERR_FAIL_COND_V( !(current_mode&READ ) , 0 );
	
	Uint8 byte;
	if ( fread(&byte,1,1,f)==0 ) 
		check_for_errors();
	
	return byte;
}

void FileAccess::get_byte_array(Uint8 *p_dest,int p_elements) {
	ERR_FAIL_COND(!f);
	
	ERR_FAIL_COND( !(current_mode&READ ) );
	
	if ( fread(p_dest,p_elements,1,f)==0 ) check_for_errors();
}

void FileAccess::get_word_array(Uint16 *p_dest,int p_elements) {
	ERR_FAIL_COND(!f);
	
	ERR_FAIL_COND( !(current_mode&READ ) );

	for (int i=0;i<p_elements;i++)
		p_dest[i]=get_word();
}

Uint16 FileAccess::get_word() {
	ERR_FAIL_COND_V(!f,0);

	ERR_FAIL_COND_V( !(current_mode&READ ) , 0 );

	Uint8 aux_byte1=get_byte();
	Uint8 aux_byte2=get_byte();
	
	endian_swap(aux_byte1,aux_byte2);

	Uint16 ret=aux_byte2;
	ret<<=8;
	ret|=aux_byte1;

	return ret;
}

Uint32 FileAccess::get_dword() {
	ERR_FAIL_COND_V(!f,0);

	ERR_FAIL_COND_V( !(current_mode&READ ) , 0 );

	Uint16 aux_word1 = get_word();
	Uint16 aux_word2 = get_word();

	endian_swap(aux_word1,aux_word2);
	
	Uint32 ret=aux_word2;
	ret<<=16;
	ret|=aux_word1;
	
	return ret;

}


void FileAccess::store_byte(Uint8 p_dest) {
	ERR_FAIL_COND(!f);

	ERR_FAIL_COND( !(current_mode&WRITE ) );
	
	if ( fwrite(&p_dest,1,1,f)==0 ) check_for_errors();
}

void FileAccess::store_byte_array(const Uint8 *p_dest,int p_elements) {

	ERR_FAIL_COND(!f);
	ERR_FAIL_COND( !(current_mode&WRITE ) );
	if ( fwrite(p_dest,p_elements,1,f)==0 ) check_for_errors();
}

void FileAccess::store_word(Uint16 p_dest) {

	ERR_FAIL_COND(!f);
	ERR_FAIL_COND( !(current_mode&WRITE ) );
	Uint8 aux_byte1,aux_byte2;

	aux_byte1=p_dest & 0xFF;
	aux_byte2=p_dest >> 8;

//	endian_swap(aux_byte1,aux_byte2);

	store_byte(aux_byte1);
	store_byte(aux_byte2);

}

void FileAccess::store_dword(Uint32 p_dest) {

	ERR_FAIL_COND(!f);
	ERR_FAIL_COND( !(current_mode&WRITE ) );
	Uint16 aux_word1,aux_word2;


	aux_word1=p_dest & 0xFFFF;
	aux_word2=p_dest>>16;

//	endian_swap(aux_word1,aux_word2);

	store_word(aux_word1);
	store_word(aux_word2);


}

void FileAccess::truncate_file(const char *p_filename, Uint32 p_length) {

	/// @todo: no truncate?

//	if (truncate(p_filename, (off_t)p_length))
//		check_for_errors();
};

#endif

