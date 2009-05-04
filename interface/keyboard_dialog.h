//
// C++ Interface: keyboard_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KEYBOARD_DIALOG_H
#define KEYBOARD_DIALOG_H


#include "mixer/mixer_soft.h"
#include "base/window.h"
#include "bundles/combo_box.h"
#include "bundles/margin_group.h"
#include "bundles/spin_box.h"
#include "bundles/message_box.h"
#include "widgets/check_button.h"
#include "widgets/list.h"
#include "widgets/scroll_bar.h"
#include "widgets/line_edit.h"
#include "widgets/label.h"
#include "interface/config_api.h"

using namespace GUI;

/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class KeyboardDialog : public Window {

	List *list;
	LineEdit *bind_string;
	
	void change_cbk();
	void clear_cbk();
	void keybind_selected( int p_which);

	MessageBox *mb;
	Window *kb_change_window;
	Label *kb_change_label;
	unsigned int kb_change_last;
	
	SpinBox *spin_rate;
	SpinBox *spin_delay;
	
	void keyrepeat_changed(double);
	
	void kb_change_ok();
	void key_handler(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	void update_kb_text();
	ConfigApi *config;
	void save_slot();
	bool allow_kb_load;
	void read_slot(String p_section,String p_entry,String p_value);
	void read_finished();
	
public:
	
	void update_keybind_list();
	KeyboardDialog(Window *p_parent,ConfigApi *p_config);
	~KeyboardDialog();

};

#endif
