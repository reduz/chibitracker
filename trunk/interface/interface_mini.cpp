//
// C++ Implementation: interface
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bundles/window_box.h"

#include "interface_mini.h"
#include "ctskin.h"
#include <stdio.h>
#include "interface_logos.h"
#include "mini_theme.h"
#include "mixer/sound_driver_manager.h"
void InterfaceMini::update_title_bar() {

	String title="ChibiTracker v"+String::num(VERSION_MAJOR)+"."+String::num(VERSION_MINOR)+String(VERSION_REV);
	if (tracker.song->get_name()[0]!=0) 
		title+=" - " +String(tracker.song->get_name());
	else
		title+=" - (c) 2001-2007 Juan Linietsky";
	
	window->get_painter()->set_display_title(title,"chibi");
}

void InterfaceMini::set_default_keybinds() {


	KeyBind::set(KB_CURSOR_MOVE_UP, KEY_UP);
	KeyBind::set(KB_CURSOR_MOVE_DOWN,KEY_DOWN );
	KeyBind::set(KB_CURSOR_MOVE_LEFT, KEY_LEFT );
	KeyBind::set(KB_CURSOR_MOVE_RIGHT,KEY_RIGHT );
	KeyBind::set(KB_CURSOR_PAGE_UP, KEY_PAGEUP );
	KeyBind::set(KB_CURSOR_PAGE_DOWN, KEY_PAGEDOWN );
	KeyBind::set(KB_CURSOR_MOVE_UP_1_ROW, KEY_MASK_CTRL|KEY_HOME);
	KeyBind::set(KB_CURSOR_MOVE_DOWN_1_ROW, KEY_MASK_CTRL|KEY_END);
	
	KeyBind::set(KB_CURSOR_MOVE_TRACK_LEFT,KEY_MASK_ALT|KEY_LEFT);
	KeyBind::set(KB_CURSOR_MOVE_TRACK_RIGHT,KEY_MASK_ALT|KEY_RIGHT);
	
	KeyBind::set(KB_CURSOR_HOME, KEY_HOME);
	KeyBind::set(KB_CURSOR_END, KEY_END );
	KeyBind::set(KB_CURSOR_INSERT, KEY_INSERT );
	KeyBind::set(KB_CURSOR_DELETE, KEY_DELETE );
	KeyBind::set(KB_CURSOR_TAB, KEY_TAB );
	KeyBind::set(KB_CURSOR_BACKTAB, KEY_MASK_SHIFT|KEY_TAB );
	KeyBind::set(KB_CURSOR_FIELD_CLEAR, KEY_PERIOD);
	
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_0,KEY_MASK_ALT|KEY_0);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_1,KEY_MASK_ALT|KEY_1);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_2,KEY_MASK_ALT|KEY_2);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_3,KEY_MASK_ALT|KEY_3);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_4,KEY_MASK_ALT|KEY_4);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_5,KEY_MASK_ALT|KEY_5);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_6,KEY_MASK_ALT|KEY_6);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_7,KEY_MASK_ALT|KEY_7);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_8,KEY_MASK_ALT|KEY_8);
	KeyBind::set(KB_PATTERN_CURSOR_SPACING_9,KEY_MASK_ALT|KEY_9);
		
	KeyBind::set(KB_PATTERN_CURSOR_INSERT_NOTE_OFF_VOLPAN_TOGGLE,KEY_BACKQUOTE);
	KeyBind::set(KB_PATTERN_CURSOR_INSERT_NOTE_CUT,KEY_1);
	KeyBind::set(KB_PATTERN_CURSOR_PLAY_FIELD, KEY_4);
	KeyBind::set(KB_PATTERN_CURSOR_PLAY_ROW, KEY_8);

	KeyBind::set(KB_PATTERN_MUTE_TOGGLE_CHANNEL,KEY_F9);
	KeyBind::set(KB_PATTERN_SOLO_TOGGLE_CHANNEL,KEY_F10);
	   	
	KeyBind::set(KB_PATTERN_TIME_SCALE,KEY_MASK_SHIFT|KEY_MASK_ALT|KEY_t);
	KeyBind::set(KB_PATTERN_TIME_SCALE_LINEAR,KEY_MASK_ALT|KEY_t);
	
	KeyBind::set(KB_PATTERN_TRANSPOSE_MENU,KEY_MASK_ALT|KEY_r);
		
	KeyBind::set(KB_PATTERN_NEXT_INSTRUMENT,KEY_MASK_SHIFT|KEY_PERIOD);
	KeyBind::set(KB_PATTERN_PREV_INSTRUMENT,KEY_MASK_SHIFT|KEY_COMMA);
	
	KeyBind::set(KB_PATTERN_PAN_WINDOW_UP,KEY_MASK_ALT|KEY_UP);
	KeyBind::set(KB_PATTERN_PAN_WINDOW_DOWN,KEY_MASK_ALT|KEY_DOWN);
	
	KeyBind::set(KB_PATTERN_MARK_BLOCK_BEGIN, KEY_MASK_ALT|KEY_b );
	KeyBind::set(KB_PATTERN_MARK_BLOCK_END, KEY_MASK_ALT|KEY_e);
	KeyBind::set(KB_PATTERN_MARK_COLUMN_ALL, KEY_MASK_ALT|KEY_l );
	KeyBind::set(KB_PATTERN_BLOCK_COPY, KEY_MASK_ALT|KEY_c );
	KeyBind::set(KB_PATTERN_BLOCK_PASTE_INSERT, KEY_MASK_ALT|KEY_p);
	KeyBind::set(KB_PATTERN_BLOCK_PASTE_OVERWRITE, KEY_MASK_ALT|KEY_o);
	KeyBind::set(KB_PATTERN_BLOCK_PASTE_MIX, KEY_MASK_ALT|KEY_m);
	KeyBind::set(KB_PATTERN_BLOCK_UNMARK, KEY_MASK_ALT|KEY_u);
	KeyBind::set(KB_PATTERN_BLOCK_CUT, KEY_MASK_ALT|KEY_z);
	KeyBind::set(KB_PATTERN_BLOCK_SET_CURRENT_INSTRUMENT, KEY_MASK_ALT|KEY_s);
	KeyBind::set(KB_PATTERN_BLOCK_SET_CURRENT_VOLUME, KEY_MASK_ALT|KEY_v);
	KeyBind::set(KB_PATTERN_BLOCK_WIPE_STRAY_VOLUMES, KEY_MASK_ALT|KEY_w);
	KeyBind::set(KB_PATTERN_BLOCK_RAMP_VOLUMES, KEY_MASK_ALT|KEY_k);
	KeyBind::set(KB_PATTERN_BLOCK_AMPLIFY_VOLUMES, KEY_MASK_ALT|KEY_j);
	KeyBind::set(KB_PATTERN_BLOCK_RAMP_WIPE_EFFECTS, KEY_MASK_ALT|KEY_x);
	KeyBind::set(KB_PATTERN_BLOCK_DOUBLE_BLOCK_LENGTH, KEY_MASK_ALT|KEY_f);
	KeyBind::set(KB_PATTERN_BLOCK_HALVE_BLOCK_LENGTH, KEY_MASK_ALT|KEY_g);
	KeyBind::set(KB_PATTERN_FIELD_TOGGLE_MASK, KEY_COMMA);
	KeyBind::set(KB_PATTERN_FIELD_COPY_CURRENT, KEY_RETURN);
	KeyBind::set(KB_PATTERN_FIELD_WRITE_CURRENT, KEY_SPACE );
	KeyBind::set(KB_PATTERN_RAISE_NOTES, KEY_MASK_ALT|KEY_q);
	KeyBind::set(KB_PATTERN_LOWER_NOTES, KEY_MASK_ALT|KEY_a);
	KeyBind::set(KB_PATTERN_RAISE_12_NOTES, KEY_MASK_SHIFT|KEY_MASK_ALT|KEY_q );
	KeyBind::set(KB_PATTERN_LOWER_12_NOTES, KEY_MASK_SHIFT|KEY_MASK_ALT|KEY_a);
	KeyBind::set(KB_PATTERN_NEXT, KEY_KP_PLUS);
	KeyBind::set(KB_PATTERN_PREV, KEY_KP_MINUS);
	KeyBind::set(KB_PATTERN_NEXT_ORDER, KEY_MASK_CTRL|KEY_KP_PLUS);
	KeyBind::set(KB_PATTERN_PREV_ORDER, KEY_MASK_CTRL|KEY_KP_MINUS);
	KeyBind::set(KB_PATTERN_UNDO,KEY_MASK_CTRL|KEY_BACKSPACE);
	KeyBind::set(KB_PIANO_C0, KEY_z);
	KeyBind::set(KB_PIANO_Cs0, KEY_s );
	KeyBind::set(KB_PIANO_D0, KEY_x);
	KeyBind::set(KB_PIANO_Ds0, KEY_d);
	KeyBind::set(KB_PIANO_E0, KEY_c);
	KeyBind::set(KB_PIANO_F0, KEY_v);
	KeyBind::set(KB_PIANO_Fs0, KEY_g );
	KeyBind::set(KB_PIANO_G0, KEY_b);
	KeyBind::set(KB_PIANO_Gs0, KEY_h);
	KeyBind::set(KB_PIANO_A0, KEY_n);
	KeyBind::set(KB_PIANO_As0, KEY_j);
	KeyBind::set(KB_PIANO_B0, KEY_m);
	KeyBind::set(KB_PIANO_C1, KEY_q);
	KeyBind::set(KB_PIANO_Cs1, KEY_2);
	KeyBind::set(KB_PIANO_D1, KEY_w);
	KeyBind::set(KB_PIANO_Ds1, KEY_3);
	KeyBind::set(KB_PIANO_E1, KEY_e);
	KeyBind::set(KB_PIANO_F1, KEY_r);
	KeyBind::set(KB_PIANO_Fs1, KEY_5);
	KeyBind::set(KB_PIANO_G1, KEY_t);
	KeyBind::set(KB_PIANO_Gs1, KEY_6);
	KeyBind::set(KB_PIANO_A1, KEY_y);
	KeyBind::set(KB_PIANO_As1, KEY_7);
	KeyBind::set(KB_PIANO_B1, KEY_u);
	KeyBind::set(KB_PIANO_C2, KEY_i);
	KeyBind::set(KB_PIANO_Cs2, KEY_9);
	KeyBind::set(KB_PIANO_D2, KEY_o);
	KeyBind::set(KB_PIANO_Ds2, KEY_0);
	KeyBind::set(KB_PIANO_E2, KEY_p);
	KeyBind::set(KB_ORDERLIST_INSERT_SEPARATOR,KEY_EQUALS);

	KeyBind::set(KB_CURSOR_WRITE_MASK,KEY_SPACE);
	
	KeyBind::set(KB_GOTO_PATTERN_SCREEN,KEY_F2);
	KeyBind::set(KB_GOTO_PATTERN_SCREEN_ORDERLIST,KEY_F11);
	KeyBind::set(KB_GOTO_SAMPLE_SCREEN,KEY_F3);	
	KeyBind::set(KB_GOTO_INSTRUMENT_SCREEN,KEY_F4);
	KeyBind::set(KB_GOTO_VARIABLES_SCREEN,KEY_F12);	
	KeyBind::set(KB_OCTAVE_RAISE, KEY_KP_MULTIPLY);
	KeyBind::set(KB_OCTAVE_LOWER, KEY_KP_DIVIDE);
	KeyBind::set(KB_PATTERN_SCREEN_SONG_FOLLOW,KEY_MASK_CTRL|KEY_f);
	
	KeyBind::set(KB_PATTERN_SCREEN_HIDE_ORDERLIST,KEY_MASK_SHIFT|KEY_F11);
	KeyBind::set(KB_PATTERN_SCREEN_OPTIONS,KEY_MASK_SHIFT|KEY_F2);
	
	KeyBind::set(KB_PLAYER_PLAY_SONG,KEY_F5);
	KeyBind::set(KB_PLAYER_STOP_SONG,KEY_F8);
	KeyBind::set(KB_PLAYER_FF_SONG,KEY_MASK_ALT|KEY_KP_PLUS);
	KeyBind::set(KB_PLAYER_RW_SONG,KEY_MASK_ALT|KEY_KP_MINUS);
	KeyBind::set(KB_PLAYER_PLAY_PATTERN,KEY_F6);
	KeyBind::set(KB_PLAYER_PLAY_CURSOR,KEY_F7);
	KeyBind::set(KB_PLAYER_PLAY_ORDER,KEY_MASK_SHIFT|KEY_F6);
		
	KeyBind::set(KB_FILE_OPEN,KEY_MASK_CTRL|KEY_o);
	KeyBind::set(KB_FILE_SAVE,KEY_MASK_CTRL|KEY_s);
	KeyBind::set(KB_FILE_SAVE_AS,KEY_MASK_CTRL|KEY_MASK_SHIFT|KEY_s);
	KeyBind::set(KB_DISPLAY_TOGGLE_FULLSCREEN,KEY_MASK_CTRL|KEY_RETURN);
	KeyBind::set(KB_QUIT,KEY_MASK_CTRL|KEY_x);
	
}

