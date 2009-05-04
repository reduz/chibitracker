//
// C++ Interface: interface_settings_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INTERFACE_SETTINGS_DIALOG_H
#define INTERFACE_SETTINGS_DIALOG_H


#include "base/file_system.h"
#include "base/window.h"
#include "bundles/combo_box.h"
#include "bundles/spin_box.h"
#include "widgets/check_button.h"
#include "widgets/button.h"
#include "widgets/line_edit.h"
#include "interface/config_file.h"
#include "interface/theme_loader.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class InterfaceSettingsDialog : public Window{
	
	ComboBox *fullscreen_list;
	CheckButton *fullscreen;
	ConfigApi *config;
	ComboBox *theme_list;
	SpinBox *repaint_interval;
	
	void fullscreen_mode_changed(int);
	void fullscreen_toggle(bool p_fsc);
	void key_handler(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	int find_closest_fs_mode(Size p_size);
	void read_slot(String p_section,String p_entry,String p_value);
	void read_finished();
	void save_slot();
	
	Size fsconfig;
	
	FileAccessWrapper *file;
	ConfigFile *config_theme;
	ThemeLoader *theme_loader;
	void load_theme_slot();
		
	String last_theme_loaded;
	
	String system_theme_path;
	String theme_path;
	void repaint_interval_changed(double p_to);
public:
	void load_theme(String p_theme);
	
	Signal< Method1<int> > repaint_interval_changed_signal;
	void set_system_theme_path(String p_path);
	void set_theme_path(String p_path);
	InterfaceSettingsDialog(Window *p_parent,ConfigApi *p_config);
	~InterfaceSettingsDialog();

};

#endif
