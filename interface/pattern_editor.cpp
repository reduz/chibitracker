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


#include "bundles/window_box.h"
#include "widgets/line_edit.h"
#include "pattern_editor.h"
#include "ctskin.h"
#include "tracker/key_bindings.h"
#include "base/painter.h"
#include <stdio.h>

//int PatternEditor::volume_scale_memory=100; //default val



void PatternEditor::screen_to_pattern_coords(int &x,int &y,int &field, bool &outside) {

		outside=false;


		if (y<0) {
			outside=true;
		}

		y/=get_row_height();
		if (y>=editor->get_visible_rows()) {
			outside=true;
		}

		y+=editor->get_row_offset();


		if (x<0) {
			outside=true;
			x-=get_column_width(); //so division is correct on negative column
		}
		field=x % get_column_width();
		x/=get_column_width();
		if (x>=editor->get_visible_columns()) {
			outside=true;
		}
		x+=editor->get_column_offset();

		Pattern *pat=song->get_pattern( editor->get_current_pattern() );
		if (!pat) {
			return;
		}

		if (y<0) y=0;

		if (y>=pat->get_length()) y=pat->get_length()-1;

		if (x<0) x=0;

		if (x>=Pattern::WIDTH) x=Pattern::WIDTH-1;

		field/=get_font_width();

		switch (field) {

			case 1:
				case 0: field=0; break;
				case 2: field=1; break;
			case 3:
				case 4: field=2; break;
				case 5: field=3; break;
			case 6:
				case 7: field=4; break;
				case 8: field=5; break;
			case 9:
				case 10: field=6; break;
				case 11: field=7; break;
				case 12: field=8; break;
				default: field=8; break;
		};

}

void PatternEditor::resize(const Size& p_new_size) {

	size=p_new_size;
	adjust_editor();
	size_changed.call();
}

void PatternEditor::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	if (p_press) {
		if (p_button==BUTTON_WHEEL_UP) {
			
			scroll_attempt.call(UP);
			return;
		} else if (p_button==BUTTON_WHEEL_DOWN) {
	
			scroll_attempt.call(DOWN);
			return;
		}
	}					
		
	if (!p_press) {

		if (p_button!=BUTTON_LEFT)
			return;
		
		if (!get_row_height() || !get_column_width() || !song->get_pattern( editor->get_current_pattern() )) {

			return;

		}


		if (mouse_select.begun) {
			editor->selection_release();
			update();

		}

		mouse_select.active=false;
		mouse_select.begun=false;
		return;

	}
	
	if (p_button!=BUTTON_LEFT)
		   return;

	int x,y,field;
	if (!get_row_height() || !get_column_width() || !song->get_pattern( editor->get_current_pattern() )) {

		return;

	}
	
	x=p_pos.x;
	y=p_pos.y;

	bool outside=false;
	field=0;

	screen_to_pattern_coords(x,y,field,outside);


	if (!outside) {
		editor->set_cursor_field(field);
		editor->set_cursor_y(y);
		editor->set_cursor_x(x);
		editor->adjust_view_to_cursor();		

		

		mouse_select.begun=true;
		mouse_select.begin_x=x;
		mouse_select.begin_y=y;
		update();

	}

}

void PatternEditor::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {


	if (!get_row_height() || !get_column_width() || !song->get_pattern( editor->get_current_pattern() )) {

		return;

	}


	if (mouse_select.begun || mouse_select.active) {



		if (mouse_select.begun) {

			mouse_select.begun=false;
			mouse_select.active=true;
			editor->set_cursor_y(mouse_select.begin_y);
			editor->set_cursor_x(mouse_select.begin_x);
			editor->selection_begin_at_cursor();
		}

		int x=p_pos.x;
		int y=p_pos.y;
		int field=0;
		bool outside=false;

		screen_to_pattern_coords(x,y,field,outside);

		editor->set_cursor_field(field);
		editor->set_cursor_y(y);
		editor->set_cursor_x(x);

		editor->selection_end_at_cursor();
		editor->adjust_view_to_cursor();

		update();
		display_pos_changed.call();
	}


}

