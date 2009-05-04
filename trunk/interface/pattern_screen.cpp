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

#include "pattern_screen.h"
#include "ctskin.h"
#include "tracker/key_bindings.h"

#include <stdio.h>
void PatternScreen::update_timer_callback() {

	pattern_editor_rows->check_play_cursor();
	orderlist_editor->check_playing_order();
	//pattern_editor_oscs->update();
	if (editor->follow_to_current_pos()) {
		
		pattern_editor->update();
		pattern_changed();
		pattern_editor_moved();
		
	}
}

void PatternScreen::focus_pattern_edit_widget() {

	//

}

void PatternScreen::goto_pattern() {
	
	pattern_editor->get_focus();
}
void PatternScreen::goto_orderlist() {
	
	if(!orderlist_vbox->is_visible())
		toggle_orderlist_visible();
		   
	orderlist_editor->get_focus();	
}

void PatternScreen::handle_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return;
	if (!p_press)
		return;
	
	switch(kbind) {
		
		case KB_PATTERN_SCREEN_OPTIONS: {
				
			show_settings();
		} break;
		case KB_PATTERN_SCREEN_HIDE_ORDERLIST: {
			
			if(orderlist_vbox->is_visible())
				toggle_orderlist_visible();
			goto_pattern();
			
			
		} break;
		case KB_PATTERN_SCREEN_SONG_FOLLOW: {
			
			editor->set_song_follow( !editor->is_song_follow() );
			
		} break;
		default: {} //dont annoy, gcc
	}
}


