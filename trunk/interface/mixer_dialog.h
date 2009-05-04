//
// C++ Interface: mixer_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MIXER_DIALOG_H
#define MIXER_DIALOG_H

#include "mixer/mixer_soft.h"
#include "base/window.h"
#include "bundles/combo_box.h"
#include "bundles/margin_group.h"
#include "widgets/check_button.h"
#include "widgets/button.h"
#include "widgets/line_edit.h"
#include "widgets/slider.h"
#include "widgets/value_label.h"

#include "interface/config_api.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class MixerDialog : public Window {

	Mixer_Soft *sw_mixer;
	ComboBox *interpolation;
	CheckButton *filters;
	CheckButton *volume_ramp;
	CheckButton *declicker;
	CheckButton *clipping;
	CheckButton *effects;
	CheckButton *oversampling;
	HSlider *globalvol;
	
	bool updating;
	void update_vars();
	
	void update_params();
	void update_params_i(int);
	void update_params_p(double);
	
	ConfigApi *config;
	
	void read_slot(String p_section,String p_entry,String p_value);
	void read_finished();
	void save_slot();
public:
	
	void set_mixer(Mixer_Soft *p_sw_mixer);
	MixerDialog(Window *p_parent,ConfigApi *p_config);
	~MixerDialog();

};

#endif