void InterfaceMini::interface_update_timer_callback() {
	
	String text;
	
	int voices_in_use=tracker.mixer->get_active_voice_count();
	if (tracker.player->is_playing()) {
		
		if (tracker.player->get_play_mode()==Player::PLAY_PATTERN) {
			
			text="Playing pattern " + String::num( tracker.player->get_current_pattern() ) +", row "+String::num( tracker.player->get_current_row() );
		} else {
			int used_orders=0;
			
			for (int i=0;i<Song::MAX_ORDERS;i++) {
				
				int order=tracker.song->get_order(i);
				if (order>=Song::MAX_PATTERNS && order!=ORDER_BREAK) {
					used_orders=i-1;
					break;
				}
			}
						
			text="Playing order " +String::num(tracker.player->get_current_order()) +"/"+String::num(used_orders)+", pattern " + String::num( tracker.player->get_current_pattern() ) +", row "+String::num( tracker.player->get_current_row());
		}
		
		text+=", " + String::num(voices_in_use) + " Voices.";
	} else if (voices_in_use>0) {
		
		text="Stopped, " + String::num(voices_in_use) + " Voices.";
	} else {
			
		text="Stopped.";
			
	}
	
	/*@TODO SET STATUS TEXT */
//	if (status_override_time<=0 && status_label->get_text()!=text)
		//status_label->set_text( text );
	
	if (status_override_time>0) {
		status_override_time-=window->get_timer()->get_interval( timer );
		if (status_override_time<0)
			status_override_time=0;
			
	}
	
	
	int total_mem=MemPool_Wrapper::get_singleton()->get_total_mem_used();
	String mem_usage;
	
		
	if (total_mem<1024) {
		
		mem_usage=String::num( total_mem )+" bytes";
	} else if (total_mem<(1024*1024)) {
		mem_usage=String::num( total_mem/1024,2 )+" kb";
		
	} else {
		
		mem_usage=String::num( total_mem/(1024.0*1024.0),2 )+" mb";
		
	}
	
	/*@TODO SHOW MEMORY */
//	if (mem_usage!=memory_label->get_text())
//		memory_label->set_text( mem_usage );
	
	if (tabs->get_selected_tab()==SCREEN_PATTERN)
		pattern_screen->update_timer_callback();
	if (tabs->get_selected_tab()==SCREEN_SAMPLES)
		sample_screen->update_timer_callback();
	if (tabs->get_selected_tab()==SCREEN_INSTRUMENTS)
		instrument_screen->update_timer_callback();
	
	sound_driver_dialog->check_driver_status();
	
	
}