void PatternEditor::transposed_callback() {
	
	update();
}
bool PatternEditor::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {

	if (!p_press)
		return false;
	
	
	unsigned int scode=p_scan_code;
		
	
	
	if (		p_scan_code!=KeyBind::get(KB_CURSOR_MOVE_UP) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_MOVE_DOWN) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_MOVE_LEFT) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_MOVE_RIGHT) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_PAGE_UP) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_PAGE_DOWN) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_HOME) &&
		       p_scan_code!=KeyBind::get(KB_CURSOR_END) ) {
		
		scode|=p_modifier_mask;		

	} else {
		
		if (p_modifier_mask&KEY_MASK_ALT)
			scode|=KEY_MASK_ALT;
		
		if (p_modifier_mask&KEY_MASK_CTRL)
			scode|=KEY_MASK_CTRL;
		
		if (p_modifier_mask&KEY_MASK_META)
			scode|=KEY_MASK_META;
			
	}
		       
	bool shift=p_modifier_mask&KEY_MASK_SHIFT;
	if (p_scan_code==KeyBind::get( KB_CURSOR_TAB ))
		shift=false;
	
	
	if (KeyBind::get_bind( scode )==KB_PATTERN_BLOCK_AMPLIFY_VOLUMES) {
		/* amplifier! */
		amp_param_window->show();
		amp_param->get_line_edit()->get_focus();
		amp_param->get_line_edit()->select_all();
		return false;
	} else if (KeyBind::get_bind( scode )==KB_PATTERN_TIME_SCALE) {
		/* timescale! */
		timescale.linear_hide_vb->show();
		timescale.window->show();
		timescale.linear=false;
		return false;
	} else if (KeyBind::get_bind( scode )==KB_PATTERN_TIME_SCALE_LINEAR) {
		/* timescale! */
		timescale.linear_hide_vb->hide();
		timescale.window->show();
		timescale.linear=true;
		return false;
	} else if (KeyBind::get_bind( scode )==KB_PATTERN_TRANSPOSE_MENU) {
		/* transpose! */
		transpose->show();
		return false;
	}

		
	editor->clear_redraw_flags();
	int prev_pos_x=editor->get_column_offset();
	int prev_pos_y=editor->get_row_offset();
	int prev_pattern=editor->get_current_pattern();
	int prev_cursor_y=editor->get_cursor_y();
	int prev_undocount=editor->get_undo_count();
	int prev_insmask=editor->get_instrument_mask_value();
	
	/** KEY PRESS KEY PRESS KEY PRESS KEY PRESS **/
	bool grab=editor->key_press_at_cursor( scode, p_unicode, shift );
	/** KEY PRESS KEY PRESS KEY PRESS KEY PRESS **/
	
	editor->adjust_view_to_cursor();
	
	if (prev_pos_y!=editor->get_row_offset() || prev_pos_x!=editor->get_column_offset() || scode==	KB_PATTERN_MUTE_TOGGLE_CHANNEL || scode==KB_PATTERN_SOLO_TOGGLE_CHANNEL)
		display_pos_changed.call();
	if (editor->get_current_pattern()!=prev_pattern)
		pattern_changed.call();

	if (prev_undocount!=editor->get_undo_count())
		new_undoable_operation.call();
	
	if (editor->flag_redraw_all() || editor->flag_redraw_top())
		update();
	else if (editor->flag_redraw_row()) {
		

		int new_cursor_y=editor->get_cursor_y()-editor->get_row_offset();
		prev_cursor_y-=editor->get_row_offset();
		
		int from=MIN(prev_cursor_y,new_cursor_y)-1;
		int rows=ABS(new_cursor_y-prev_cursor_y)+2;
		
		
		Rect rect( Point( 0, get_row_height()*(from)), Size( size.width, rows*get_row_height()) );
		rect = Rect( Point(), size ).clip(rect);
		if (!rect.no_area())
			update(true,rect);
	}
				
	editor->clear_redraw_flags();
	if (prev_insmask!=editor->get_instrument_mask_value() && editor->get_instrument_mask_value()<Song::MAX_INSTRUMENTS) {
		
		instrument_changed_signal.call( editor->get_instrument_mask_value() );
	}

	
	return grab;
}


