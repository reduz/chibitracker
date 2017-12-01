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
/***************************************************************************
                          editor.h  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EDITOR_H
#define EDITOR_H

#include "song/song.h"
#include "player/player_data.h"

/**
  *@author Juan Linietsky
  */

/************************************************
 editor.h
 --------
This class aims to be a pure abstraction to the
editing features. This way, programming a GUI
in other OS/Toolkit should be much easier
*************************************************/



class Editor {
public:
	
	enum {
		
		MAX_UNDO_OPERATIONS=4096,
		CURSOR_FIELD_MAX=9,
	};
	enum TrackerRedrawFlags{

		TRACKER_REDRAW_ROW=1,
		TRACKER_REDRAW_COLUMN=2,
		TRACKER_REDRAW_ALL=4,
		TRACKER_REDRAW_TOP=8,
		TRACKER_REDRAW_PLAYING_ROW=16,
	};

	
	enum PatternOperationType {
		PATTERN_NONE,
		PATTERN_OP_SET_PATTERN,
		PATTERN_RAISE_NOTES_ON_SELECTION,
		PATTERN_LOWER_NOTES_ON_SELECTION,
		PATTERN_SELECTION_ZAP,
		PATTERN_SELECTION_RAMP_PARAMETERS,
		PATTERN_SELECTION_RAMP_VOLUMES,
		PATTERN_SELECTION_PASTE_OVERWRITE,
		PATTERN_SELECTION_PASTE_MIX,
		PATTERN_SELECTION_PASTE_INSERT,
		PATTERN_SELECTION_VOLUME_SCALE,
		PATTERN_SELECTION_SET_INSTRUMENT_MASK,
		PATTERN_SELECTION_SET_VOLUME_MASK,
		PATTERN_SELECTION_EXPAND_X2,
		PATTERN_SELECTION_SHRINK_X2,
		PATTERN_SELECTION_WIPE_STRAY_VOLUMES,
		PATTERN_SELECTION_TIME_SCALE,
		PATTERN_SELECTION_TRANSPOSE,
		PATTERN_INPUT_DATA
		
	};
	
	enum Pattern_Actions {

		Pattern_MARK_BLOCK_BEGIN,
		Pattern_MARK_BLOCK_END,
		Pattern_MARK_COLUMN_ALL,
		Pattern_BLOCK_COPY,
		Pattern_BLOCK_PASTE_INSERT,
		Pattern_BLOCK_PASTE_OVERWRITE,
		Pattern_BLOCK_UNMARK,
		Pattern_BLOCK_CUT,
		Pattern_BLOCK_RAISE_NOTES,
		Pattern_BLOCK_LOWER_NOTES,
		Pattern_BLOCK_SET_CURRENT_INSTRUMENT,
		Pattern_BLOCK_SET_CURRENT_VOLUME,
		Pattern_BLOCK_WIPE_STRAY_VOLUMES,
		Pattern_BLOCK_RAMP_WIPE_VOLUMES,
		Pattern_BLOCK_RAMP_WIPE_EFFECTS,
		Pattern_BLOCK_DOUBLE_BLOCK_LENGTH,
		Pattern_BLOCK_HALVE_BLOCK_LENGTH,
		Pattern_FIELD_TOGGLE_MASK,
		Pattern_FIELD_CLEAR,
		Pattern_FIELD_COPY_CURRENT,
		Pattern_FIELD_PASTE_CURRENT,
		Pattern_MAX_ACTIONS
	};
private:

	
	
	/* POSITIONING VARIABLES */

	int column_offset,row_offset;

        int visible_columns,visible_rows;

	int cursor_x,cursor_y,cursor_field,current_pattern;

	int previous_cursor_x,previous_cursor_y;

	int playing_row,playing_row_old;

	bool playing;

	/* DATA INSERTION METHODS */

	void insert_noteoff_at_cursor();
	void insert_mask_at_cursor();
	void insert_notecut_at_cursor();
	void insert_script_at_cursor();
	
	bool insert_note_at_cursor(unsigned int chr);
	void insert_octave_at_cursor(Uint8 octave);

	void insert_instrument_at_cursor_field1(Uint8 parameter);
	void insert_instrument_at_cursor_field2(Uint8 parameter);