void InterfaceMini::status_override(String p_message) {
	
	status_override_time=STATUS_OVERRIDE_TIME_DEFAULT;
	//status_label->set_text( p_message );
}
void InterfaceMini::song_menu_selected(int p_option) {
	
	
	switch (p_option) {
		case SONG_MENU_NEW: {
			
			new_song.patterns->set_checked(false);
			new_song.instruments->set_checked(false);
			new_song.samples->set_checked(false);
			new_song.variables->set_checked(false);
			new_song.window->show();
			last_filename=""; //this is to avoid a potentially dangerous risk of overtwriting previous song
			
		} break;	
		case SONG_MENU_OPEN: {
			
			file_dialog->clear_filter_list();
			file_dialog->add_filter("Impulse Tracker",".it");
			file_dialog->add_filter("Fast Tracker 2",".xm");
			file_dialog->add_filter("Scream Tracker 3",".s3m");
			file_dialog->add_filter("Standard Module",".mod");
			file_dialog->show(FileDialog::MODE_OPEN);
			
		} break;
		case SONG_MENU_SAVE: {
			
			if (last_filename!="") {
				
				Saver::Error err = tracker.file_manager.save_song( last_filename.utf8().get_data(), tracker.song );
				
				switch(err) {
					
					case Saver::SAVE_ERROR: error_msg->show("Error Saving"); break;
					case Saver::UNKNOWN_EXTENSION: error_msg->show("Unknown File Extension"); break;
					default:{}; 
				}
				break;
				
			}
			
		} //go to saveas otherwise
		case SONG_MENU_SAVE_AS: {
			
			file_dialog->clear_filter_list();
			file_dialog->add_filter("Impulse Tracker",".it");
			file_dialog->show(FileDialog::MODE_SAVE);
			
			//file_dialog->add_filter("Fast Tracker 2",".xm");
			//file_dialog->add_filter("Scream Tracker 3",".s3m");
			//file_dialog->add_filter("Standard Module",".mod");
			
		} break;
		case SONG_MENU_EXPORT_WAV: {
			
			wav_saver->show();
			
			//file_dialog->add_filter("Fast Tracker 2",".xm");
			//file_dialog->add_filter("Scream Tracker 3",".s3m");
			//file_dialog->add_filter("Standard Module",".mod");
			
		} break;
		case SONG_MENU_CLEANUP: {
			
			cleanup.patterns->set_checked(false);
			cleanup.instruments->set_checked(false);
			cleanup.samples->set_checked(false);
			cleanup.orders->set_checked(false);
			cleanup.def_pan->set_checked(false);
			cleanup.def_vol->set_checked(false);
			cleanup.window->show();
			
		} break;
		case SONG_MENU_QUIT: {
			
			quit_request();
		} break;
	};
	
}