int PatternEditor::get_row_height() {


	return get_painter()->get_font_height( font( FONT_PATTERN_EDITOR ))+constant(C_PATTERN_EDITOR_ROW_SEPARATION);
		
}

int PatternEditor::get_column_width() {

	int char_width=get_painter()->get_font_char_width( font( FONT_PATTERN_EDITOR ), ' ' );

	return char_width*13+constant(C_PATTERN_EDITOR_COLUMN_SEPARATION);
}

int PatternEditor::get_font_height() {

	return get_painter()->get_font_height( font( FONT_PATTERN_EDITOR ));
}
int PatternEditor::get_font_width() {

	
	return get_painter()->get_font_char_width( font( FONT_PATTERN_EDITOR ), ' ' );

}


void PatternEditor::adjust_editor() {

	
	editor->set_visible_columns( size.width/get_column_width() );
	editor->set_visible_rows( size.height/get_row_height() );
	editor->adjust_view_to_cursor();


}


void PatternEditor::print_single_row(int rowofs) {

	int i;
        char note_string[19];
	int current_row,y_offset;

	note_string[18]=0;

	if ((rowofs<0) || (rowofs>=editor->get_visible_rows()) ) return;

	current_row=rowofs+editor->get_row_offset();

	y_offset=rowofs*get_row_height();	
	
	Painter *p=get_painter();
	
	if (current_row<0 || current_row>=song->get_pattern( editor->get_current_pattern() )->get_length()) {
		
		p->draw_fill_rect(Point(0,y_offset),Size( size.width,get_row_height()),color(COLOR_PATTERN_EDITOR_BG));
		return;
		
	}
	/* ROW Background - may depend on row hilite */





	/* Draw notes and separator lines. */


	for (i=0;i<editor->get_visible_columns();i++) {

		editor->get_note_string(editor->get_column_offset()+i,current_row,(char*)&note_string);

                int x,y;

		int row_x=i*get_column_width();
		int row_y=y_offset;
		
		x=row_x;
		y=row_y;

		int cw=get_column_width()-constant(C_PATTERN_EDITOR_COLUMN_SEPARATION);



		if ( (editor->is_selection_active())
                     && (editor->get_column_offset()+i>=editor->get_selection_begin_x())
                     && (editor->get_column_offset()+i<=editor->get_selection_end_x())
                     && (editor->get_row_offset()+rowofs>=editor->get_selection_begin_y())
                     && (editor->get_row_offset()+rowofs<=editor->get_selection_end_y())
		    ) {


			if ((current_row % song->get_row_highlight_major())==0) {

				p->draw_fill_rect(Point(x,y),Size(cw,get_row_height()),color(COLOR_PATTERN_EDITOR_ROW_HL_SELECTED));
                        } else {

				p->draw_fill_rect(Point(x,y),Size(cw,get_row_height()),color(COLOR_PATTERN_EDITOR_BG_SELECTED));
				
			}

			p->draw_text( font(FONT_PATTERN_EDITOR), Point(x, y+p->get_font_ascent( font(FONT_PATTERN_EDITOR) )), note_string, color(COLOR_PATTERN_EDITOR_FONT_SELECTED) );
			
		} else 	{

			if (song->get_row_highlight_major() && (current_row % song->get_row_highlight_major())==0) {

				p->draw_fill_rect(Point(x,y),Size(cw,get_row_height()),color(COLOR_PATTERN_EDITOR_ROW_HL_MAJOR));

			} else if (song->get_row_highlight_minor() && (current_row % song->get_row_highlight_minor())==0) {

				p->draw_fill_rect(Point(x,y),Size(cw,get_row_height()),color(COLOR_PATTERN_EDITOR_ROW_HL_MINOR));

			} else {

				p->draw_fill_rect(Point(x,y),Size(cw,get_row_height()),color(COLOR_PATTERN_EDITOR_BG));

			}

			bool mute=song->is_channel_mute( editor->get_column_offset()+ i);
			bool cloned=song->get_pattern( editor->get_current_pattern() )->get_note( editor->get_column_offset()+i, current_row ).cloned;
			
			p->draw_text( font(FONT_PATTERN_EDITOR), Point(x, y+p->get_font_ascent( font(FONT_PATTERN_EDITOR) )), note_string, color(mute?COLOR_PATTERN_EDITOR_FONT_MUTE:(cloned?COLOR_PATTERN_EDITOR_FONT_CLONED:COLOR_PATTERN_EDITOR_FONT)) );

		}

		p->draw_fill_rect(Point((x+cw),y),Size( constant(C_PATTERN_EDITOR_COLUMN_SEPARATION),get_row_height()),color(COLOR_PATTERN_EDITOR_SEPARATOR));
		
		if (i==(editor->get_visible_columns()-1)) { //paint separator} else { //paint end


			p->draw_fill_rect(Point(x+get_column_width(),y),Size( size.width-(x+get_column_width()),get_row_height()),color(COLOR_PATTERN_EDITOR_BG));
			
		}


	}
}