/*

void PatternScreen::row_offset_changed_callback(int p_to_which) {

	editor->set_row_offset(p_to_which);
	pattern_edit->update();
}

void PatternScreen::column_offset_changed_callback(int p_to_which) {

	editor->set_column_offset(p_to_which);
	pattern_edit->update();
}
*/
/*
void PatternScreen::timer_callback() {

	if ((editor==NULL) || (song==NULL))
		return;

	if (timer_callback_count==0) //since it's the first time..
		pattern_len_spin->setValue(song->get_pattern(editor->get_current_pattern())->get_length());

	timer_callback_count++;

	if (pattern_spin->value()!=editor->get_current_pattern()) {

		pattern_spin->setValue(editor->get_current_pattern());
		//only if the pattern changed, i can change the length
		pattern_len_spin->setValue(song->get_pattern(editor->get_current_pattern())->get_length());
	}


	if ((timer_callback_count%2)==0) {

		int vis_rows=editor->get_visible_rows();
		int vis_cols=editor->get_visible_columns();
		if (hscroll->value()!=editor->get_column_offset())
			hscroll->setValue( editor->get_column_offset() );

		if (vscroll->pageStep()!=vis_rows) {

			vscroll->setPageStep(vis_rows);
		}
		Pattern *pat = song->get_pattern( editor->get_current_pattern() );

		if ((pat != NULL) && (vscroll->maxValue()!=(pat->get_length()-vis_rows) ))
			vscroll->setMaxValue( pat->get_length()-vis_rows );

		if (vscroll->value()!=editor->get_row_offset())
			vscroll->setValue( editor->get_row_offset() );

		if (hscroll->maxValue()!=(64-vis_cols)) {

			hscroll->setMaxValue(64-vis_cols);
		}
		if (hscroll->pageStep()!=vis_cols) {

			hscroll->setPageStep(vis_cols);
		}

	}

	if ((timer_callback_count%7)==0) {

		int patterns_used=song->get_patterns_used();

		if (display_cache.patterns_used!=patterns_used) {
			char buf[20];
			sprintf(buf,"/%i",song->get_patterns_used()-1);
			display_cache.patterns_used=patterns_used;
			pattern_spin->setSuffix(buf);

		}
	}



	if (editor->orderlist_get_cursor_y()!=order_spin->value()) {

		order_spin->setValue(editor->orderlist_get_cursor_y());
	}

	if ((timer_callback_count%7)==0) {

		int orders_used=song->get_used_orders_amount();

		if (display_cache.orders_used!=orders_used) {
			char buf[20];
			sprintf(buf,"/%i",orders_used-1);
			display_cache.orders_used=orders_used;
			order_spin->setSuffix(buf);
		}
	}

	if ((timer_callback_count%7)==0) {

		int current_instrument=editor->get_instrument_mask_value();
		string instrument_text;
		instrument_text=song->get_instrument(current_instrument)?song->get_instrument(current_instrument)->name:"";

		if ((current_instrument!=(instrument_edit->value()-1)) || (display_cache.last_instrument_text!=instrument_text)) {
			instrument_edit->setValue(current_instrument+1);
			display_cache.last_instrument_text=instrument_text;
			instrument_text+=" - ";

			instrument_edit->setPrefix(instrument_text.c_str());
		}
	}
}

void PatternScreen::create_undo_list() {

	if (!editor->has_undo_display_pending())
		return;

	undo_history->clear();
	char buf[100];

	for (int i=0;i<editor->get_undo_max();i++) {

		sprintf(buf,"%i- %s",editor->get_undo_pattern(i),editor->get_undo_info(i).c_str());
		undo_history->insertItem( buf, i );
	}

	undo_history->setCurrentItem(0);
	editor->reset_pending_undo_display();
}


void PatternScreen::set_pattern_len() {

	CSpinDialog * ask_value = new CSpinDialog(this);
	ask_value->setCaption("Resize pattern");
	ask_value->set_description("Resize the next:");
	ask_value->get_spin()->setMaxValue(200);
	ask_value->get_spin()->setSuffix(" patterns");
	ask_value->get_spin()->setValue(0);

	if (ask_value->exec() == QDialog::Accepted) {

		int resize_count=ask_value->get_spin()->value()+1;

		Pattern *pat;
		for (int i=0;i<resize_count;i++) {
			pat=song->get_pattern(editor->get_current_pattern()+i);
			if (pat==NULL)
				break;
			pat->set_length(pattern_len_spin->value());
			pattern_edit->update();
		}
	}

	delete ask_value;


}

void PatternScreen::undo_button_callback() {

	
	editor->undo_index(undo_history->currentItem());
	create_undo_list();
	pattern_edit->update();

}
*/
void PatternScreen::pattern_spin_changed_cbk(double p_to_which) {

	editor->set_current_pattern((int)pattern->get_range()->get());
	editor->push_current_pattern_to_undo_list(Editor::PATTERN_OP_SET_PATTERN);

	//create_undo_list();
	pattern_editor->update();
}

void PatternScreen::pattern_editor_resized() {
	
	pattern_h_scroll->get_range()->set_page( editor->get_visible_columns() );
	pattern_v_scroll->get_range()->set_page( editor->get_visible_rows() );
	

}
void PatternScreen::pattern_editor_moved() {
	
	updating_bars=true;
	
	pattern_h_scroll->get_range()->set( editor->get_column_offset() );
	pattern_v_scroll->get_range()->set( editor->get_row_offset() );
	updating_bars=false;
	
	pattern_editor_rows->update();
	pattern_editor_top->update();
	
}

void PatternScreen::pattern_changed() {

	if (updating_pattern)
		return;

	
	Pattern *p=editor->get_song()->get_pattern( editor->get_current_pattern() );
	if (!p)
		return;
	updating_pattern=true;
	pattern_h_scroll->get_range()->set_max(  Pattern::WIDTH );
	pattern_v_scroll->get_range()->set_max( p->get_length() );
	pattern->get_range()->set( editor->get_current_pattern() );
	pattern_editor_rows->update();
	orderlist_editor->update();
	updating_pattern=false;
	
}

