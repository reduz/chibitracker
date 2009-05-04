//
// C++ Interface: sample_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAMPLE_SCREEN_H
#define SAMPLE_SCREEN_H

#include "widgets/list.h"
#include "widgets/check_button.h"
#include "widgets/scroll_bar.h"
#include "bundles/margin_group.h"
#include "bundles/menu_box.h"
#include "bundles/tab_box.h"
#include "bundles/file_dialog.h"
#include "containers/box_container.h"

#include "tracker/tracker.h"

#include "interface/sample_viewer_zoom.h"
#include "interface/sample_editor.h"
#include "interface/sample_status_list.h"
#include "interface/sample_file_dialog.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SampleScreen : public HBoxContainer {

	enum FileMenu {
		
		MENU_CLEAR,
		MENU_LOAD,
		MENU_SAVE,
		MENU_COPY,
		MENU_CUT,
		MENU_PASTE,
		MENU_MAKE_INSTRUMENT,
		
	};
	
	
	
	int selected;
	
	bool selecting_outside;
	bool updating;

	SampleEditor *sample_editor;
	
	VBoxContainer *sample_properties;
	
	Tracker *tracker;
	List *sample_list;
	SampleStatusList *sample_status;
	
	/* Sample (song) stuff */
	
	HSlider *global_volume;
	HSlider *default_volume;
	
	CheckButton *use_default_pan;
	HSlider *default_pan;
	
	ComboBox *vibrato_type;
	HSlider *vibrato_speed;
	HSlider *vibrato_depth;
	HSlider *vibrato_rate;
	
	MenuBox *edit;
	MenuBox *sample_edit;
	MenuBox *sample_fx;
	
	void parameter_changed();
	void parameter_changed_c(int p_v);
	void parameter_changed_p(double p_v);
	void list_sample_selected(int p_which);
	
	void open_samples_callback();
	void save_sample_callback(String);
	void sample_file_action_callback(int p_which);
	FileDialog *file_dialog;
	SampleFileDialog *sample_file_dialog;
	void set_in_window();
	
	Sample clipboard;
	
	void sample_data_edit_menu_callback(int p_which);
	
	void sample_name_edited(int p_which,String p_name);
	
	TabBox *mini_tabbox;
public:
	
	void set_sample_path(String p_path);
	Signal< Method1<int> > instrument_changed_signal;
	Signal<> instrument_list_changed_signal;
	
	void handle_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	void update_components();
	void select(int p_which);
	void force_focus() { sample_list->get_focus(); }
	
	void update_timer_callback();
	
	SampleScreen(Tracker *p_tracker,bool p_mini=false);
	~SampleScreen();

};

#endif
