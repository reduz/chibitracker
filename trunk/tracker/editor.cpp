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
                          editor.cpp  -  description
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

#include "editor.h"

Pattern Editor::clipboard; 
int Editor::clipboard_width=0;

char noteincr_qwerty[32] = {'Z','S','X','D','C','V','G','B','H','N','J','M','Q','2','W','3','E','R','5','T','6','Y','7','U','I','9','O','0','P','[','=',']'};

int Editor::default_octave=4;

const char* Editor::notes[128] = {

	"C-0", "C#0", "D-0", "D#0", "E-0", "F-0", "F#0", "G-0", "G#0", "A-0", "A#0", "B-0",
	"C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1", "G-1", "G#1", "A-1", "A#1", "B-1",
	"C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2", "A-2", "A#2", "B-2",
	"C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3", "A-3", "A#3", "B-3",
	"C-4", "C#4", "D-4", "D#4", "E-4", "F-4", "F#4", "G-4", "G#4", "A-4", "A#4", "B-4",
	"C-5", "C#5", "D-5", "D#5", "E-5", "F-5", "F#5", "G-5", "G#5", "A-5", "A#5", "B-5",
	"C-6", "C#6", "D-6", "D#6", "E-6", "F-6", "F#6", "G-6", "G#6", "A-6", "A#6", "B-6",
	"C-7", "C#7", "D-7", "D#7", "E-7", "F-7", "F#7", "G-7", "G#7", "A-7", "A#7", "B-7",
	"C-8", "C#8", "D-8", "D#8", "E-8", "F-8", "F#8", "G-8", "G#8", "A-8", "A#8", "B-8",
	"C-9", "C#9", "D-9", "D#9", "E-9", "F-9", "F#9", "G-9", "G#9", "A-9", "A#9", "B-9",
	"C-A", "C#A", "D-A", "D#A", "E-A", "F-A", "F#A", "G-A"
};



Editor::Editor(Song *p_song,Player *p_player){

     	song=p_song;
	player=p_player;


	cursor_spacing=1;

	last_note=Note::EMPTY;
	last_instrument=1;
	last_volume=Note::EMPTY;
	last_command=Note::EMPTY;
	last_parameter=0;

	previous_cursor_y=0;
	previous_cursor_x=0;

	current_pattern=0;
        volume_pan_mode=false;
	clear_redraw_flags();
	column_offset=0;

	instrument_mask=true;
	volume_mask=true;
	command_mask=false;

	selection_active=false;
        undo_max=-1;
	playing=false;
	playing_row_old=-1;

	orderlist_cursor_x=0;
	orderlist_cursor_y=0;
	current_pattern=0;

	cursor_x=0;
	cursor_y=0;
	cursor_field=0;

	undo_display_pending=true;

	mark_active=false;

	shift_selection_in_progress=false;

	previous_shift=false;

	song_follow=false;
	row_offset=0;
	
	undo_op_count=0;
}

bool Editor::follow_to_current_pos() {
	
	if (!song_follow || player->get_play_mode()==Player::PLAY_NOTHING)
		return false;
	
	bool follow=false;
	int playback_pattern=player->get_current_pattern();
	if (playback_pattern!=current_pattern) {
		current_pattern=playback_pattern;
		follow=true;
	}
	
	if (cursor_y!=player->get_current_row()) {
		set_cursor_y(player->get_current_row()); 
		follow=true;
	}
	
	if (follow)
		clear_redraw_flags();
	return follow;
}