	void insert_script_source_at_cursor_field1(unsigned int unicode);
	void insert_script_source_at_cursor_field2(Uint8 parameter);
	
	void insert_script_instrument_at_cursor_field1(Uint8 parameter);
	void insert_script_instrument_at_cursor_field2(Uint8 parameter);
	
	void insert_volume_at_cursor_field1(Uint8 volume);
	void insert_volume_at_cursor_field2(Uint8 volume);

	void insert_command_at_cursor(char command);
	void insert_parameter_at_cursor_field1(Uint8 parameter);
	void insert_parameter_at_cursor_field2(Uint8 parameter);

	void insert_script_command_at_cursor(char command);
	void insert_script_parameter_at_cursor_field1(Uint8 parameter);
	void insert_script_parameter_at_cursor_field2(Uint8 parameter);

	/* USER_EDITING_VARIABLES */

	static int default_octave;

	int last_note;
	int last_instrument;	
	int last_volume;
	int last_command;
	int last_parameter;

	bool instrument_mask;
	bool volume_mask;
	bool command_mask;
        bool volume_pan_mode;
	bool script_mode;

        int cursor_spacing;
	
	/* PLAYER MARK */

	int marked_pattern,marked_row;
	bool mark_active;

	/* SELECTION VARIABLES*/

	static Pattern clipboard;
	
	static int clipboard_width;

	int selection_begin_x,selection_begin_y,selection_end_x,selection_end_y;
	bool selection_active;
	int shift_selection_begin_x,shift_selection_begin_y,shift_selection_end_x,shift_selection_end_y;
	int mouse_selection_begin_x,mouse_selection_begin_y;
	bool shift_selection_in_progress;

	/* Undo Buffer */

	int undo_max;
	int undo_op_count;
	struct Undo_Operation {

		bool is_redo;
		PatternOperationType reason;
		int pattern_index;
		Pattern *pattern_data;
		
	};

	Undo_Operation undo_buffer[MAX_UNDO_OPERATIONS];
	bool undo_display_pending;

	void push_pattern_to_undo_list(int p_index,const Pattern &p_pattern,PatternOperationType p_optype,bool p_is_redo=false,bool p_dont_collapse=false);

	/* REDRAW OPTIMIZATION FLAGS */

 	char redraw_flags;

	/* ORDERLIST EDITING */

	int orderlist_cursor_x,orderlist_cursor_y;
        /* VISUAL INFO SCREEN */

	int visual_channel;

	/* KEYBOARD CUSTOMIZATION */


	bool song_follow;

        static const char* notes[128];
	/* SONG DATA SOURCE */
	
	Song *song;
	Player *player;

	
	void set_current_note(const Note& p_note);
	Note get_current_note();
	
	bool handle_scancode(unsigned int scode,bool p_shift,bool& modified);
	
	bool previous_shift;
	
	bool selection_time_scale_scale (
			int * p_row,			// Location of row value to be modified
	int * p_tick,			// Location of tick value to be modified
	float f_initial_stretch,
	float f_final_stretch,
	float f_total_stretch,
	int offset,
	int p_row_times[Pattern::MAX_LEN + 1],
	int pattern_length
					);
	
public:

	Song *get_song() { return song; }
	Player *get_player() { return player; }

	/* RAW DATA RETRIEVING */

	void get_single_note_string(Uint8 p_note,char *p_str);
	int get_note_from_charcode(unsigned int p_charcode);
	void get_note_string(int column,int row,char *buf);
	bool is_note_cloned(int column,int row);

	//The visual ones!

	void set_visible_columns(int p_visible); ///< Called by Pattern Edit
	void set_visible_rows(int p_visible); ///< Called by Pattern Edit
	int get_visible_columns(); ///< Called by Pattern Edit
	int get_visible_rows(); ///< Called by Pattern Edit

	int get_column_offset() { return column_offset; }
	int get_row_offset() { return row_offset; };

	/* PSEUDO SCREEN INFO SETTING */

	void set_column_offset(int value);
	void set_row_offset(int value);
        void adjust_view_to_cursor();

	/* EDITING VARIABLES RETRIEVING */