void InterfaceMini::config_menu_selected(int p_option) {
	
	switch(p_option) {
			
		case CONFIG_MENU_AUDIO: {
			
			sound_driver_dialog->show();
		} break;
		case CONFIG_MENU_MIXER: {
			
			mixer_dialog->show();
		} break;
		case CONFIG_MENU_KEYS: {
			
			kb_dialog->show();
			
		} break;
		case CONFIG_MENU_THEME: {
			
			ui_dialog->show();
		} break;
		case CONFIG_MENU_PATHS: {
			
			path_dialog->show();
		} break;
			
			
		
	}
}

void InterfaceMini::help_menu_selected(int p_option) {
	
	switch(p_option) {
			
		case HELP_MENU_PATTERN: {
			
			uihelp_dialog->show();
		} break;
		case HELP_MENU_SKINS: {
			
			skins_dialog->show();
		} break;
		case HELP_MENU_ABOUT: {
			
			about_msg->show("ChibiTracker v"+String::num(VERSION_MAJOR)+"."+String::num(VERSION_MINOR)+String(VERSION_REV)+ " - (c) 2001-2006 Juan Linietsky");
			
		} break;
			
		
	}
	
	
}

void InterfaceMini::new_song_accept() {
	
	new_song.window->hide();
	
	tracker.song->reset( !new_song.patterns->is_checked() , !new_song.samples->is_checked(), !new_song.instruments->is_checked(), !new_song.variables->is_checked() );
	update_components();
	
}


void InterfaceMini::song_file_callback(String p_fname) {
	
	switch (file_dialog->get_mode()) {
				
		case FileDialog::MODE_OPEN: {
			
			button_stop_pressed(); //stop playback before loading.. avoid mess!
			Loader::Error err = tracker.file_manager.load_song( p_fname.utf8().get_data(), tracker.song );
			if (!err)
				last_filename=p_fname;
			
			switch(err) {
					
				case Loader::FILE_CANNOT_OPEN: error_msg->show("Can't Open File!"); break;
				case Loader::FILE_CORRUPTED: error_msg->show("File Corrupted"); break;
				case Loader::FILE_OUT_OF_MEMORY: error_msg->show("Out Of Memory"); break;
				case Loader::FILE_UNRECOGNIZED: error_msg->show("Unknown File Type"); break;
				default:{};
			}
			
			// 	[15:23] <mrb> most trackers put you on the pattern that corresponds to order 0
			if (tracker.song->get_order(0) < Song::MAX_PATTERNS ) {
				tracker.editor->set_current_pattern( tracker.song->get_order(0) );
			}
			update_components();
		} break;
		case FileDialog::MODE_SAVE: {
			
			Saver::Error err = tracker.file_manager.save_song( p_fname.utf8().get_data(), tracker.song );
			
			switch(err) {
					
				case Saver::SAVE_ERROR: error_msg->show("Error Saving"); break;
				case Saver::UNKNOWN_EXTENSION: error_msg->show("Unknown File Extension"); break;
				default:{};
			}
			
			if (!err)
				last_filename=p_fname;

		} break;
		default: {}
	}
}

