//
// C++ Implementation: editor_pattern_keypress
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "editor.h"
#include "key_bindings.h"
#include <stdio.h>
bool Editor::handle_scancode(unsigned int chr,bool p_shift) {


	int previous_cursor_x,previous_cursor_y;
	int aux_instrument;
	bool grab_event=true;
	bool must_repaint=false;


	if (p_shift && !previous_shift) shift_selection_begin();
	if (!p_shift && previous_shift) shift_selection_end();

	previous_shift=p_shift;
		
	aux_instrument=get_instrument_mask_value();

	
	KeyBindList kbind=KeyBind::get_bind( chr );
	
	if (kbind==KB_MAX)
		return false;
	
	switch(kbind) {
		
		
		case KB_CURSOR_MOVE_UP: {
			
			cursor_move_up();
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_MOVE_DOWN: {
			
			cursor_move_down();
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_MOVE_UP_1_ROW: {
			
			cursor_move_up(1);
			
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_MOVE_DOWN_1_ROW: {
			
			cursor_move_down(1);
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_MOVE_LEFT: {
			
			cursor_move_field_left();
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_MOVE_RIGHT: {
			
			cursor_move_field_right();
			must_repaint=true;
			
		} break;
		case KB_CURSOR_PAGE_UP: {
			
			cursor_page_up();
			must_repaint=true;
			
		} break;
		case KB_CURSOR_PAGE_DOWN: {
			
			cursor_page_down();
			must_repaint=true;
			
		} break;
		case KB_CURSOR_MOVE_TRACK_LEFT: {
			

			cursor_move_left(1);
			must_repaint=true;
		} break;
		case KB_CURSOR_MOVE_TRACK_RIGHT: {
			

			cursor_move_right(1);
			must_repaint=true;
			
		} break;
		
		case KB_CURSOR_HOME: {
			
			cursor_home();
			must_repaint=true;

		} break;
		case KB_CURSOR_END: {
			
			cursor_end();
			must_repaint=true;
			
		} break;
		case KB_CURSOR_TAB: {
			
			cursor_tab();
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_BACKTAB: {
			
			cursor_back_tab();
			must_repaint=true;
						
			
		} break;
		case KB_CURSOR_INSERT: {
			
			cursor_insert();
			must_repaint=true;
			
		} break;
		case KB_CURSOR_DELETE: {
			
			cursor_delete();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_PAN_WINDOW_UP: {
			
			set_row_offset( get_row_offset() -1 );
		} break;
		case KB_PATTERN_PAN_WINDOW_DOWN: {
			
			set_row_offset( get_row_offset() +1 );
			
		} break;
		
		case KB_PATTERN_MARK_BLOCK_BEGIN: {
			
			mark_block_begining();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_MARK_BLOCK_END: {
			
			mark_block_end();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_MARK_COLUMN_ALL: {
			
			mark_column_all();
			must_repaint=true;
			
			
		} break;
		case KB_PATTERN_BLOCK_COPY: {
			
			selection_copy();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_PASTE_INSERT: {
			
			selection_paste_insert();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_PASTE_OVERWRITE: {
			
			selection_paste_overwrite();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_PASTE_MIX: {
			
			selection_paste_mix();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_UNMARK: {
			
			selection_release();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_CUT: {
			
			selection_zap();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_SET_CURRENT_INSTRUMENT: {
			
			selection_set_instrument_mask();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_SET_CURRENT_VOLUME: {
			
			selection_set_volume_mask();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_WIPE_STRAY_VOLUMES: {
			
			selection_wipe_stray_volumes();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_RAMP_VOLUMES: {
			
			selection_volume_ramp();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_RAMP_WIPE_EFFECTS: {
			
			selection_parameter_ramp();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_DOUBLE_BLOCK_LENGTH: {
			
			selection_expand();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_BLOCK_HALVE_BLOCK_LENGTH: {
			
			selection_shrink();
			must_repaint=true;
			
		} break;
		case KB_CURSOR_WRITE_MASK: {
			
			insert_mask_at_cursor();
			must_repaint=true;
		} break;
		case KB_PATTERN_FIELD_TOGGLE_MASK: {
			
			toggle_mask_at_cursor();
			must_repaint=true;
			
			
		} break;
		case KB_CURSOR_FIELD_CLEAR: {
			
			clear_field_at_cursor();
			must_repaint=true;
			
			
		} break;
		case KB_PATTERN_FIELD_COPY_CURRENT: {
			
			get_mask_from_cursor();
			must_repaint=true;
			
			
		} break;
		case KB_PATTERN_FIELD_WRITE_CURRENT: {
			
			
		} break;
		case KB_PATTERN_RAISE_NOTES: {
			
			selection_raise();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_LOWER_NOTES: {
			
			selection_lower();
			must_repaint=true;
			
		} break;

		case KB_PATTERN_RAISE_12_NOTES: {
			
			selection_raise_octave();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_LOWER_12_NOTES: {
			
			selection_lower_octave();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_UNDO: {
			
			undo_index( 0 );
		} break;
		case KB_PATTERN_NEXT: {
			
			cursor_next_pattern();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_PREV: {
			
			cursor_prev_pattern();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_NEXT_ORDER: {
			
			orderlist_move_down();
			if (song->get_order(orderlist_get_cursor_y())<Song::MAX_PATTERNS) cursor_goto_pattern(song->get_order(orderlist_get_cursor_y()));
			must_repaint=true;
			
		} break;
		case KB_PATTERN_PREV_ORDER: {
			
			orderlist_move_up();
			if (song->get_order(orderlist_get_cursor_y())<Song::MAX_PATTERNS) cursor_goto_pattern(song->get_order(orderlist_get_cursor_y()));
			must_repaint=true;
			
		} break;
		case KB_PATTERN_MUTE_TOGGLE_CHANNEL: {
			
			current_channel_mute_toggle();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_SOLO_TOGGLE_CHANNEL: {
			
			current_channel_solo_toggle();
			must_repaint=true;
			
		} break;
		case KB_PATTERN_NEXT_INSTRUMENT: {
			
			if (get_instrument_mask_value()<Song::MAX_INSTRUMENTS-1) set_instrument_mask_value(get_instrument_mask_value()+1);
			
			
		} break;
		case KB_PATTERN_PREV_INSTRUMENT: {
			
			if (get_instrument_mask_value()>0) set_instrument_mask_value(get_instrument_mask_value()-1);
			
			
		} break;
		case KB_PATTERN_CURSOR_SPACING_0:
		case KB_PATTERN_CURSOR_SPACING_1:
		case KB_PATTERN_CURSOR_SPACING_2:
		case KB_PATTERN_CURSOR_SPACING_3:
		case KB_PATTERN_CURSOR_SPACING_4:
		case KB_PATTERN_CURSOR_SPACING_5:
		case KB_PATTERN_CURSOR_SPACING_6:
		case KB_PATTERN_CURSOR_SPACING_7:
		case KB_PATTERN_CURSOR_SPACING_8:
		case KB_PATTERN_CURSOR_SPACING_9: {
			
			set_cursor_spacing(kbind-KB_PATTERN_CURSOR_SPACING_0);
			
		} break;
		
		default:
			grab_event=false;
			must_repaint=false;
			
	}
		

		/*
		if ((get_cursor_field()==0) && (key_value==kinput->get_key_bind_key_nocase(KEY_PLAY_NOTE_ON_CURSOR))) {

			if (variables_lock) variables_lock->release();
			player->play_note(get_cursor_x(),song->get_pattern(get_current_pattern())->get_note(get_cursor_x(),get_cursor_y()));
			if (variables_lock) variables_lock->grab();

			cursor_move_down();
			must_repaint=true;
			grab_event=true;

		} else if ((get_cursor_field()==0) && (key_value==kinput->get_key_bind_key_nocase(KEY_PLAY_ROW_ON_CURSOR))) {

			int i;
			if (variables_lock) variables_lock->release();
			for (i=0;i<PATTERN_WIDTH;i++) player->play_note(i,song->get_pattern(get_current_pattern())->get_note(i,get_cursor_y()),false);
			if (variables_lock) variables_lock->grab();

			cursor_move_down();


			must_repaint=true;
			grab_event=true;

		} else if ( ((key_value>='0') && (key_value<='9')) || ((key_value>='A') && (key_value<='Z') ))  { //quickhack!

			previous_cursor_x=get_cursor_x();
			previous_cursor_y=get_cursor_y();

			if (press_key_at_cursor(key_value)) {

				if (variables_lock) variables_lock->release();
				if (key_value!='`') player->play_note(previous_cursor_x,song->get_pattern(get_current_pattern())->get_note(previous_cursor_x,previous_cursor_y));
				if (variables_lock) variables_lock->grab();

			}
			must_repaint=true;
			grab_event=true;
	} */
	



	

	return grab_event;

}