void Editor::adjust_view_to_cursor() {


	if (cursor_x>=Pattern::WIDTH) cursor_x=Pattern::WIDTH-1;
	if (cursor_y>=song->get_pattern( current_pattern )->get_length() )
		cursor_y=song->get_pattern( current_pattern )->get_length()-1;

	if (column_offset>cursor_x) {

		//[todo] gdk_copy_area for SCROLLING! would be neat in the future
		column_offset=cursor_x;
		set_flag_redraw_all();
        }

	if (column_offset+(get_visible_columns()-1)<cursor_x) {

		//[todo] gdk_copy_area for SCROLLING! would be neat in the future
		column_offset=cursor_x-(get_visible_columns()-1);
		set_flag_redraw_all();
	}


	if (row_offset+(get_visible_rows()) > song->get_pattern( current_pattern )->get_length()) {
		
		row_offset=song->get_pattern( current_pattern )->get_length()-get_visible_rows();
		if (row_offset<0)
			row_offset=0;
		set_flag_redraw_all();
	}
	
	if (row_offset>cursor_y) {


		//[todo] gdk_copy_area for SCROLLING! would be neat in the future
		row_offset=cursor_y;
		set_flag_redraw_all();
	}

	if (row_offset+(get_visible_rows()-1)<cursor_y) {


		//[todo] gdk_copy_area for SCROLLING! would be neat in the future
		row_offset=cursor_y-(get_visible_rows()-1);
		set_flag_redraw_all();
	}
	
}


void Editor::set_visible_columns(int p_visible) {
	
	visible_columns=p_visible;
}
void Editor::set_visible_rows(int p_visible) {
	
	visible_rows=p_visible;
}

int Editor::get_visible_columns() {

	return visible_columns;
}

int Editor::get_visible_rows() {

	return visible_rows;
}

void Editor::get_single_note_string(Uint8 p_note,char *p_str) {
	
	
	if (p_note==Note::EMPTY) {

		p_str[0]='.';
		p_str[1]='.';
		p_str[2]='.';


	} else if (p_note<Note::NOTES) {

		
		p_str[0]=notes[p_note][0];
		p_str[1]=notes[p_note][1];
		p_str[2]=notes[p_note][2];


	} else if (p_note==Note::OFF) {

		p_str[0]='=';
		p_str[1]='=';
		p_str[2]='=';

	} else if (p_note==Note::CUT) {

		p_str[0]='^';
		p_str[1]='^';
		p_str[2]='^';

	} else if (p_note==Note::SCRIPT) {

		p_str[0]='c';
		p_str[1]='p';
		p_str[2]='y';

	} else {
		p_str[0]='?';
		p_str[1]='?';
		p_str[2]='?';
	}

	p_str[3]=0;
	
	
}

void Editor::get_note_string(int column,int row, char *buf) {

	Note note;

//	const char *notestring;
//      	unsigned char tmpvol;

	//get note from editor;

	note=song->get_pattern(current_pattern)->get_note(column,row);

	/* Note */

	if (note.note==Note::EMPTY) {

		buf[0]='.';
		buf[1]='.';
		buf[2]='.';


	} else if (note.note<Note::NOTES) {

		
		buf[0]=notes[note.note][0];
		buf[1]=notes[note.note][1];
		buf[2]=notes[note.note][2];


	} else if (note.note==Note::OFF) {

		buf[0]='=';
		buf[1]='=';
		buf[2]='=';

	} else if (note.note==Note::CUT) {

		buf[0]='^';
		buf[1]='^';
		buf[2]='^';

	} else if (note.note==Note::SCRIPT) {

		buf[0]='c';
		buf[1]='p';
		buf[2]='y';

	} else {
		buf[0]='?';
		buf[1]='?';
		buf[2]='?';
	}

	buf[3]=' ';


	/* Instrument */

	if (note.instrument==Note::EMPTY) {
	    
	    buf[4] = '.';
	    buf[5] = '.';
	    
	} else {
	    
	    buf[4] = '0'+(note.instrument+1)/10;
	    buf[5] = '0'+(note.instrument+1) % 10;
	}
	
	if ( note.note==Note::SCRIPT && note.script_source_sign!='\0' )
	    buf[4] = note.script_source_sign;

	buf[6]=' ';
	
	
	/* Volume */

	unsigned char tmpvol=note.volume;

	if (note.note==Note::SCRIPT) {
	    
	    if (tmpvol==Note::EMPTY) {
		buf[7] = '.';
		buf[8] = '.';
	    } else {
		buf[7] = '0'+(tmpvol+1)/10;
		buf[8] = '0'+(tmpvol+1) % 10;
	    }
	    
	} else if (tmpvol<65) {

		buf[7] = '0'+tmpvol/10;
		buf[8] = '0'+tmpvol % 10;

	} else if (tmpvol<75) {

		buf[7] = 'A';
		buf[8] = '0'+(tmpvol-65) % 10;

	} else if (tmpvol<85) {

		buf[7] = 'B';
		buf[8] = '0'+(tmpvol-75) % 10;

	} else if (tmpvol<95) {

		buf[7] = 'C';
		buf[8] = '0'+(tmpvol-85) % 10;

	} else if (tmpvol<105) {

		buf[7] = 'D';
		buf[8] = '0'+(tmpvol-95) % 10;

	} else if (tmpvol<115) {

		buf[7] = 'E';
		buf[8] = '0'+(tmpvol-105) % 10;

	} else if (tmpvol<125) {

		buf[7] = 'F';
		buf[8] = '0'+(tmpvol-115) % 10;

	} else if (tmpvol<128) {

		buf[7] = '?';
		buf[8] = '?';

	} else if (tmpvol<193) {

		buf[6] = 'p';
		buf[7] = '0'+(tmpvol-128)/10;
		buf[8] = '0'+(tmpvol-128) % 10;


	} else if (tmpvol<203) {

		buf[7] = 'G';
		buf[8] = '0'+(tmpvol-193) % 10;

	} else if (tmpvol<213) {

		buf[7] = 'H';
		buf[8] = '0'+(tmpvol-203) % 10;

	} else if (tmpvol<Note::EMPTY) {

		buf[7] = '?';
		buf[8] = '?';
	} else {

		buf[7] = '.';
		buf[8] = '.';
	}


	buf[9]=' ';

	
	/* Command & Parameter */

	
	if (note.command==Note::EMPTY)
	    buf[10] = '.';
	else
	    buf[10] = note.note!=Note::SCRIPT ? 'A'+note.command : note.command;

	// Parameter 

	static const char hexn[] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	buf[11] = hexn[note.parameter/0x10];
	buf[12] = hexn[note.parameter&0xF];

	buf[13]=0;
	

}