	int get_cursor_x() { return cursor_x; };
	int get_cursor_y() { return cursor_y; };
        void set_cursor_y(int p_cursor_y) { redraw_flags|=TRACKER_REDRAW_ROW; cursor_y=p_cursor_y; };
        void set_cursor_x(int p_cursor_x) { cursor_x=p_cursor_x; };
	int get_previous_cursor_x() { return previous_cursor_x; };
	int get_previous_cursor_y() { return previous_cursor_y; };

	void selection_begin_at_cursor();
	void selection_end_at_cursor();

	int get_current_pattern() { return current_pattern; };
        void set_current_pattern(int p_current_pattern) {  redraw_flags|=TRACKER_REDRAW_ALL; current_pattern=p_current_pattern;};

	void set_previous_cursor_x(int p_previous_cursor_x) { previous_cursor_x=p_previous_cursor_x; };
	void set_previous_cursor_y(int p_previous_cursor_y) { previous_cursor_y=p_previous_cursor_y; };

	int get_cursor_field() { return cursor_field; };
	void set_cursor_field(int p_f) { if ((p_f>=0) && (p_f<CURSOR_FIELD_MAX)) cursor_field=p_f; }

        void set_cursor_spacing(int amount) { cursor_spacing=amount;};
        int get_cursor_spacing() { return cursor_spacing; };

        bool get_instrument_mask() { return instrument_mask; };
        void set_instrument_mask_value(int p_value) { last_instrument=p_value; };
        int get_instrument_mask_value() { return last_instrument;};

        bool get_volume_mask() { return volume_mask; };
        bool get_command_mask() { return command_mask; };
	
	void set_volume_mask(bool p_set) { volume_mask=p_set; }
	void set_instrument_mask(bool p_set) { instrument_mask=p_set; }
	void set_command_mask(bool p_set) { command_mask=p_set; }
		
	static char get_default_octave();
	static void set_default_octave(char value);

	bool get_volume_pan_mode() { return volume_pan_mode; };

	/* EDITING VARIABLES SETTING */

	void cursor_move_up(int amount);
	void cursor_move_left(int amount);
	void cursor_move_down(int amount);
	void cursor_move_right(int amount);
	void cursor_move_field_right();
	void cursor_move_field_left();
	void cursor_page_up();
	void cursor_page_down();
	void cursor_move_up();//uses cursor_spacing
	void cursor_move_down();//uses cursor_spacing
	void cursor_home();
	void cursor_end();
	void cursor_tab();	
	void cursor_back_tab();
	void cursor_insert();
	void cursor_delete();
        void toggle_mask_at_cursor();
	void clear_field_at_cursor();
	void cursor_next_pattern();
	void cursor_prev_pattern();
	void cursor_goto_pattern(int p_pattern);

	void default_octave_raise();
	void default_octave_lower();

	/* MARK */

	void set_mark();
	int get_marked_pattern() { return marked_pattern;};
	int get_marked_row() { return marked_row; };
	bool is_mark_active() { return mark_active; };


	/* UNDO OPTIONS */

	void undo_index(int p_undo_index);
	int get_undo_max();
	int get_undo_count();

	int get_undo_pattern(int p_undo_index);
	PatternOperationType get_undo_info(int p_undo_index);
	bool get_undo_is_redo(int p_undo_index);
	bool has_undo_display_pending() { return undo_display_pending;}
	void reset_pending_undo_display() { undo_display_pending=false; }
	void push_current_pattern_to_undo_list(PatternOperationType p_reason);


	/* SELECTION BASIC BLOCK FUNCTIONS */

	void normalize_selection();
	void mark_block_begining();
	void mark_block_end();
        void selection_release();
        void mark_column_all();

	/* SELECTION INFO RETRIEVING */

	int get_selection_begin_x () { return selection_begin_x; };
	int get_selection_begin_y () { return selection_begin_y; };
	int get_selection_end_x () { return selection_end_x; };
	int get_selection_end_y () { return selection_end_y; };
	bool is_selection_active () { return selection_active; };

	/* SELECTION SHIFT/MOUSE SETTING */

	void shift_selection_begin();
	void shift_selection_end();
	void shift_selection_update();

	/* SELECTION BASIC CLIPBOARD OPERATIONS */

	void selection_copy();
	void selection_paste_overwrite();
	void selection_paste_insert();
        void selection_paste_mix();
        void selection_zap();
	void selection_time_scale(float f_initial_stretch,float f_final_stretch,float f_total_stretch,int offset);
	