void InterfaceMini::cleanup_accept() {
	
	cleanup.window->hide();
	/* by logic, orders must be cleaned first */
	if (cleanup.orders->is_checked())
		tracker.song->cleanup_unused_orders();
		
	/* then, patterns must be cleaned second */
	if (cleanup.patterns->is_checked())
		tracker.song->cleanup_unused_patterns();
	
	/* thid, instruments */
	if (cleanup.instruments->is_checked())
		tracker.song->cleanup_unused_instruments();
	
	/* and last, samples */
	if (cleanup.samples->is_checked())
		tracker.song->cleanup_unused_samples();
	
	/* This is optative, mainly for XM imports.. */
	if (cleanup.def_pan->is_checked())
		tracker.song->clear_all_default_pan();

	if (cleanup.def_vol->is_checked())
		tracker.song->clear_all_default_vol();
	
	update_components();
}

void InterfaceMini::quit_accept() {
	
	
	quit=true;
}

bool InterfaceMini::must_quit() {
	
	return quit;
}
void InterfaceMini::quit_request() {
	
	confirm_quit_window->show();	
}


void InterfaceMini::pre_screen_changed_from(int p_which) {
	
	
	if (p_which<0 || p_which>=SCREEN_MAX)
		return;
	
	/* Here, we'll remember which widget was focused */
	
	Frame *in_focus=window->get_focused_child();
	
	
	switch(p_which) {
		
		case SCREEN_PATTERN:{ if ( !pattern_screen->is_far_parent_of(in_focus) ) in_focus=0; } break;
		case SCREEN_SAMPLES:{ if ( !sample_screen->is_far_parent_of(in_focus) ) in_focus=0; } break;
		case SCREEN_INSTRUMENTS:{ if ( !instrument_screen->is_far_parent_of(in_focus) ) in_focus=0; } break;
		case SCREEN_VARIABLES:{ if ( !variables_screen->is_far_parent_of(in_focus) ) in_focus=0; } break;
		default: {} //dont annoy, gcc
		
	}
	
//	printf("Changing from screen %i\n",p_which);
	old_focus[p_which]=in_focus;
	
}

void InterfaceMini::screen_changed_to(int p_which) {
	
	if (p_which==SCREEN_PATTERN)
		return;
		
	/* Set to the old focused widget in this screen */
	
	if (old_focus[p_which]) {
		old_focus[p_which]->get_focus();
	} else {
		
		switch (p_which) {
			
			case SCREEN_SAMPLES:{ sample_screen->force_focus(); } break;
			case SCREEN_INSTRUMENTS:{ instrument_screen->force_focus(); } break;
			case SCREEN_VARIABLES:{ variables_screen->force_focus(); } break;
			default: {} //dont annoy, gcc
		}
	}
		
	
//	printf("Changing to screen %i\n",p_which);
	
}


void InterfaceMini::skin_changed() {
	
	main_stack->set_stylebox_override(window->get_skin()->get_stylebox(SB_MAIN_STACK_FRAME));
	top->set_stylebox_override( window->get_skin()->get_stylebox(SB_TOP_STACK_FRAME));
	
}