/*
string Editor::get_pattern_note_string(Note p_note) {

	string note;

	if (p_note.note==Note::EMPTY) {

		note = "...";

	} else if (p_note.note<Note::NOTES) {

	       	note = notes[p_note.note];

	} else if (p_note.note==Note::OFF) {

		note = "===";
	} else if (p_note.note==Note::CUT) {

		note = "^^^";
	} else {

		note = "???";
	}

	return note;
}

string Editor::get_pattern_instrument_string(Note p_note) {

	string buf=" xx";

	if (p_note.instrument==Note::EMPTY) {

		buf[1] = '.';
		buf[2] = '.';

	} else {

		buf[1] = '0'+(p_note.instrument+1)/10;
		buf[2] = '0'+(p_note.instrument+1) % 10;
	}

	return buf;
}

string Editor::get_pattern_volume_string(Note p_note) {

	string buf=" ..";

	int tmpvol=p_note.volume;

	/// volume 

	if (tmpvol<65) {

		buf[1] = '0'+tmpvol/10;
		buf[2] = '0'+tmpvol % 10;


	} else if (tmpvol<75) {

		buf[1] = 'A';
		buf[2] = '0'+(tmpvol-65) % 10;

	} else if (tmpvol<85) {

		buf[1] = 'B';
		buf[2] = '0'+(tmpvol-75) % 10;

	} else if (tmpvol<95) {

		buf[1] = 'C';
		buf[2] = '0'+(tmpvol-85) % 10;

	} else if (tmpvol<105) {

		buf[1] = 'D';
		buf[2] = '0'+(tmpvol-95) % 10;

	} else if (tmpvol<115) {

		buf[1] = 'E';
		buf[2] = '0'+(tmpvol-105) % 10;

	} else if (tmpvol<125) {

		buf[1] = 'F';
		buf[2] = '0'+(tmpvol-115) % 10;

	} else if (tmpvol<128) {

		buf[1] = '?';
		buf[2] = '?';

	} else if (tmpvol<193) {

		buf[0] = 'P';
		buf[1] = '0'+(tmpvol-128)/10;
		buf[2] = '0'+(tmpvol-128) % 10;


	} else if (tmpvol<203) {

		buf[1] = 'G';
		buf[2] = '0'+(tmpvol-193) % 10;

	} else if (tmpvol<213) {

		buf[1] = 'H';
		buf[2] = '0'+(tmpvol-203) % 10;

	} else if (tmpvol<Note::EMPTY) {

		buf[1] = '?';
		buf[2] = '?';
	} else {

		buf[1] = '.';
		buf[2] = '.';
	}


	return buf;

}
string Editor::get_pattern_command_string(Note p_note) {

	string buf="...";
 	static const char hexn[] = {
        	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	

	if (p_note.command==Note::EMPTY) {

		buf[0] = '.';
	} else {

		buf[0] ='A'+p_note.command;

	}

	// Parameter 


	buf[1] = hexn[p_note.parameter/0x10];
	buf[2] = hexn[p_note.parameter&0xF];

	return buf;


}

*/

