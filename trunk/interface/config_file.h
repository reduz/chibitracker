//
// C++ Interface: config_file
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "interface/config_api.h"
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class ConfigFile : public ConfigApi {

	FileAccessWrapper *file;
	String path;
public:
	
	/* Save Methods */
	void set_section(String p_section);
	void add_entry(String p_name, String p_var,String p_comment="");
	
	
	bool save();
	bool load();
	
	void set_path(String p_path);
	
	ConfigFile(FileAccessWrapper *p_file);
	~ConfigFile();

};

#endif