void PatternScreen::pattern_h_pos_changed(double p_to_which) {
	
	if (updating_bars)
		return;
	editor->set_column_offset( (int)p_to_which );
	pattern_editor->update();
	pattern_editor_top->update();
}
void PatternScreen::pattern_v_pos_changed(double p_to_which) {
	
	if (updating_bars)
		return;
	editor->set_row_offset( (int)p_to_which );
	pattern_editor->update();
	pattern_editor_rows->update();
	
	
}

void PatternScreen::pattern_spin_changed(double p_to_which) {

	if (updating_pattern)
		return;

	editor->set_current_pattern( (int)p_to_which );
	pattern_editor->update();
	pattern_editor_rows->update();
	pattern_editor_top->update();
	Pattern *p=editor->get_song()->get_pattern( editor->get_current_pattern() );
	if (!p)
		return;
	
	pattern_v_scroll->get_range()->set_max( p->get_length() );
	

}

void PatternScreen::pattern_editor_update() {

	pattern_editor->update();
}
void PatternScreen::update_components() { //needed from outside :(

	//create_undo_list();
	pattern_editor->update();
	pattern_editor_top->update();
	pattern_editor_rows->update();
	orderlist_editor->update();
	pattern_changed();
}

void PatternScreen::update_undo_list() {
	
	settings.undo_list->clear();
	
	for (int i=0;i<editor->get_undo_max();i++) {
		
		String undo_text;
		switch( editor->get_undo_info( i ) ) {
			
			case Editor::PATTERN_NONE: undo_text=("None"); break;
			case Editor::PATTERN_OP_SET_PATTERN: undo_text=(String::num( editor->get_undo_pattern(i))+" Pattern"); break;
			case Editor::PATTERN_RAISE_NOTES_ON_SELECTION: undo_text=(String::num( editor->get_undo_pattern(i))+" Raise"); break;
			case Editor::PATTERN_LOWER_NOTES_ON_SELECTION: undo_text=(String::num( editor->get_undo_pattern(i))+" Lower"); break;
			case Editor::PATTERN_SELECTION_ZAP: undo_text=(String::num( editor->get_undo_pattern(i))+" Selection Zap"); break;
			case Editor::PATTERN_SELECTION_RAMP_PARAMETERS: undo_text=(String::num( editor->get_undo_pattern(i))+" Ramp Params"); break;
			case Editor::PATTERN_SELECTION_RAMP_VOLUMES: undo_text=(String::num( editor->get_undo_pattern(i))+" Ramp Volumes"); break;
			case Editor::PATTERN_SELECTION_PASTE_OVERWRITE: undo_text=(String::num( editor->get_undo_pattern(i))+" Paste (Over)"); break;
			case Editor::PATTERN_SELECTION_PASTE_MIX: undo_text=(String::num( editor->get_undo_pattern(i))+" Paste (Mix)"); break;
			case Editor::PATTERN_SELECTION_PASTE_INSERT: undo_text=(String::num( editor->get_undo_pattern(i))+" Paste"); break;
			case Editor::PATTERN_SELECTION_VOLUME_SCALE: undo_text=(String::num( editor->get_undo_pattern(i))+" Volume Scale"); break;
			case Editor::PATTERN_SELECTION_SET_INSTRUMENT_MASK: undo_text=(String::num( editor->get_undo_pattern(i))+" Set Mask(Ins)"); break;
			case Editor::PATTERN_SELECTION_SET_VOLUME_MASK: undo_text=(String::num( editor->get_undo_pattern(i))+" Set Mask(Vol)"); break;
			case Editor::PATTERN_SELECTION_EXPAND_X2: undo_text=(String::num( editor->get_undo_pattern(i))+" Expand x2"); break;
			case Editor::PATTERN_SELECTION_SHRINK_X2: undo_text=(String::num( editor->get_undo_pattern(i))+" Shrink x2"); break;
			case Editor::PATTERN_SELECTION_WIPE_STRAY_VOLUMES: undo_text=(String::num( editor->get_undo_pattern(i))+" Wipe Stray Vol"); break;
			case Editor::PATTERN_SELECTION_TRANSPOSE: undo_text=(String::num( editor->get_undo_pattern(i))+" Reassign Notes"); break;
			case Editor::PATTERN_SELECTION_TIME_SCALE: undo_text=(String::num( editor->get_undo_pattern(i))+" Time Scale"); break;
			
		}
		

		if (editor->get_undo_is_redo(i))
			undo_text="UNDO: "+undo_text;;
		undo_text=String::num(i+1)+"- "+undo_text;
		settings.undo_list->add_string(undo_text);
		
	}
	
}
void PatternScreen::show_settings() {

	Point ofs=pattern_editor->get_global_pos();
	ofs+=(pattern_editor->get_size()-settings.vb->get_minimum_size())/2;
	settings.popup->set_pos(ofs);
	settings.popup->set_size( settings.vb->get_minimum_size() );
	settings.popup->show();

	
	settings_editing_updating=true;
		
	settings.cursor_stepping->get_range()->set( editor->get_cursor_spacing() );
	
	settings.row_hl_major->get_range()->set( editor->get_song()->get_row_highlight_major());
	settings.row_hl_minor->get_range()->set( editor->get_song()->get_row_highlight_minor());
	
	settings.mask_volume->set_pressed( editor->get_volume_mask() );
	settings.mask_instrument->set_pressed( editor->get_instrument_mask() );
	settings.mask_command->set_pressed( editor->get_command_mask() );
	
	settings.cursor_pattern_len->get_range()->set( editor->get_song()->get_pattern(editor->get_current_pattern())->get_length()  );
	settings.cursor_pattern_len_rep->get_range()->set( 0 );
	
	settings_editing_updating=false;
	
}

