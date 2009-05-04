//
// C++ Interface: sound_driver_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SOUND_DRIVER_DIALOG_H
#define SOUND_DRIVER_DIALOG_H

#include "base/window.h"
#include "bundles/combo_box.h"
#include "widgets/check_button.h"
#include "widgets/button.h"
#include "widgets/line_edit.h"
#include "interface/config_api.h"


using namespace GUI;
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundDriverDialog : public Window {

	
	bool updating;
	bool updating_parameters;
	ComboBox *sound_driver;
	ComboBox *mix_freq;
	ComboBox *bits;
	CheckButton *stereo;
	ComboBox *buffsize;
	LineEdit *status;

	void update_parameters();
	void update_status();
	
	void sound_driver_changed(int p_to_which);
	
	void parameter_changed();
	void parameter_changed_i(int p_idx);
	
	void restart();
	void disable();
	
	bool need_param_update;
	
	/* Config */
	ConfigApi *config;
	
	bool config_must_enable_driver;
	int config_driver;
	void read_slot(String p_section,String p_entry,String p_value);
	void read_finished();
	void save_slot();
public:
	
	void show();
	
	void check_driver_status();
	
	SoundDriverDialog(Window *p_parent,ConfigApi *p_config);
	~SoundDriverDialog();

};

#endif