void PatternEditor::print_rows(const Rect &p_repaint_area) {

	int i;

	for (i=0;i<editor->get_visible_rows();i++) {
		
		Rect row_rect( Point( 0, i*get_row_height()), Size( size.width, get_row_height() ));
		
		if (!row_rect.intersects_with( p_repaint_area ))	
			continue;
		
		print_single_row(i);
		
	}
	//feel remaining thing
	get_painter()->draw_fill_rect(Point(0,(editor->get_visible_rows())*get_row_height()),Size(size.width,size.height-((editor->get_visible_rows())*get_row_height())),color(COLOR_PATTERN_EDITOR_BG),p_repaint_area);
	for (int i=0;i<editor->get_visible_columns();i++) {
	
                int x,y;

		int row_x=i*get_column_width();
		int row_y=editor->get_visible_rows()*get_row_height();
		
		x=row_x;
		y=row_y;

		int cw=get_column_width()-constant(C_PATTERN_EDITOR_COLUMN_SEPARATION);
	
		get_painter()->draw_fill_rect(Point((x+cw),y),Size( constant(C_PATTERN_EDITOR_COLUMN_SEPARATION),get_row_height()),color(COLOR_PATTERN_EDITOR_SEPARATOR),p_repaint_area);
	
	}
}

void PatternEditor::print_cursor() {

	int xinit,yinit;

        if (!has_focus()) return; //no focus, no cursor
	const char cursor_spacing[] = {

		0 , //note
		2 , //octave
		4 , //instrument 1
		5 , //instrument 2
		7 , //volume 1
		8 , //volume 2
		10, //command
		11, //parameter 1
		12, //parameter 2
	};

	xinit=(editor->get_cursor_x()-editor->get_column_offset())*get_column_width()+cursor_spacing[editor->get_cursor_field()]*get_font_width();
 	yinit=(editor->get_cursor_y()-editor->get_row_offset())*get_row_height();
	
	get_painter()->draw_stylebox(stylebox( SB_PATTERN_EDITOR_CURSOR), Point(xinit-2, yinit-2),Size(get_font_width()+4,get_font_height()+3));
}


void PatternEditor::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	
	if ((editor==NULL) || (song==NULL))
		return;

	
