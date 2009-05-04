/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : reduzio@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PATTERN_EDIT_WIDGET_H
#define PATTERN_EDIT_WIDGET_H

#include "bundles/window_box.h"
#include "containers/box_container.h"
#include "containers/grid_container.h"
#include "bundles/spin_box.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include "widgets/arrow_button.h"
#include "widgets/scroll_bar.h"
#include "interface/pattern_editor.h"
#include "interface/pattern_editor_rows.h"
#include "interface/pattern_editor_top.h"
#include "interface/pattern_editor_oscs.h"
#include "interface/orderlist_editor.h"
#include "tracker/editor.h"
#include "containers/box_container.h"
#include "bundles/margin_group.h"
#include "bundles/spin_box.h"
#include "bundles/combo_box.h"
#include "widgets/separator.h"

using namespace GUI;

 
 
class PatternScreen : public VBoxContainer {



	//PatternEditorOscs *pattern_editor_oscs;
	PatternEditor *pattern_editor;
	PatternEditorRows *pattern_editor_rows;
	PatternEditorTop *pattern_editor_top;
	OrderListEditor *orderlist_editor;
	VScrollBar *orderlist_scroll;
	VScrollBar *pattern_v_scroll;
	HScrollBar *pattern_h_scroll;
			
	VBoxContainer *orderlist_vbox;
	
	ArrowButton *orderlist_show;
	SpinBox *pattern;
	
	Editor *editor;
	
	void pattern_spin_changed_cbk(double p_to_which);
	void pattern_h_pos_changed(double p_to_which);
	void pattern_v_pos_changed(double p_to_which);
	void pattern_spin_changed(double p_to_which);
	void pattern_editor_resized();
	void pattern_editor_moved();
	void pattern_editor_update();
	void pattern_changed();
	
	bool updating_bars;
	bool updating_pattern;


	void set_in_window();
	struct Settings {
		Window *popup;
		
		ComboBox *undo_list;
		SpinBox *cursor_stepping;
		SpinBox *row_hl_major;
		SpinBox *row_hl_minor;
		Button *mask_volume;
		Button *mask_instrument;
		Button *mask_command;
		SpinBox *cursor_pattern_len;
		SpinBox *cursor_pattern_len_rep;
		WindowBox *vb;
		
		
	} settings;

	void toggle_orderlist_visible();
	void show_settings();

	void undo_editing();
	
	void update_undo_list();
	void settings_editing_changed_v(double p_v);
	bool settings_editing_updating;
	void apply_settings_editing();
	void apply_settings_pattern();
	
	void pattern_scroll_dir(Direction p_scr);
	
	bool mini;
public:	
	

	
	

	PatternEditor * get_pattern_editor() { return pattern_editor; }

	void update_components(); //needed from outside :(
	void update_timer_callback();
	void focus_pattern_edit_widget();

	void handle_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	void goto_pattern();
	void goto_orderlist();
	
	PatternScreen(Tracker *p_tracker,bool p_mini=false);
	~PatternScreen();
};

#endif
