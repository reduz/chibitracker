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
//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "base/painter.h"
#include "tracker/key_bindings.h"
#include "sample_instrument_table.h"
#include <math.h> //thanks mrs brisby!
#include "ctskin.h"


int SampleInstrumentTable::get_row_size() {

	return get_painter()->get_font_height( font( FONT_SAMPLETABLE ) )+constant(C_SAMPLETABLE_ROW_SEPARATION);
}

void SampleInstrumentTable::resize(const Size& p_new_size) {

	size=p_new_size;
	get_painter()->get_font_char_width( font( FONT_SAMPLETABLE ),' ' )*LINE_CHAR_WIDTH;

	if (range)
		range->set_page( size.height/get_row_size() );
}
	
void SampleInstrumentTable::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	if (!p_press)
		return;
	
	switch (p_button) {
		
		
		case BUTTON_LEFT: {
	
			int row=p_pos.y/get_row_size();
			int visible_rows=size.height/get_row_size();
			if (row>=visible_rows)
				row=visible_rows-1;
			
			cursor_y=row_offset+row;
			int field=p_pos.x/get_painter()->get_font_char_width( font( FONT_SAMPLETABLE ),' ' );
			
			if (field>=10)
				cursor_x=3;
			else if (field>=9)
				cursor_x=2;
			else if (field>=7)
				cursor_x=1;
			else if (field>=5)
				cursor_x=0;
		} break;
		
		case BUTTON_WHEEL_UP: {
			
			
			if (range)
				range->set( range->get() - 4 );
			
		} break;
		case BUTTON_WHEEL_DOWN: {
			
			
			if (range)
				range->set( range->get() + 4 );
			
		} break;
	}
	
	update();
}
	
Size SampleInstrumentTable::get_minimum_size_internal() {
	
	Size min;
	
	min.width+=get_painter()->get_font_char_width( font( FONT_SAMPLETABLE ),' ' )*LINE_CHAR_WIDTH;

	return min;
		
	
}

	
void SampleInstrumentTable::draw_single_mode() {


	int i;


	int rows_to_draw;

      	if ((editor==NULL) || (instrument==NULL)) return;

	Painter *p=get_painter();
	int font_width=p->get_font_char_width( font( FONT_SAMPLETABLE ),' ' );
//	int font_height=p->get_font_height( font( FONT_SAMPLETABLE ) );
 
	
//	char buf[20];

	rows_to_draw=size.height/get_row_size();

	if (cursor_y<row_offset) row_offset=cursor_y;
	if (cursor_y>=(row_offset+rows_to_draw)) row_offset=(cursor_y-rows_to_draw)+1;

	for (i=0;i<rows_to_draw;i++) {

		if ((i+row_offset)<Note::NOTES) {


			char instr_number[3]={'.','.',0};
			
			if (instrument->get_sample_number(i+row_offset)!=Note::EMPTY) {

				instr_number[0]='0'+(instrument->get_sample_number(i+row_offset)+1)/10;
				instr_number[1]='0'+(instrument->get_sample_number(i+row_offset)+1) % 10;
			}

			char note_left_str[4];
			char note_right_str[4];
			editor->get_single_note_string(i+row_offset,note_left_str);
			editor->get_single_note_string(instrument->get_note_number(i+row_offset),note_right_str);


			p->draw_fill_rect(Point(0,get_row_size()*i),Size(size.width,get_row_size()),color(COLOR_SAMPLETABLE_BG));

			int text_ofs=get_row_size()*i+p->get_font_ascent( font(FONT_SAMPLETABLE))+constant(C_SAMPLETABLE_ROW_SEPARATION);
			
			p->draw_text( font(FONT_SAMPLETABLE), Point(font_width,text_ofs),note_left_str, color(COLOR_SAMPLETABLE_SOLID));
			
			p->draw_text( font(FONT_SAMPLETABLE), Point(font_width*5,text_ofs),note_right_str, color(COLOR_SAMPLETABLE_FG));
			
			
			p->draw_text( font(FONT_SAMPLETABLE), Point(font_width*9,text_ofs),instr_number, color(COLOR_SAMPLETABLE_FG));


		}

	}

	if (has_focus()) {


		
		p->draw_rect(Point(0,0),Size(size.width-1,size.height-1),color(COLOR_SAMPLETABLE_FOCUS));

		int initx,inity;
		const int cursor_spacing[4]={0,2,4,5};

		initx=font_width*5+cursor_spacing[cursor_x]*font_width;
		inity=get_row_size()*(cursor_y-row_offset);
		
		p->draw_rect( Point(initx-1,inity-2), Size(font_width+2,get_row_size()+2) ,color(COLOR_SAMPLETABLE_CURSOR));
	}
	
}