void PatternScreen::settings_editing_changed_v(double p_v) {
	
	if (settings_editing_updating)
		return;
	apply_settings_editing();
}

void PatternScreen::apply_settings_editing() {
	
	if (settings_editing_updating)
		return;
	
	editor->get_song()->set_row_highlight_major((int)settings.row_hl_major->get_range()->get());
	editor->get_song()->set_row_highlight_minor((int)settings.row_hl_minor->get_range()->get());
	
	editor->set_volume_mask( settings.mask_volume->is_pressed() );
	editor->set_instrument_mask( settings.mask_instrument->is_pressed() );
	editor->set_command_mask( settings.mask_command->is_pressed() );

	pattern_editor->update();
	pattern_changed();	
}
void PatternScreen::apply_settings_pattern() {
	
	
	int howmany=(int)settings.cursor_pattern_len_rep->get_range()->get();
	int tgt_rows=(int)settings.cursor_pattern_len->get_range()->get();
	
	for (int i=0;i<=howmany;i++) {
		
		int pidx=editor->get_current_pattern()+i;
		if (pidx>=Song::MAX_PATTERNS)
			break;
		editor->get_song()->get_pattern( pidx )->set_length( tgt_rows );
		
		
	}
	
	editor->adjust_view_to_cursor();
	settings.popup->hide();
	
	pattern_editor->update();
	pattern_editor_moved();	
	pattern_changed();
	
	
}

void PatternScreen::undo_editing() {

	editor->undo_index( settings.undo_list->get_selected() );
	settings.popup->hide();
	pattern_editor->update();
	pattern_changed();	
	update_undo_list();

	
}

