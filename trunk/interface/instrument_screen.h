//
// C++ Interface: instrument_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INSTRUMENT_SCREEN_H
#define INSTRUMENT_SCREEN_H


#include "widgets/list.h"
#include "widgets/check_button.h"
#include "widgets/scroll_bar.h"
#include "widgets/button_group.h"
#include "widgets/slider.h"
#include "bundles/tab_box.h"
#include "bundles/menu_box.h"
#include "bundles/message_box.h"
#include "widgets/value_label.h"
#include "bundles/margin_group.h"
#include "bundles/file_dialog.h"
#include "containers/box_container.h"

#include "tracker/tracker.h"
#include "interface/sample_instrument_table.h"
#include "interface/envelope_editor.h"


using namespace GUI;

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class InstrumentScreen : public HBoxContainer{
	
	
	enum FileMenu {
		
		MENU_CLEAR,
		MENU_CLEAR_ALL,
		MENU_LOAD,
		MENU_SAVE,
		MENU_COPY,
		MENU_CUT,
		MENU_PASTE,
		MENU_REMAP_TO_MANY,
		MENU_REMAP_ALL_TO_MANY
		
	};
	
	
	
	int selected;
	
	void instrument_selected(int p_selected);
	
	bool updating;

	MenuBox *edit;
	List *instrument_list;
	Tracker *tracker;
	
	TabBox *sections;
	
	ButtonGroup nna_group;
	ButtonGroup dct_group;
	ButtonGroup dca_group;
	
	SampleInstrumentTable *sample_table;
	EnvelopeEditor *vol_envelope;
	EnvelopeEditor *pan_envelope;
	EnvelopeEditor *pitch_filter_envelope;
	
	/* Volume */
	
	HSlider *vol_global_send;
	HSlider *vol_random_send;
	HSlider *vol_fadeout;
	
	/* Pan */
	
	CheckButton *pan_use_default;
	HSlider *pan_default;
	HSlider *pan_ppsep;
	HSlider *pan_ppcenter;
	HSlider *pan_random;
	
	/* Pitch/Filter */
	
	CheckButton *env_use_as_filter;
	CheckButton *filter_cutoff_default;
	HSlider *filter_cutoff;
	CheckButton *filter_reso_default;
	HSlider *filter_reso;
	
	void parameter_changed_p(double);
	void parameter_changed_i(int);
	void parameter_changed();
	
	void song_menu_selected(int p_option);
	
	FileDialog *file_dialog;
	void instrument_file_callback(String p_file);
	void instrument_file_callback_multi();
	void instrument_file_action_callback(int p_which);
	
	
	MessageBox *error_msg;
	Instrument clipboard;
	
	void instrument_name_edited(int p_which,String p_name);

	virtual void set_in_window();
	
	TabBox *mini_tabbox;

public:
	
	void set_instrument_path(String p_path);
	
	void select(int p_which);
	Signal<> sample_list_changed_signal;
	InstrumentScreen(Tracker *tracker,bool p_mini=false);
	void update_components();
	void update_timer_callback();
	void handle_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	void force_focus() { instrument_list->get_focus(); }
	~InstrumentScreen();

};

#endif