void InterfaceMini::main_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	
	
	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return;
	
	
	switch(kbind) {
		
		case KB_PLAYER_PLAY_SONG: {
			
			if (!p_press)
				break;
			button_play_pressed();
		} break;
		case KB_PLAYER_STOP_SONG: {
			
			if (!p_press)
				break;
			
			button_stop_pressed();
			
		} break;
		case KB_PLAYER_FF_SONG: {
			
			if (!p_press)
				break;
			
			button_fw_pressed();
			
		} break;
		case KB_PLAYER_RW_SONG: {
			
			if (!p_press)
				break;
			
			button_rw_pressed();
			
		} break;
		case KB_PLAYER_PLAY_PATTERN: {
			
			if (!p_press)
				break;
			button_play_pattern_pressed();
		} break;
		case KB_PLAYER_PLAY_CURSOR: {
			
			if (!p_press)
				break;
			
			button_play_cursor_pressed();
		} break;
		case KB_PLAYER_PLAY_ORDER: {
			
			if (!p_press)
				break;
			
			button_play_order_pressed();			
		} break;
		
		case KB_GOTO_PATTERN_SCREEN: {
			
			if (!p_press)
				break;

			pre_screen_changed_from( tabs->get_selected_tab() );			
			tabs->set_tab( SCREEN_PATTERN );
			main_stack->raise( SCREEN_PATTERN );
			screen_changed_to( SCREEN_PATTERN );			
			pattern_screen->goto_pattern();
			
			
		} break;
		case KB_GOTO_PATTERN_SCREEN_ORDERLIST: {
			
			if (!p_press)
				break;
			
			pre_screen_changed_from( tabs->get_selected_tab() );			
			tabs->set_tab( SCREEN_PATTERN );
			main_stack->raise( SCREEN_PATTERN );
			screen_changed_to( SCREEN_PATTERN );
			pattern_screen->goto_orderlist();
		
			
		} break;
		
		case KB_GOTO_SAMPLE_SCREEN: {
			
			if (!p_press)
				break;
			
			pre_screen_changed_from( tabs->get_selected_tab() );			
			tabs->set_tab( SCREEN_SAMPLES );
			main_stack->raise( SCREEN_SAMPLES );
			screen_changed_to( SCREEN_SAMPLES );
		} break;
		
		case KB_GOTO_INSTRUMENT_SCREEN: {
			
			if (!p_press || !tracker.song->has_instruments())
				break;
			
			pre_screen_changed_from( tabs->get_selected_tab() );			
			tabs->set_tab( SCREEN_INSTRUMENTS );
			main_stack->raise( SCREEN_INSTRUMENTS);
			screen_changed_to( SCREEN_INSTRUMENTS );
			
			
		} break;
		case KB_GOTO_VARIABLES_SCREEN: {
			
			if (!p_press)
				break;
			
			pre_screen_changed_from( tabs->get_selected_tab() );			
			tabs->set_tab( SCREEN_VARIABLES );
			main_stack->raise( SCREEN_VARIABLES);
			screen_changed_to( SCREEN_VARIABLES );
			
			screen_changed_to( SCREEN_VARIABLES );
			
		} break;
		case KB_FILE_OPEN: {
			
			if (!p_press)
				break;
			song_menu_selected( SONG_MENU_OPEN );
		} break;
		case KB_FILE_SAVE: {
			
			if (!p_press)
				break;
			
			song_menu_selected( SONG_MENU_SAVE );
			
		} break;
		case KB_FILE_SAVE_AS: {
			
			if (!p_press)
				break;
			
			song_menu_selected( SONG_MENU_SAVE_AS );
						
		} break;
		
		case KB_OCTAVE_RAISE: {
			
			if (!p_press)
				break;
			
			tracker.editor->set_default_octave( tracker.editor->get_default_octave() +1 );
			/*default_octave->get_range()->set( (int)tracker.editor->get_default_octave() );*/
						
		} break;
		case KB_OCTAVE_LOWER: {
			
			if (!p_press)
				break;
			
			tracker.editor->set_default_octave( tracker.editor->get_default_octave() -1 );
			/*default_octave->get_range()->set( (int)tracker.editor->get_default_octave() );*/
			
		} break;
		case KB_QUIT: {
			
			if (!p_press)
				break;
			quit_request();
		} break;
		
		default: {

			switch( tabs->get_selected_tab() ) {
				
				case SCREEN_PATTERN: {
					
					pattern_screen->handle_key( p_unicode,p_scan_code,p_press,p_repeat,p_modifier_mask);
					
				} break;
				case SCREEN_SAMPLES: {
					
					sample_screen->handle_key( p_unicode,p_scan_code,p_press,p_repeat,p_modifier_mask);
					
				} break;
				case SCREEN_INSTRUMENTS: {
					
					instrument_screen->handle_key( p_unicode,p_scan_code,p_press,p_repeat,p_modifier_mask);
					
				} break;
				case SCREEN_VARIABLES: {
					
					
				} break;
				
				
			}
		
		} 
	};	
}

void InterfaceMini::check_instrument_visibility() {
	
	tabs->set_tab_visible( 2, tracker.song->has_instruments() );
	instrument_screen->update_components();
}
Tracker * InterfaceMini::get_tracker() {
	
	return &tracker;
}

void InterfaceMini::update_components() {
	
	pattern_screen->update_components();
	sample_screen->update_components();
	instrument_screen->update_components();
	variables_screen->update_components();
	check_instrument_visibility();
	update_title_bar();
}

void InterfaceMini::button_rw_pressed() {
	
	tracker.player->goto_previous_order();
}
void InterfaceMini::button_play_pressed(){
	
	tracker.player->play_start_song();
}
void InterfaceMini::button_stop_pressed(){
	
	tracker.player->play_stop();
	//stop virtual piano too
	tracker.virtual_piano->instrument_stop_all();
	tracker.virtual_piano->sample_stop_all();
	
}
void InterfaceMini::button_fw_pressed(){
	
	tracker.player->goto_next_order();
}
void InterfaceMini::button_play_pattern_pressed(){
	
	tracker.player->play_start_pattern( tracker.editor->get_current_pattern() );
	
	
}

void InterfaceMini::button_play_order_pressed() {
	
	tracker.player->play_start_song_from_order( tracker.editor->orderlist_get_cursor_y() );	
}

void InterfaceMini::button_play_cursor_pressed() {
	
	int order=-1;
	
	for (int i=0;i<Song::MAX_ORDERS;i++) {
		
		if (tracker.song->get_order(i)==tracker.editor->get_current_pattern()) {
			
			order=i;
			break;
		} 
	}
	
	if (order>=0) {
		
		tracker.player->play_start_song_from_order_and_row( order, tracker.editor->get_cursor_y() );
	} else {
		tracker.player->play_start( tracker.editor->get_current_pattern(),-1,tracker.editor->get_cursor_y());
	};
		
	//tracker.player->play_start(  tracker.editor->get_current_pattern() );
	
}

