//
// C++ Interface: sample_file_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAMPLE_FILE_DIALOG_H
#define SAMPLE_FILE_DIALOG_H

#include "widgets/check_button.h"
#include "tracker/tracker.h"
#include "bundles/file_dialog.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
 */

 using namespace GUI;
 
 
class FileSystemSample : public FileSystem {
	
	Tracker *tracker;
	FileSystem *fs; //internal
	Song song; //aux stuff
	bool inside_song;
	int sample_list_idx;
	String inside_song_path;
	String pre_inside_song_path;

	Sample external_sample;
public:
	
	bool is_inside_song() { return inside_song; }
	
	virtual bool list_dir_begin();
	virtual String get_next(bool* p_is_dir=0);
	virtual void list_dir_end();
	
	virtual int get_drive_count();
	virtual String get_drive(int p_drive);
	
	virtual bool change_dir(String p_dir); ///< can be relative or absolute, return false on success
	virtual String get_current_dir(); ///< return current dir location
	
	virtual bool make_dir(String p_dir);
	
	virtual bool file_exists(String p_file);
	
	void set_sample_in_virtual_piano(String p_file);
	void load_sample_from_inside_song(String p_sample_file,Sample *p_sample);
		
	
	FileSystemSample(Tracker *p_tracker);
	virtual ~FileSystemSample();
};
 
class SampleFileDialog : public FileDialog {

	
	Tracker *tracker;

	void key_handler(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	void file_selected(String p_string);
	
	FileSystemSample fs_sample;
	
	virtual bool ignore_filter();
	
	int sample_to_load;
	
	void open_samples_callback();
	
	MarginGroup *options_mg;
	CheckButton *make_instruments;
public:

	Signal<> samples_opened_signal; //notify samples were opened

	void set_sample_to_load(int p_sample);
	void show();
	SampleFileDialog(Window *p_parent,Tracker *p_tracker);
	~SampleFileDialog();

};

#endif