void PatternScreen::set_in_window() {

	settings.popup=new Window(get_window(),Window::MODE_POPUP);
	settings.vb = new WindowBox("Editing Options");
	 
	settings.popup->set_root_frame( settings.vb );
	//settings.cursor_stepping=settings.vb->add( new MarginGroup, 0 )->add( new SpinBox , 0);
	//settings.cursor_stepping->set_max(16);
	
	settings.cursor_stepping=settings.vb->add( new MarginGroup("Cursor Step"), 0 )->add( new SpinBox , 0);
	settings.cursor_stepping->get_range()->set_max(16);
	settings.cursor_stepping->get_range()->value_changed_signal.connect(this,&PatternScreen::settings_editing_changed_v);
	
	settings.row_hl_minor=settings.vb->add( new MarginGroup("Row Hilight Minor"), 0 )->add( new SpinBox , 0);
	settings.row_hl_minor->get_range()->value_changed_signal.connect(this,&PatternScreen::settings_editing_changed_v);
	settings.row_hl_minor->get_range()->set_max(32);
	settings.row_hl_major=settings.vb->add( new MarginGroup("Row Hilight Major"), 0 )->add( new SpinBox , 0);
	settings.row_hl_major->get_range()->set_max(128);
	settings.row_hl_major->get_range()->value_changed_signal.connect(this,&PatternScreen::settings_editing_changed_v);
	
	HBoxContainer *masks=settings.vb->add( new MarginGroup("Field Masks"), 0 )->add( new HBoxContainer , 0);
	settings.mask_volume=masks->add( new Button("Ins"), 1 );
	settings.mask_volume->set_toggle_mode(true);
	settings.mask_volume->pressed_signal.connect(this,&PatternScreen::apply_settings_editing);
	settings.mask_instrument=masks->add( new Button("Vol"), 1 );
	settings.mask_instrument->pressed_signal.connect(this,&PatternScreen::apply_settings_editing);
	
	settings.mask_instrument->set_toggle_mode(true);
	settings.mask_command=masks->add( new Button("Cmd"), 1 );
	settings.mask_command->set_toggle_mode(true);
	settings.mask_command->pressed_signal.connect(this,&PatternScreen::apply_settings_editing);

	/***/
	settings.vb->add(new Separator(HORIZONTAL),0);

	settings.cursor_pattern_len=settings.vb->add( new MarginGroup("Pattern Length"), 0 )->add( new SpinBox , 0);
	settings.cursor_pattern_len_rep=settings.vb->add( new MarginGroup("Apply Next (Patterns)"), 0 )->add( new SpinBox , 0);
	
	settings.cursor_pattern_len->get_range()->set_min(16);
	settings.cursor_pattern_len->get_range()->set_max(256);
	settings.cursor_pattern_len_rep->get_range()->set_max(200);

	HBoxContainer *apply=settings.vb->add( new HBoxContainer, 0 );
	apply->add( new Widget, 1);
	Button *apply_b=apply->add( new Button("Apply"), 0 );
	apply->add( new Widget, 1);

	apply_b->pressed_signal.connect( this, &PatternScreen::apply_settings_pattern);
	
	
	if (mini) {
		
		settings.vb->add(new Separator(HORIZONTAL),0);
		
		HBoxContainer *undo_hb=settings.vb->add(new HBoxContainer);
		settings.undo_list=undo_hb->add(new ComboBox,2);
		
		Button *apply_b=undo_hb->add( new Button("Undo"), 0 );
		
		
		apply_b->pressed_signal.connect( this, &PatternScreen::undo_editing);
	}
	
	
}

void PatternScreen::pattern_scroll_dir(Direction p_scr) {
	
	if (p_scr==UP) 
		pattern_v_scroll->get_range()->set( pattern_v_scroll->get_range()->get() - editor->get_song()->get_row_highlight_minor() );
	else
		pattern_v_scroll->get_range()->set( pattern_v_scroll->get_range()->get() + editor->get_song()->get_row_highlight_minor() );
	
}