void InterfaceMini::default_octave_changed(double p_to) {
	
	tracker.editor->set_default_octave( (int) p_to );
}
void InterfaceMini::instrument_mask_changed(int p_to) {
	
	String insname="Invalid";
	if (tracker.song->has_instruments()) {
		instrument_screen->select(p_to);
		
		if (tracker.song->get_instrument( p_to ) ) {
			
			insname=tracker.song->get_instrument( p_to )->get_name();
			
		}
		
		status_override( "Instrument mask changed to: " + String::num( p_to+1 ) + " - \"" + insname +"\"");
	} else {
		sample_screen->select(p_to);
		if (tracker.song->get_sample( p_to ) ) {
			
			insname=tracker.song->get_sample( p_to )->get_name();
			
		}		
		status_override( "Sample mask changed to: " + String::num( p_to+1 ) + " - \"" + insname + "\"");
		
	}
	
	
	
}

void InterfaceMini::ui_repaint_interval_changed(int p_to) {
	
	window->get_timer()->change_timer_interval( timer, p_to );		
}



void InterfaceMini::paths_changed() {
	
	sample_screen->set_sample_path( paths.sample_path );
	instrument_screen->set_instrument_path( paths.instrument_path );
	file_dialog->set_dir( paths.song_path );
}
void InterfaceMini::setup_logos() {
	
	
	
	
	about_msg->get_custom_vb()->add( new Icon( get_me_a_bitmap(window->get_painter(),about_logo) ));
	
	about_msg->get_custom_vb()->add( new Label("http://www.chibitracker.com",Label::ALIGN_CENTER));

	window->get_painter()->set_window_icon( get_me_a_bitmap(window->get_painter(),ct_icon) );
	
	
	
}

void InterfaceMini::show_splash() {
	
	MessageBox *initial=new MessageBox(window);
	initial->get_custom_vb()->add(new Label("This is the FIRST ChibiTracker public release."));
	initial->get_custom_vb()->add(new Label("As such, it may still contain some bugs.."));
	initial->get_custom_vb()->add(new Label("If you happen to find one (or more)"));
	initial->get_custom_vb()->add(new Label("Please send a bug report to:"));
	initial->get_custom_vb()->add(new Label("  reduzio@gmail.com"));
	initial->get_custom_vb()->add(new Label("Help make ChibTracker stable!"));
	initial->show("");	
}