void Editor::channel_mute_toggle(int p_track) {

	if ((p_track<0) || (p_track>=Pattern::WIDTH)) return;

	
	song->set_channel_mute( p_track, !song->is_channel_mute( p_track) );
	redraw_flags=TRACKER_REDRAW_ALL;
	
	
}

void Editor::channel_solo_toggle(int p_track) {

	if ((p_track<0) || (p_track>=Pattern::WIDTH)) return;

        int i,muted_tracks;

        muted_tracks=0;

        // Count the muted tracks besides the current one
        for (i=0;i<Pattern::WIDTH;i++) {

		if (song->is_channel_mute( i)) muted_tracks++;

        }


	if ((muted_tracks==Pattern::WIDTH-1) && (!song->is_channel_mute( p_track))) {

                //[[UNMUTE ALL]]
                for (i=0;i<Pattern::WIDTH;i++) 			
			song->set_channel_mute( i, false );

        } else {

                for (i=0;i<Pattern::WIDTH;i++) {

			song->set_channel_mute( i, i!=p_track );
                }
		
        }
	
	redraw_flags=TRACKER_REDRAW_ALL;
}

void Editor::set_column_offset(int value) {

	column_offset=value;

	if (cursor_x<column_offset)
		cursor_x=column_offset;
	if (cursor_x>=(column_offset+get_visible_columns())) {

		cursor_x=(column_offset+get_visible_columns())-1;
	}
}
void Editor::set_row_offset(int value) {

	int new_row_offset=value;

	if (new_row_offset<0)
		new_row_offset=0;

	Pattern *pat=song->get_pattern(get_current_pattern());

	if (!pat)
		return;

	if (new_row_offset>(pat->get_length()-get_visible_rows()))
		new_row_offset=(pat->get_length()-get_visible_rows());

	row_offset=new_row_offset;
			
	if (cursor_y<row_offset)

		cursor_y=row_offset;
	if (cursor_y>=(row_offset+get_visible_rows())) {

		cursor_y=(row_offset+get_visible_rows())-1;
	}
}

void Editor::set_mark() {

        if (mark_active && (current_pattern==marked_pattern) && (cursor_y==marked_row)) {

		mark_active=false;

	} else {

		marked_pattern=current_pattern;
		marked_row=cursor_y;
		mark_active=true;
	}
}

void Editor::current_channel_mute_toggle() {

	channel_mute_toggle(cursor_x);

}
void Editor::current_channel_solo_toggle() {

	channel_solo_toggle(cursor_x);
}

Note Editor::get_current_note() {
	
	Pattern *p=song->get_pattern( current_pattern );
	Note n;
	if (!p)
		return(n);
	
	return p->get_note( cursor_x, cursor_y );
	
}
void Editor::set_current_note(const Note& p_note) {
	
	Pattern *p=song->get_pattern( current_pattern );
	if (!p)
		return;
	
	p->set_note( cursor_x, cursor_y, p_note );
	
}


void Editor::chord_mode_enter() {


}
void Editor::chord_mode_press_note(int p_note) {


}
void Editor::chord_mode_release_note(int p_note) {


}
void Editor::chord_mode_cancel() {


}


Editor::~Editor(){
}