void PatternScreen::toggle_orderlist_visible() {
	
	if (orderlist_vbox->is_visible()) {
		
		orderlist_vbox->hide();
		orderlist_show->set_dir(LEFT);
	} else {
		
		orderlist_vbox->show();
		orderlist_show->set_dir(RIGHT);
		
	}
}
PatternScreen::PatternScreen(Tracker *p_tracker,bool p_mini)  {

	mini=p_mini;
	editor=p_tracker->editor;
	
	HBoxContainer *top_hb=add( new HBoxContainer, 0);
	
	top_hb->add( new Label("Pattern:"), 0 );
	pattern = top_hb->add( new SpinBox , 1 );
	pattern->get_range()->value_changed_signal.connect( this, &PatternScreen::pattern_spin_changed );
	pattern->get_range()->set_max( Song::MAX_PATTERNS -1 );
	
	
	if (!p_mini) {
		top_hb->add( new VSeparator,0);
		top_hb->add( new Label("Operations:"), 0 );
		settings.undo_list=top_hb->add(new ComboBox,2);
		
		Button *apply_b=top_hb->add( new Button("Undo"), 0 );
		
		
		apply_b->pressed_signal.connect( this, &PatternScreen::undo_editing);
	}

	top_hb->add( new VSeparator,0);
	top_hb->add( new Button("Options"), 0 )->pressed_signal.connect(this,&PatternScreen::show_settings);
	
	
	HBoxContainer *bottom_hb=add( new HBoxContainer, 1);
	
	GridContainer *g = bottom_hb->add( new GridContainer(3), 1 );

	
	g->add( new Widget, false, false ); //topleft
	pattern_editor_top=g->add( new PatternEditorTop(editor), true, false ); //top
	orderlist_show = g->add( new ArrowButton(RIGHT,true), false, false ); //right
	orderlist_show->pressed_signal.connect(this,&PatternScreen::toggle_orderlist_visible);
	
	pattern_editor_top->channel_mute_toggled.connect( this, &PatternScreen::pattern_editor_update );
	
//	g->add( new Widget, false, false ); //topleft
	//pattern_editor_oscs=g->add( new PatternEditorOscs(p_tracker),true,false);
	//g->add( new Widget, false, false ); //topright
	
	pattern_editor_rows=g->add( new PatternEditorRows(editor), false, true ); //top 
	
	pattern_editor = g->add( new PatternEditor(editor), true, true );
	
	pattern_editor->display_pos_changed.connect( this, &PatternScreen::pattern_editor_moved );
	pattern_editor->new_undoable_operation.connect( this, &PatternScreen::update_undo_list );
	pattern_editor->size_changed.connect( this, &PatternScreen::pattern_editor_resized );
	pattern_editor->pattern_changed.connect( this, &PatternScreen::pattern_changed );
	
	pattern_editor->scroll_attempt.connect( this, &PatternScreen::pattern_scroll_dir );
	
	pattern_v_scroll = g->add( new VScrollBar, false, true );
	g->add( new Widget, false, false ); //bottomleft
	pattern_v_scroll->get_range()->value_changed_signal.connect( this, &PatternScreen::pattern_v_pos_changed);
	
	pattern_h_scroll = g->add( new HScrollBar, false, false );
	pattern_h_scroll->get_range()->value_changed_signal.connect( this, &PatternScreen::pattern_h_pos_changed);
	
	orderlist_vbox = bottom_hb->add( new VBoxContainer, 0);
	
	orderlist_vbox->add(new Label("Orders:"));
	HBoxContainer *olhb = orderlist_vbox->add( new HBoxContainer, 1 );
	
	orderlist_editor = olhb->add( new OrderListEditor(editor), 1 );
	orderlist_scroll = olhb->add( new VScrollBar, 0 );
	orderlist_editor->set_range( orderlist_scroll->get_range() );
	orderlist_editor->current_pattern_changed.connect(this,&PatternScreen::update_components);

	orderlist_vbox->add( new Label, 0 ); //decoration :)
	
	updating_bars=false;
	updating_pattern=false;
	pattern_changed();

	settings.popup=0;
	settings_editing_updating=false;

	/* Settings Screen */
}

PatternScreen::~PatternScreen() {


	if (settings.popup)
		delete settings.popup;
}
