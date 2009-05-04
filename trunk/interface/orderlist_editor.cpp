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

#include "orderlist_editor.h"
#include "base/painter.h"
#include "tracker/key_bindings.h"
#include "interface/ctskin.h"
#include <stdio.h>

void OrderListEditor::check_playing_order() {


	int current_order=editor->get_player()->is_playing()?editor->get_player()->get_current_order():-1;
	if (current_order!=current_playing_order)
		update();
	
	current_playing_order=current_order;
	
	
}

Size OrderListEditor::get_minimum_size_internal() {

	Size min;
	
	int font_width=get_painter()->get_font_char_width(font(FONT_ORDERLIST),' ');
	min.width=font_width*9;
	
	return min;

}
void OrderListEditor::resize(const Size& p_new_size) {

	size=p_new_size;
	
	if (range && get_painter()) {
	
		int font_height=get_painter()->get_font_height(font(FONT_ORDERLIST));
		int row_height=constant(C_ORDERLIST_ROW_SEPARATION)+font_height;
		int rows_to_draw;
		rows_to_draw=size.height/row_height;
		
		range->set_page( rows_to_draw );
	}

}
void OrderListEditor::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	int i;
	char tmpnumb[4]={'0','0','0',0};

	if ((editor==NULL)) return;

	Painter *p=get_painter();
	
	int font_width=p->get_font_char_width(font(FONT_ORDERLIST),' ');
	int font_height=p->get_font_height(font(FONT_ORDERLIST));
	int row_height=constant(C_ORDERLIST_ROW_SEPARATION)+font_height;
	int current_order=editor->get_player()->is_playing()?editor->get_player()->get_current_order():-1;

	int rows_to_draw;
	rows_to_draw=size.height/row_height;

	if (editor->orderlist_get_cursor_y()<row_offset) row_offset=editor->orderlist_get_cursor_y();
	if (editor->orderlist_get_cursor_y()>=(row_offset+rows_to_draw)) row_offset=(editor->orderlist_get_cursor_y()-rows_to_draw)+1;

	for (i=0;i<rows_to_draw;i++) {

		p->draw_fill_rect(Point(0,row_height*i),Point(size.width,row_height),color(COLOR_ORDERLIST_BG));

		if ((i+row_offset)<Song::MAX_ORDERS) {
			tmpnumb[0]='0'+(i+row_offset)/100;
			tmpnumb[1]='0'+((i+row_offset)/10) % 10;
			tmpnumb[2]='0'+(i+row_offset) % 10;
			
			Color drawcol;
			if (current_order==(i+row_offset)) {
				drawcol=color(COLOR_ORDERLIST_PLAYED_ORDER);
			} else {
				drawcol=color(COLOR_ORDERLIST_ORDER);
			}

			p->draw_text( font(FONT_ORDERLIST),Point(font_width,row_height*i+constant(C_ORDERLIST_ROW_SEPARATION)+p->get_font_ascent(font(FONT_ORDERLIST))),tmpnumb,drawcol);

			
			char orderlist_cursor_str[5];
			editor->orderlist_get_cursor_string(i+row_offset,orderlist_cursor_str);
			p->draw_text(font(FONT_ORDERLIST),Point(font_width*5,row_height*i+constant(C_ORDERLIST_ROW_SEPARATION)+p->get_font_ascent(font(FONT_ORDERLIST))),orderlist_cursor_str,color(COLOR_ORDERLIST_PATTERN));
		}

	}

	//clear last part
	p->draw_fill_rect(Point(0,row_height*rows_to_draw),Size(size.width,row_height),color(COLOR_ORDERLIST_BG));

 //       painter.fillRect(
//	window.draw_rectangle(sepcol,TRUE,13+font_width*3,0,3,height());
	if (has_focus()) {

		
		p->draw_rect( Point(0,0),Size(size.width-1,size.height-1),color(COLOR_ORDERLIST_FOCUS_FRAME));
	}
	int initx,inity;

	initx=font_width*4+editor->orderlist_get_cursor_x()*font_width;
	inity=row_height*(editor->orderlist_get_cursor_y()-row_offset);

	if (has_focus()) {
	
		p->draw_stylebox( stylebox(SB_ORDERLIST_CURSOR),Point(font_width+initx-1,inity-1),Size(font_width+2,row_height+2));

	}	
	
	int font_asc=p->get_font_ascent( font(FONT_SAMPLETABLE ) );
	int arrow_ofs=(editor->orderlist_get_cursor_y()-row_offset)*row_height+constant(C_ORDERLIST_ROW_SEPARATION);
	int arrow_h=font_asc;

	p->draw_arrow( Point(0,arrow_ofs), Size(font_width,arrow_h), RIGHT, color( COLOR_ORDERLIST_ORDER ) );
	
	
}


