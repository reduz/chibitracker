//
// C++ Interface: config
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFIG_API_H
#define CONFIG_API_H


#include "globals/file_access_wrapper.h"
#include "string/rstring.h"
#include "signals/signals.h"

using namespace GUI;



/**
	@author Juan Linietsky <reduz@gmail.com>
*/


class ConfigApi{
public:
	
	/* Load Callback */
	Signal< Method3< String, String, String> > read_entry_signal; ///< section, entry, value
	Signal< Method1<String> > read_section_begin_signal; ///begin reading section
	Signal< Method1<String> > read_section_end_signal; ///end reading section
	Signal<> read_finished_signal; ///end reading config
	Signal<> write_entry_signal; ///called when must write config

	
	/* Save Methods */
	virtual void set_section(String p_section)=0;
	virtual void add_entry(String p_name, String p_var,String p_comment="")=0;
	
	virtual bool save()=0;
	virtual bool load()=0;	
	ConfigApi();
	
	virtual ~ConfigApi();

};

#endif