InterfaceMini::InterfaceMini(Window *p_window,Mixer *p_mixer,ConfigApi *p_config) : tracker(p_mixer) {

	config=p_config;

	window=p_window;
	//window->set_large_dialogs(true);
	
	MiniTheme::create(window->get_skin(),window->get_painter());
	
	VBoxContainer *vbc = new VBoxContainer;

	window->set_root_frame(vbc);
	window->key_unhandled_signal.connect( this, &InterfaceMini::main_key );
	vbc->set_separation( 0 );
	
	top=vbc->add( new VBoxContainer);
	
	top_bar=top->add( new HBoxContainer, 0 );

	
	file = top_bar->add( new MenuBox("Song.."), 0);
	file->add_item("New");
	file->add_item("Open..");
	file->add_item("Save");
	file->add_item("Save As..");
	file->add_separator();
	file->add_item("Export WAV");
	file->add_separator();
	file->add_item("Clean Up");
	file->add_separator();
	file->add_item("Quit");
	file->item_selected_signal.connect(this, &InterfaceMini::song_menu_selected );
	file->set_focus_mode( FOCUS_NONE );
	top_bar->add( new VSeparator,0);
	
	/* Tab Bar */
	
	tabs=top_bar->add( new TabBar, 0 );
	tabs->add_tab("Pat");
	tabs->add_tab("Smp");
	tabs->add_tab("Ins");
	tabs->add_tab("Var");
	tabs->set_focus_mode( FOCUS_NONE );
	tabs->set_tab_visible( 2, false );
	

//	top_bar->add( new Widget,1);
	
	top_bar->add( new VSeparator,0);
	settings=top_bar->add(new MenuBox("Setup"),0);
	settings->add_item("Audio Driver");
	settings->add_item("Software Mixer");
	settings->add_item("Keyboard");
	settings->add_item("InterfaceMini");
	settings->add_item("Default Paths");
	settings->item_selected_signal.connect( this, &InterfaceMini::config_menu_selected );
	settings->set_focus_mode( FOCUS_NONE );
	top_bar->add( new VSeparator );
	help=top_bar->add(new MenuBox("Help"),0);
	help->add_item("Command List");
	help->add_item("Skins");
	help->add_item("About");
	help->set_focus_mode( FOCUS_NONE );
	help->item_selected_signal.connect( this, &InterfaceMini::help_menu_selected );
	
	/*
	HBoxContainer *status_hb = top->add( new HBoxContainer, 0);
	status_label = status_hb->add(new Label("Stopped"),1);
	status_hb->add( new Label("Memory:  "), 0);
	memory_label=status_hb->add( new Label("0kb"), 0); */

	
	top->set_stylebox_override( window->get_skin()->get_stylebox(SB_TOP_STACK_FRAME) );
	
	
	main_stack=vbc->add( new StackContainer,1);
	
	
	tabs->tab_changed_signal.connect( main_stack, &StackContainer::raise );
	tabs->tab_changed_signal.connect( this, &InterfaceMini::screen_changed_to );
	tabs->pre_tab_changed_signal.connect( this, &InterfaceMini::pre_screen_changed_from );
	
	pattern_screen=main_stack->add( new PatternScreen(&tracker,true) );
	
	sample_screen=main_stack->add( new SampleScreen(&tracker,true) );
	
	instrument_screen = main_stack->add(new InstrumentScreen(&tracker,true));
	instrument_screen->sample_list_changed_signal.connect( sample_screen, &SampleScreen::update_components );
	
	
	variables_screen = main_stack->add(new VariablesScreen(&tracker,true));
	variables_screen->song_name_changed_signal.connect(this, &InterfaceMini::update_title_bar);
	
	variables_screen->instrument_mode_changed.connect( this, &InterfaceMini::check_instrument_visibility );
	
	set_default_keybinds();
	
	main_stack->set_stylebox_override(window->get_skin()->get_stylebox(SB_MAIN_STACK_FRAME));

/*	default_octave->get_range()->set( (int)tracker.editor->get_default_octave() ); */
	
	for (int i=0;i<SCREEN_MAX;i++)
		old_focus[i]=0;		
	
	
	file_dialog = new FileDialog(window);
	file_dialog->file_activated_signal.connect( this, &InterfaceMini::song_file_callback );
	
	/* New Song Dialog */
	
	new_song.window=new Window(window,Window::MODE_POPUP,Window::SIZE_CENTER);
	
	WindowBox *new_song_vb = new WindowBox("New Song");
	new_song.window->set_root_frame( new_song_vb );
	MarginGroup *new_song_mg = new_song_vb->add(new MarginGroup("New Song"),0);
	
	new_song.patterns = new_song_vb->add( new CheckButton("Keep Patterns"),0);
	new_song.instruments = new_song_vb->add( new CheckButton("Keep Instruments"),0);
	new_song.samples = new_song_vb->add( new CheckButton("Keep Samples"),0);
	new_song.variables = new_song_vb->add( new CheckButton("Keep Variables"),0);
	
	
	new_song_vb->add( new CenterContainer, 0 )->set( new Button("Proceed"))->pressed_signal.connect(this,&InterfaceMini::new_song_accept);
	
	/* CleanUp Dialog */
	
	cleanup.window=new Window(window,Window::MODE_POPUP,Window::SIZE_CENTER);
	
	WindowBox *cleanup_vb = new WindowBox("Clean Up");
	cleanup.window->set_root_frame( cleanup_vb );
	
	cleanup.patterns = cleanup_vb->add( new CheckButton("Unused Patterns"),0);
	cleanup.instruments = cleanup_vb->add( new CheckButton("Unused Instruments"),0);
	cleanup.samples = cleanup_vb->add( new CheckButton("Unused Samples"),0);
	cleanup.orders = cleanup_vb->add( new CheckButton("Unused Orders"),0);
	cleanup_vb->add( new HSeparator,0);
	cleanup.def_vol = cleanup_vb->add( new CheckButton("Reset Default Volumes"),0);
	cleanup.def_pan = cleanup_vb->add( new CheckButton("Reset Default Pannings"),0);
	
	cleanup_vb->add( new CenterContainer, 0 )->set( new Button("Remove"))->pressed_signal.connect(this,&InterfaceMini::cleanup_accept);
	
	
	/* Quit Menu */
	quit=false;
	
	confirm_quit_window=new Window(window,Window::MODE_POPUP,Window::SIZE_CENTER);
	WindowBox *quit_vb = new WindowBox("Quit");
	
	confirm_quit_window->set_root_frame( quit_vb );
	quit_vb->add( new Label("Really Quit?"),0);
	quit_vb->add( new CenterContainer, 0)->set( new Button("Exit") )->pressed_signal.connect( this, &InterfaceMini::quit_accept );
	
	sound_driver_dialog = new SoundDriverDialog(window,config);	
	mixer_dialog = new MixerDialog(window,config);
	kb_dialog = new KeyboardDialog(window,config);
	ui_dialog = new InterfaceSettingsDialog(window,config);
	ui_dialog->repaint_interval_changed_signal.connect(this, &InterfaceMini::ui_repaint_interval_changed );
	uihelp_dialog = new InterfaceHelp(window,HELP_PATTERN_KEYS);
	skins_dialog = new InterfaceHelp(window,HELP_SKINS);
	wav_saver = new WavSaverDialog(window,&tracker);
	path_dialog = new PathsDialog(window,&paths,config);
	path_dialog->paths_changed_signal.connect( this, &InterfaceMini::paths_changed );
	
	
	window->get_timer()->create_timer( Method(this, &InterfaceMini::interface_update_timer_callback ),60 );
	pattern_screen->get_pattern_editor()->instrument_changed_signal.connect(this, &InterfaceMini::instrument_mask_changed );
	sample_screen->instrument_changed_signal.connect(this, &InterfaceMini::instrument_mask_changed );
	
	error_msg = new MessageBox(window);
	about_msg = new MessageBox(window);
	
	window->skin_changed_signal.connect(this,&InterfaceMini::skin_changed);
	update_title_bar();
	setup_logos();
	
	sample_screen->instrument_list_changed_signal.connect( instrument_screen, &InstrumentScreen::update_components );
	status_override_time=0;

}


InterfaceMini::~InterfaceMini()
{
}


