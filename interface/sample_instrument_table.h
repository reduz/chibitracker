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

#ifndef SAMPLE_INSTRUMENT_TABLE_H
#define SAMPLE_INSTRUMENT_TABLE_H



/**
 *
 * Juan Linietsky
 **/


#include "base/widget.h"
#include "base/range.h"
#include "song/instrument.h"
#include "tracker/editor.h"

using namespace GUI;

/**
  *@author Juan Linietsky
  */

/******************************
 interface_widget_equivtable.h
 ----------

This edits the note -> note/sample
relation in an instrument
********************************/


class SampleInstrumentTable : public Widget {

	enum {

		LINE_CHAR_WIDTH=12
	};


	void draw_single_mode();

	int column_offset, row_offset;
	int cursor_x,cursor_y;
        int sample_mask;

	Instrument *instrument; //songptr!

	Editor *editor;

	virtual void resize(const Size& p_new_size);
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< either pressed or released, 
	virtual bool key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	Size size;

	virtual Size get_minimum_size_internal();
		
	RangeBase *range;
	void range_changed(double p_toval);
	
	int get_row_size();
	void set_row_offset(double p_ofs);	
public:

	// wow, this code is so old (about 2000'), does not even respect my modern coding style..
	// I'm not touching it as a way to remember those days...
	
	
	
	void MoveUp();
	void MoveDown();
	bool MoveLeft();
	bool MoveRight();
	void PageUp();
	void PageDown();

	
	void AddSeparator();
	void ClearOrder();
        void InsertBlank();
	void DeleteRow();
	void InsertRepeat();

	void set_range(RangeBase *p_range);
	
	
	void set_instrument(Instrument *p_instrument);
	SampleInstrumentTable(Editor *p_editor);
	~SampleInstrumentTable();

};


#endif