void SampleInstrumentTable::MoveUp() {

        if (cursor_y>0) cursor_y--;

}

void SampleInstrumentTable::MoveDown() {

        if (cursor_y<Note::NOTES-1) cursor_y++;

}

bool SampleInstrumentTable::MoveLeft() {

        if (cursor_x>0) cursor_x--;
	else return false;
	
	return true;

}

bool SampleInstrumentTable::MoveRight() {

        if (cursor_x<3) cursor_x++;
	else return false;
	
	return true;
}

void SampleInstrumentTable::PageUp() {

       cursor_y-=8;
       if (cursor_y<0) cursor_y=0;
}



void SampleInstrumentTable::PageDown() {

       cursor_y+=8;
       if (cursor_y>=Note::NOTES) cursor_y=Note::NOTES-1;
}


void SampleInstrumentTable::AddSeparator() {

//	song->variables->orderlist[cursor_y]=253;
//	song->set_order(cursor_y,253);
	MoveDown();
}

void SampleInstrumentTable::ClearOrder() {

	if (cursor_x>1) instrument->set_sample_number(cursor_y,Note::EMPTY);
	sample_mask=Note::EMPTY;
	MoveDown();
}

bool SampleInstrumentTable::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	if (!p_press)
		return true;
	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return false;
	
	
	bool capture=false;
	switch (kbind) {
	
		case KB_PATTERN_RAISE_NOTES: {
			
			for (int i=0;i<Note::NOTES;i++) {
				if (instrument->get_note_number(i)>=Note::NOTES)
					continue;
				int new_note=instrument->get_note_number(i)+1;
				if (new_note>=Note::NOTES)
					new_note=Note::EMPTY;
				
				instrument->set_note_number(i,new_note);
			}
			
		} break;
		case KB_PATTERN_LOWER_NOTES: {
			
			for (int i=0;i<Note::NOTES;i++) {
				if (instrument->get_note_number(i)>=Note::NOTES)
					continue;
				if (instrument->get_note_number(i)==0)
					continue;
				instrument->set_note_number(i,instrument->get_note_number(i)-1);
				
			}
			
		} break;
		case KB_CURSOR_WRITE_MASK: {
			
			//for (int i=0;i<Note::NOTES;i++) {
			instrument->set_sample_number(cursor_y,sample_mask);
			MoveDown();
			//}
			
		} break;
		/*	
		case KB_CURSOR_INSERT: {

			editor->orderlist_insert();
		} break;
		case KB_CURSOR_DELETE: {

			editor->orderlist_delete();
	} break; */
		case KB_CURSOR_MOVE_UP: {

			MoveUp();
			capture=true;
		} break;
		case KB_CURSOR_MOVE_DOWN: {

			MoveDown();
			capture=true;

		} break;
		case KB_CURSOR_MOVE_LEFT: {

			if (MoveLeft())
				capture=true;

		} break;
		case KB_CURSOR_MOVE_RIGHT: {

			if (MoveRight())
				capture=true;

		} break;
		case KB_CURSOR_PAGE_UP: {

			PageUp();
			capture=true;

		} break;
		case KB_CURSOR_PAGE_DOWN: {

			PageDown();

			capture=true;
		} break;
		
		case KB_CURSOR_FIELD_CLEAR: {
			
			ClearOrder();
			
			
		} break;
		default: {
			
			
			switch (cursor_x) {
				case 0: {

					int tmpkey = editor->get_note_from_charcode(kbind);
					if (tmpkey>=0) {

						instrument->set_note_number(cursor_y,tmpkey);
						instrument->set_sample_number(cursor_y,sample_mask);

						MoveDown();
					}
				} break;
				case 1: {

					if ((p_unicode>='0') && (p_unicode<='9')) {

						int aux_numb=instrument->get_note_number(cursor_y) % 12;

						instrument->set_note_number(cursor_y,(p_unicode-'0')*12+aux_numb);
						MoveDown();
					}
				} break;
				case 2: {

					if ((p_unicode>='0') && (p_unicode<='9')) {

						int aux_numb=instrument->get_sample_number(cursor_y);
						
						if (aux_numb==Note::EMPTY) aux_numb=0;
						else aux_numb++;

						int num_right=aux_numb % 10;

						aux_numb=(p_unicode-'0')*10+num_right;

						if (aux_numb==0) aux_numb=Note::EMPTY;
						else aux_numb--;

						sample_mask=aux_numb;
						instrument->set_sample_number(cursor_y,aux_numb);
						MoveRight();
					}
				} break;
				case 3: {

					if ((p_unicode>='0') && (p_unicode<='9')) {

						int aux_numb=instrument->get_sample_number(cursor_y);
						int num_left;
						if (aux_numb==Note::EMPTY) aux_numb=0;
						else aux_numb++;

						num_left=aux_numb / 10;

						aux_numb=num_left*10+(p_unicode-'0');

						if (aux_numb==0) aux_numb=Note::EMPTY;
						else aux_numb--;

						sample_mask=aux_numb;
						instrument->set_sample_number(cursor_y,aux_numb);
						MoveLeft();
						MoveDown();
					}
				} break;
			}
		}
	}
	
	if (range)
		range->set(row_offset);
	update();
	return capture;
}



