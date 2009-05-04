// //
// C++ Implementation: config_file
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "config_file.h"
#include "error_macros.h"
#include <stdio.h>

/* Save Methods */

void ConfigFile::set_path(String p_path) {

	path=p_path;
}


void ConfigFile::set_section(String p_section) {

	ERR_FAIL_COND(!file->is_open());

	file->store_byte('\n');
	file->store_byte('[');
	file->store_byte_array( (Uint8*)p_section.ascii(false).get_data(), p_section.length() );
	file->store_byte(']');
	file->store_byte('\n');
	file->store_byte('\n');

}
void ConfigFile::add_entry(String p_name, String p_var,String p_comment) {

	ERR_FAIL_COND(!file->is_open());

	file->store_byte_array( (Uint8*)p_name.ascii(false).get_data(), p_name.length() );
	file->store_byte('=');
	file->store_byte_array( (Uint8*)p_var.ascii(false).get_data(), p_var.length() );
	file->store_byte(' ');
	if (p_comment!="") {
		file->store_byte(';');
		file->store_byte(' ');
		file->store_byte_array( (Uint8*)p_comment.ascii(false).get_data(), p_comment.length() );
	}
	file->store_byte('\n');

}

bool ConfigFile::save() {

	printf("saving config to: %s\n",path.utf8().get_data());
	if (path=="")
		return true;
	if (file->open( path.utf8().get_data() , FileAccessWrapper::WRITE ))
		return true;
	
	write_entry_signal.call();
	file->close();
	
	return false;
	
}
bool ConfigFile::load() {

	if (path=="")
		return true;

	
	if (file->open( path.utf8().get_data() , FileAccessWrapper::READ ))
		return true;
	
	char axloadzone[500];
	
	String section="";
	
	while(1) {

		int auxzpos=0;
		Uint8 b;
		
		while(1) {
			
			b=file->get_byte();
						
			if (file->eof_reached())
				break;
			if (b=='\r')
				continue;
			axloadzone[auxzpos++]=b;
			if (b=='\n')
				break;
		}
		
		axloadzone[auxzpos]=0;
		
		if (file->eof_reached())
			break;

		String auxstring=axloadzone;


		if (auxstring.find(";")>=0) {
			auxstring=auxstring.substr(0,auxstring.find(";"));
		}

		
		
		/* remove trailing space */

		while ((auxstring.length()>0) && ((auxstring)[0]<33)) {

			auxstring=auxstring.substr(1,auxstring.length()-1);
		}

		while ((auxstring.length()>0) && ((auxstring)[auxstring.length()-1]<33)) {

			auxstring=auxstring.substr(0,auxstring.length()-1);
		}

		if (auxstring.length()==0) continue;
		
		
		if (auxstring[0]=='[') {
			// section!
			if (section!="")
				read_section_end_signal.call(section);

			section=auxstring.substr(1,auxstring.length()-2);
			
			read_section_begin_signal.call(section);
			

		} else if (auxstring.find("=")>0) {
			//variable
			String key,data;

			key=auxstring.substr(0,auxstring.find("="));

			//remove trailnig space
			while ((key.length()>0) && ((key)[key.length()-1]<33)) {

				key=key.substr(0,key.length()-1);
			}
			
			
			data=auxstring.substr(auxstring.find("=")+1,(auxstring.length()-auxstring.find("="))-1);

			//remove trailnig space
		
			while ((data.length()>0) && ((data)[0]<33)) {

				data=data.substr(1,data.length()-1);
			}

			read_entry_signal.call( section, key, data );
	//		printf("read %s:%s=%s\n",section.ascii().get_data(),key.ascii().get_data(),data.ascii().get_data() );

		}
		
	}
	
	read_finished_signal.call();
	
	file->close();
	
	return false;
}

ConfigFile::ConfigFile(FileAccessWrapper *p_file) { 
	
	file=p_file;
}


ConfigFile::~ConfigFile() {
	
}


