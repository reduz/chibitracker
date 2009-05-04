//
// C++ Interface: variables_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUIVARIABLES_SCREEN_H
#define PIGUIVARIABLES_SCREEN_H


#include "widgets/line_edit.h"
#include "containers/grid_container.h"
#include "containers/center_container.h"
#include "bundles/spin_box.h"
#include "bundles/margin_group.h"
#include "widgets/list.h"
#include "widgets/scroll_bar.h"
#include "widgets/label.h"
#include "bundles/combo_box.h"
#include "widgets/check_button.h"
#include "widgets/text_edit.h"
#include "bundles/tab_box.h"


#include "tracker/tracker.h"

using namespace GUI;

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class VariablesScreen : public HBoxContainer {
	
	
	Tracker *tracker;
	
	bool updating;
	
	LineEdit *song_name;
	TextEdit *message;
	SpinBox *tempo;
	SpinBox *speed;
	
	SpinBox *global_volume;
	SpinBox *mixing_volume;
	SpinBox *stereo_separation;
			
	CheckButton *instruments;
	CheckButton *old_effects;
	CheckButton *compatible_gxx;
	CheckButton *stereo;
	CheckButton *linear_slides;
	
	
	ComboBox *reverb_mode;
	ComboBox *chorus_mode;
	Window *chorus_window;
	SpinBox *chorus_depth;
	SpinBox *chorus_rate;
	SpinBox *chorus_delay;
	SpinBox *chorus_stereo;
	
	void parameter_changed();
	void parameter_changed_p(double);
	void parameter_changed_i(int);
	void song_name_changed(String p_string);
	void ins_mode_clicked();
	
	Label *toggle_label;
	Window *toggle_ins_mode;

	/* for instrument checkbox */
	void toggle_accept();
	void toggle_cancel();
	void toggle_changed();
	void set_in_window();

	void message_changed();
	
	void chorus_set();
	void chorus_preset_select(int p_which);
	
	TabBox *mini_tabbox;
	VBoxContainer *var_vb; //variables vbox
	
public:
	
	Signal<> instrument_mode_changed;
	Signal<> song_name_changed_signal;
	
	void update_components();
	void force_focus() { song_name->get_focus(); }	
	VariablesScreen(Tracker *p_tracker,bool p_mini=false);
	~VariablesScreen();

};



#endif