	void transpose(Uint8 *p_values); //array of 12

	float selection_time_scale_curve (float f_initial_velocity,float f_final_velocity,float x);// must be in the range [0, 1]
	
	/* SELECTION CLIPBOARD MODIFIERS */

        void selection_raise();
        void selection_lower();
	void selection_raise_octave();
	void selection_lower_octave();
	void selection_volume_pan_ramp();
        void selection_volume_scale(int p_percent);
        void selection_parameter_ramp_wipe();
        void selection_wipe_stray_volumes();
	void selection_set_volume_mask();
        void selection_set_instrument_mask();
        void selection_expand();
        void selection_shrink();

	/* KEY PRESS HANDLERS */

	bool key_press_at_cursor(unsigned int p_scode,unsigned int p_unicode,bool p_shift=false);
	void get_mask_from_cursor();

	void perform_raise_at_cursor();
	void perform_lower_at_cursor();
	void perform_raise_octave_at_cursor();
	void perform_lower_octave_at_cursor();

	/* ORDERLIST KEY PRESS HANDLERS */

	void orderlist_move_up();
	void orderlist_move_down();
	void orderlist_move_left();
	void orderlist_move_right();
	void orderlist_page_up();
	void orderlist_page_down();

	void orderlist_insert_value(int number);
	void orderlist_add_separator();
	void orderlist_clear_order();
        void orderlist_insert();
	void orderlist_delete();
	void orderlist_set_cursor_row(int p_row);

	/* ORDERLIST INFO RETRIEVING */

	int orderlist_get_cursor_x() { return orderlist_cursor_x; };
	int orderlist_get_cursor_y() { return orderlist_cursor_y; };
	void orderlist_set_cursor_y(int p_where) { orderlist_cursor_y=p_where; };
	void orderlist_set_cursor_x(int p_where) { orderlist_cursor_x=p_where; };
	
	void orderlist_get_cursor_string(int p_order,char p_str[4]);
	/* CHANNEL SOLO/MUTE */

        void channel_mute_toggle(int p_track);
        void channel_solo_toggle(int p_track);
        void current_channel_mute_toggle();
        void current_channel_solo_toggle();
        
        /* CHORD MODE */
        
        void chord_mode_enter();
        void chord_mode_press_note(int p_note);
        void chord_mode_release_note(int p_note);
        void chord_mode_cancel();
        
	/* REDRAW OPTIMIZATION CHECKS */

	bool flag_redraw_row() { return (redraw_flags & TRACKER_REDRAW_ROW); };
	bool flag_redraw_all() { return (redraw_flags==0) || (redraw_flags & TRACKER_REDRAW_ALL); };
	bool flag_redraw_top() { return (redraw_flags & TRACKER_REDRAW_TOP); };
        bool flag_redraw_playing_row() { return (redraw_flags & TRACKER_REDRAW_PLAYING_ROW); };

	int get_playing_row() { return playing_row; };
	int get_old_playing_row() { return playing_row_old; };

	/* REDRAW OPTIMIZATIONS SET */

	void clear_redraw_flags() { redraw_flags=0; };

	void set_flag_redraw_row() { redraw_flags|=TRACKER_REDRAW_ROW; };
	void set_flag_redraw_all() { redraw_flags|=TRACKER_REDRAW_ALL; };
	void set_flag_redraw_top() { redraw_flags|=TRACKER_REDRAW_TOP; };
	void set_flag_redraw_playing_row() { redraw_flags|=TRACKER_REDRAW_PLAYING_ROW; };

	void notify_set_playing_row(int whichrow) { playing_row_old=playing_row; playing_row=whichrow; playing=true; set_flag_redraw_playing_row(); };
	void notify_stopped_playing(int whichrow) { playing_row_old=playing_row; playing_row=-1; playing=false; set_flag_redraw_playing_row(); };

	bool insert_note_value_at_cursor(int p_note);
        /* MISC FLAGS */
	
	bool is_player_active () { return playing; };
	void set_song_follow(bool p_follow) { song_follow = p_follow; }
	bool is_song_follow() { return song_follow; }
	bool follow_to_current_pos();

	void perform_action(Pattern_Actions p_action); //simplification, gui helper too

	Editor(Song *p_song,Player *p_player);
	~Editor();
};

#endif
