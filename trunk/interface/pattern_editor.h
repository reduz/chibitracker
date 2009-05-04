/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003-2006 by Juan Linietsky
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

#ifndef PATTERN_EDITOR_H
#define PATTERN_EDITOR_H

#include "tracker/editor.h"
#include "song/song.h"
//#include "trackercore/player_data.h"
#include "base/widget.h"
#include "bundles/spin_box.h"
#include "bundles/margin_group.h"
#include "widgets/button.h"
#include "containers/center_container.h"
#include "interface/pattern_transpose.h"
using namespace GUI;

/**
 *
 * Juan Linietsky
 **/
class PatternEditor : public Widget {


	int old_row_to_draw;

	Song *song;
	Editor *editor;

	Size size;

	int get_column_width();
	int get_row_height();
	int get_font_height();
	int get_font_width();
	
	/* Events & Ddefault Handlers */
	virtual void resize(const Size& p_new_size);
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< either pressed or released,
	
	virtual void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	virtual bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	

        /* misc methods */

	void adjust_editor();
	void print_single_row(int rowofs);
	void print_rows(const Rect &p_repaint_area);
	void print_cursor();

	void screen_to_pattern_coords(int &x,int &y,int &field, bool &outside);

	bool previous_shift;

	struct Mouse_Select {
		int begin_x,begin_y;
		bool begun;
		bool active;

	} mouse_select;

	Window *amp_param_window;
	SpinBox *amp_param;
	MarginGroup *amp_group;
	void apply_amp();
	void apply_amp_t(String meh);
	
	struct TimeScale {
		Window *window;
		VBoxContainer *linear_hide_vb;
		SpinBox *initial_stretch;
		SpinBox *final_stretch;
		SpinBox *total_stretch;
		SpinBox *offset;
		
		bool linear;
	} timescale;
	
	void timescale_apply();
	void set_in_window();
	PatternTranspose *transpose;
	
	void transposed_callback();
	
public:
	
	

	Signal< Method1<Direction> > scroll_attempt;
	Signal<> new_undoable_operation;
		
	Size get_size();
	
	Signal< Method1<int> > instrument_changed_signal;
	Signal<> operation_performed_signal;
	Signal<> display_pos_changed;
	Signal<> size_changed;
	Signal<> pattern_changed;

	
	PatternEditor(Editor *p_editor);
	~PatternEditor();
};

#endif