void SampleInstrumentTable::InsertBlank() {



	if (cursor_x>1) instrument->set_sample_number(cursor_y,Note::EMPTY);
/*
	for (i=NOTE_NOTES-1;i>cursor_y;i--) {

		song->set_order(i,song->get_order(i-1));
	}
	song->set_order(cursor_y,255);
*/
}

void SampleInstrumentTable::DeleteRow() {


  /*
	for (i=cursor_y;i<NOTE_NOTES-1;i++) {

		song->set_order(i,song->get_order(i+1));
	}
	song->set_order(NOTE_NOTES-1,255);
*/
}

void SampleInstrumentTable::InsertRepeat() {

//	song->set_order(cursor_y,254);
	MoveDown();
}


void SampleInstrumentTable::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	draw_single_mode();
}	


void SampleInstrumentTable::set_instrument(Instrument *p_instrument) {
	
	instrument=p_instrument;
	update();
}

void SampleInstrumentTable::set_row_offset(double p_ofs) {
	
	int old_row_offset=row_offset;
	row_offset=(int)p_ofs;
	
	cursor_y+=row_offset-old_row_offset;
	update();
}
void SampleInstrumentTable::set_range(RangeBase *p_range) {
	
	range=p_range;
	if (range) {
		range->set_max(Note::NOTES-1);
		range->set(row_offset);
		range->value_changed_signal.connect(this,&SampleInstrumentTable::set_row_offset);
	}
}
SampleInstrumentTable::SampleInstrumentTable(Editor *p_editor)  {

	set_bg_on_updates( false );	
	row_offset=0;
	cursor_x=0;
	cursor_y=0;
	editor=p_editor;
	instrument=NULL;
	range=0;

	set_focus_mode(FOCUS_ALL);
	//set_
	
	//QToolTip::add( this, "Shorcuts:\n\n'.' - Clear field\nR - Repeat last field.\nAlt+Q Raise all notes a semitone.\nAlt+A Lower all notes a semitone.\nAlt+S Set all samples to last entered one.");

}

SampleInstrumentTable::~SampleInstrumentTable(){
}
