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

#ifndef ORDERLIST_EDIT_H_
#define ORDERLIST_EDIT_H_

#include "tracker/editor.h"
#include "base/widget.h"
#include "base/range.h"

using namespace GUI;

class OrderListEditor : public Widget {



	void draw_single_mode();

	int column_offset, row_offset;
	int current_playing_order;

	

	Editor *editor;

	virtual void resize(const Size& p_new_size);
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_doubleclick(const Point& p_pos,int p_modifier_mask);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< either pressed or released, 
	virtual bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	Size size;

	virtual Size get_minimum_size_internal();
		
	RangeBase *range;
	void range_changed(double p_toval);
public:

	Signal<> current_pattern_changed;
			
	void check_playing_order();
	
	void set_range(RangeBase *p_range);
	void set_current_playing_order(int p_order);

	OrderListEditor(Editor *p_editor);
	~OrderListEditor();

};


#endif