//	printf("Exposed: %i,%i - %i,%i\n",p_exposed.pos.x,p_exposed.pos.y,p_exposed.size.x,p_exposed.size.y);
	adjust_editor();
	print_rows(p_exposed);
	print_cursor();
	if (has_focus())
		get_painter()->draw_rect( Point(), p_size, color(COLOR_PATTERN_EDITOR_FOCUS));

}


Size PatternEditor::get_size() {

	return size;
}

void PatternEditor::apply_amp() {
	
	amp_param_window->hide();
	editor->selection_volume_scale((int)amp_param->get_range()->get());
	update();
	new_undoable_operation.call();
}

void PatternEditor::apply_amp_t(String meh) {
	
	apply_amp();
}

void PatternEditor::timescale_apply() {
	
	
	timescale.window->hide();
	
	if (timescale.linear) {
		
		editor->selection_time_scale(timescale.total_stretch->get_range()->get(),timescale.total_stretch->get_range()->get(),timescale.total_stretch->get_range()->get(),(int)timescale.offset->get_range()->get());
		
	} else {
		
		editor->selection_time_scale(timescale.initial_stretch->get_range()->get(),timescale.final_stretch->get_range()->get(),timescale.total_stretch->get_range()->get(),(int)timescale.offset->get_range()->get());
		
	}
	
	update();
	new_undoable_operation.call();
	
}

void PatternEditor::set_in_window() {
	
	amp_param_window = new Window(get_window(),Window::MODE_POPUP,Window::SIZE_CENTER);
	WindowBox *vbc = new WindowBox("Volume Scale");
	amp_param_window->set_root_frame( vbc );
	amp_group=vbc->add( new MarginGroup("Amplify by (%):"),0);
	amp_param=amp_group->add( new SpinBox, 0);
	amp_param->get_range()->set_max(500);
	amp_param->get_range()->set_min(0);
	amp_param->get_range()->set(100);
	vbc->add(new CenterContainer)->set( new Button("Apply"))->pressed_signal.connect(this,&PatternEditor::apply_amp);
	amp_param->get_line_edit()->text_enter_signal.connect(this,&PatternEditor::apply_amp_t);
	amp_param->get_line_edit()->set_max_length( 3 );
	
		
	timescale.window = new Window(get_window(),Window::MODE_POPUP,Window::SIZE_CENTER);

	vbc = new WindowBox("Time Scale");
	timescale.window->set_root_frame( vbc );
	
	timescale.linear_hide_vb = vbc->add(new VBoxContainer);
	timescale.linear_hide_vb->add(new MarginGroup("Initial Stretch"))->add((timescale.initial_stretch=new SpinBox))->get_range()->config(0.01,100,1,0.01);
	timescale.linear_hide_vb->add(new MarginGroup("Final Stretch"))->add((timescale.final_stretch=new SpinBox))->get_range()->config(0.01,100,1,0.01);
	vbc->add(new MarginGroup("Total Stretch"))->add((timescale.total_stretch=new SpinBox))->get_range()->config(0.01,100,1,0.01);
	vbc->add(new MarginGroup("Offset"))->add((timescale.offset=new SpinBox))->get_range()->config(-1000,1000,0,1);
	
	vbc->add(new CenterContainer)->set( new Button("Scale"))->pressed_signal.connect(this,&PatternEditor::timescale_apply);
	
	transpose = new PatternTranspose(get_window(),editor);
	transpose->update_pattern.connect(this,&PatternEditor::transposed_callback);
}

PatternEditor::PatternEditor(Editor *p_editor) {

	song=p_editor->get_song();
	editor=p_editor;
	old_row_to_draw=-1;

//	setBackgroundMode (NoBackground);
	previous_shift=false;
	set_focus_mode(FOCUS_ALL);

	mouse_select.begin_y=0;
	mouse_select.begin_x=0;
	mouse_select.begun=false;
	mouse_select.active=false;

	set_bg_on_updates(false);
}

PatternEditor::~PatternEditor()
{
}
