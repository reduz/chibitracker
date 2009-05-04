//
// C++ Interface: paths_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PATHS_DIALOG_H
#define PATHS_DIALOG_H


#include "bundles/file_dialog.h"
#include "base/window.h"
#include "widgets/button.h"
#include "widgets/line_edit.h"
#include "interface/config_file.h"
#include "interface/default_paths.h"

/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class PathsDialog : public Window{

	enum SettingPathFor{
		SONG,
		SAMPLE,
		INSTRUMENT
	};

	SettingPathFor setting_for;
	
	
	DefaultPaths *paths;
	LineEdit *song_path;
	LineEdit *sample_path;
	LineEdit *instrument_path;
	
	ConfigApi *config;
	
	void browse_song_path();
	void browse_sample_path();
	void browse_instrument_path();
	
	void dir_selected(String p_dir);
	
	
	void read_slot(String p_section,String p_entry,String p_value);
	void read_finished();
	void save_slot();
	
	bool updating;
	void update_paths();
	void paths_text_changed(String);
	
	FileDialog *browser;
public:
	
	Signal<> paths_changed_signal;
	
	PathsDialog(Window *p_parent,DefaultPaths *p_paths,ConfigApi *p_config);
	~PathsDialog();
};

#endif