void OrderListEditor::mouse_doubleclick(const Point& p_pos,int p_modifier_mask) {
	
	Painter *p=get_painter();
	int font_height=p->get_font_height(font(FONT_ORDERLIST));
	int row_height=constant(C_ORDERLIST_ROW_SEPARATION)+font_height;
	
	int new_row=p_pos.y;
	new_row/=row_height;
	new_row+=row_offset;
	if (new_row>=Song::MAX_ORDERS) {

		new_row=Song::MAX_ORDERS-1;
	}
	
	int pattern=editor->get_song()->get_order( new_row );
	
	if (pattern< 0 || pattern>=Song::MAX_PATTERNS)
		return;
	
	editor->set_current_pattern( pattern );
	current_pattern_changed.call();

}

void OrderListEditor::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	if (!p_press)
		return;
	
	if (range) {

		if (p_button==BUTTON_WHEEL_UP) {
			
			range->set( range->get() - 4 );
			return;
		} else if (p_button==BUTTON_WHEEL_DOWN) {
			
			range->set( range->get() + 4 );
			return;
			
		}
	}
	Painter *p=get_painter();
	int font_width=p->get_font_char_width(font(FONT_ORDERLIST),' ');
	int font_height=p->get_font_height(font(FONT_ORDERLIST));
	int row_height=constant(C_ORDERLIST_ROW_SEPARATION)+font_height;
	
	int new_row=p_pos.y;
	new_row/=row_height;
	new_row+=row_offset;
	if (new_row>=Song::MAX_ORDERS) {

		new_row=Song::MAX_ORDERS-1;
	}

	editor->orderlist_set_cursor_y(new_row);

	switch( p_pos.y / font_width ) {

		case 5: {
			editor->orderlist_set_cursor_x(0);
		} break;
		case 6: {
			editor->orderlist_set_cursor_x(1);
		} break;
		case 7: {
			editor->orderlist_set_cursor_x(2);
		} break;
	};

	update();
		
}

bool OrderListEditor::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	
	if (!p_press)
		return true;
	KeyBindList kbind=KeyBind::get_bind( p_scan_code );
	
	if (kbind==KB_MAX)
		return false;
	
	bool capture=false;
	switch (kbind) {

		case KB_CURSOR_INSERT: {

			editor->orderlist_insert();
		} break;
		case KB_CURSOR_DELETE: {

			editor->orderlist_delete();
		} break;
		case KB_CURSOR_MOVE_UP: {

			editor->orderlist_move_up();
			capture=true;
		} break;
		case KB_CURSOR_MOVE_DOWN: {

			editor->orderlist_move_down();
			capture=true;

		} break;
		case KB_CURSOR_MOVE_LEFT: {

			editor->orderlist_move_left();
			capture=true;

		} break;
		case KB_CURSOR_MOVE_RIGHT: {

			editor->orderlist_move_right();
			capture=true;

		} break;
		case KB_CURSOR_PAGE_UP: {

			editor->orderlist_page_up();
			capture=true;

		} break;
		case KB_CURSOR_PAGE_DOWN: {

			editor->orderlist_page_down();
			capture=true;
		} break;
		
		case KB_CURSOR_FIELD_CLEAR: {
			
			editor->orderlist_clear_order();
			
		} break;
		
		default: {
			
			unsigned int key_char=p_unicode;
			
			if (kbind==KB_ORDERLIST_INSERT_SEPARATOR || key_char=='=' || key_char=='+') {
				
				editor->orderlist_add_separator();
				
			} else 	if ((key_char>='0') && (key_char<='9'))
				editor->orderlist_insert_value(key_char-'0');
			
			
		}
	}
	
	if (range)
		range->set( row_offset );
	update();
	return capture;

	
}


void OrderListEditor::set_current_playing_order(int p_order) {

	if (current_playing_order!=p_order) {

		update();
		current_playing_order=p_order;
	}
}

void OrderListEditor::range_changed(double p_toval) {
	
	int new_row_offset=(int)p_toval;
	
	int cursor_pos=editor->orderlist_get_cursor_y();
	
	cursor_pos+=new_row_offset-row_offset;
	
	row_offset=new_row_offset;
	editor->orderlist_set_cursor_row(cursor_pos);	
	update();
}
void OrderListEditor::set_range(RangeBase *p_range) {
	
	range=p_range;
	range->set_min(0);
	range->set(0);
	range->set_max( Song::MAX_ORDERS );
	range->value_changed_signal.connect( this, &OrderListEditor::range_changed );
}
OrderListEditor::OrderListEditor(Editor *p_editor)  {

	range=0;
	row_offset=0;
	editor=p_editor;
	current_playing_order=-1;
	set_focus_mode(FOCUS_ALL);
}

OrderListEditor::~OrderListEditor(){
	
	
}
