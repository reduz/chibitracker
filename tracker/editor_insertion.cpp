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
                          editor_insertion.cpp  -  description
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
#include "key_bindings.h"

#define IS_LETTER(value) ((value>='A') && (value<='Z') || (value>='a') && (value<='z'))
#define IS_NUMBER(value) ((value>='0') && (value<='9'))
#define IS_HEX_NUMBER(value) ( ((value>='0') && (value<='9')) || ((value>='A') && (value<='F')) )
#define CHAR_TO_HEX(value) ((value>='0') && (value<='9'))?(value-'0'):((value>='A') && (value<='F'))?(10+(value-'A')):0
#define UPPERC(value) (( (value>='a') && (value<='z'))?(value-('a'-'A')):value)
//a few macros!



//insert data stuff


bool Editor::insert_note_value_at_cursor(int p_note) {


	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n = get_current_note();
	n.note=p_note;
	//remember mask
	last_note=n.note;
	//and APPLY mask
	if (volume_mask) n.volume=last_volume;

	if (command_mask) {

		n.command=last_command;
		n.parameter=last_parameter;
	}

	if (instrument_mask) {

		n.instrument=last_instrument;
	}

	set_current_note( n );
	//cursor moves
	cursor_move_down(cursor_spacing);
	player->play_note( cursor_x , n,true );

	return true;
}

int Editor::get_note_from_charcode(unsigned int p_charcode) {
	
	if (p_charcode>=KB_PIANO_MIN && p_charcode<=KB_PIANO_MAX) {
		
		Uint8 tmpinc=p_charcode-KB_PIANO_MIN;
		Uint8 tmpnote=(12*default_octave+tmpinc);	
		if (tmpnote>=Note::NOTES) tmpnote=Note::NOTES-1;
		
		return tmpnote;
		
	}	
	
	return -1;
}


bool Editor::insert_note_at_cursor(unsigned int scode) {
	
	KeyBindList bind=KeyBind::get_bind( scode );
	if (bind==KB_MAX)
		return false;
	
	if (bind==KB_PATTERN_CURSOR_INSERT_NOTE_OFF_VOLPAN_TOGGLE) {
	    
		insert_noteoff_at_cursor();
		return true;
	    
	} else if (bind==KB_PATTERN_CURSOR_INSERT_NOTE_CUT) {
	    
		insert_notecut_at_cursor();
		return true;
		
	} else if (bind==KB_PATTERN_CURSOR_INSERT_SCRIPT) {
		
		insert_script_at_cursor();
		return true;
		
	} else if (bind>=KB_PIANO_MIN && bind<=KB_PIANO_MAX) {
		Note n = get_current_note();
		if (n.note==Note::SCRIPT) { n.clear(); set_current_note( n ); }
		
		Uint8 tmpinc=bind-KB_PIANO_MIN;
		Uint8 tmpnote=(12*default_octave+tmpinc);	
		if (tmpnote>=Note::NOTES) tmpnote=Note::NOTES-1;

		insert_note_value_at_cursor(tmpnote);
		return true;
	} else if (bind==KB_PATTERN_CURSOR_PLAY_FIELD) {
		
		Note n=get_current_note();
		player->play_note( cursor_x , n,true );
		cursor_move_down( cursor_spacing );
		return true;
	} else if (bind==KB_PATTERN_CURSOR_PLAY_ROW) {
		
		Pattern *p=song->get_pattern( current_pattern );
		if (!p)
			return false;
	
		for (int i=0;i<Pattern::WIDTH;i++) {
			Note n=p->get_note( i, cursor_y );
			player->play_note( i , n );
		}
		cursor_move_down( cursor_spacing );
		
		return true;
	}
	
	
	return false;

}

void Editor::insert_octave_at_cursor(Uint8 octave) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	if (n.note>=Note::NOTES) return;
	
	n.note=(octave*12+(n.note) % 12); // 0 = nothing
	set_current_note(n);
	cursor_move_down();
}

void Editor::insert_instrument_at_cursor_field1(Uint8 instrument) {

	int tmp_inst;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	tmp_inst=n.instrument;

	if (tmp_inst==Note::EMPTY) tmp_inst=0;
        else tmp_inst++;

	tmp_inst=(tmp_inst % 10);
	tmp_inst=tmp_inst+instrument*10;

	if (tmp_inst==0) tmp_inst=Note::EMPTY;
	else {

		if (tmp_inst>99) tmp_inst=99;
		tmp_inst--;
	}


	n.instrument=tmp_inst;
	last_instrument=tmp_inst;
	set_current_note(n);

	cursor_move_field_right();
	
}

void Editor::insert_instrument_at_cursor_field2(Uint8 instrument) {

	int tmp_inst;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	tmp_inst=n.instrument;

	if (tmp_inst==Note::EMPTY) tmp_inst=0;
        else tmp_inst++;

	tmp_inst=(tmp_inst / 10)*10;
	tmp_inst=tmp_inst+instrument;

	if (tmp_inst==0) tmp_inst=Note::EMPTY;
	else {

		if (tmp_inst>99) tmp_inst=99;
		tmp_inst--;
	}



	n.instrument=tmp_inst;
	last_instrument=tmp_inst;
	set_current_note(n);
	cursor_move_down();
	cursor_move_field_left();
	
}


void Editor::insert_script_source_at_cursor_field1(unsigned int unicode) {

	int tmp_inst;
	int instrument=0;
    
	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	tmp_inst=n.instrument;
    
	if (unicode == '-' || unicode == '+')
	    n.script_source_sign = unicode;
	else {
	    n.script_source_sign = '\0';
	    instrument=(unicode-'0');
	}
    
	
	if (tmp_inst==Note::EMPTY) tmp_inst=0;
        else tmp_inst++;

	tmp_inst=(tmp_inst % 10);
	tmp_inst=tmp_inst+instrument*10;

	if (tmp_inst==0) tmp_inst=Note::EMPTY;
	else {

		if (tmp_inst>99) tmp_inst=99;
		tmp_inst--;
	}


	n.instrument=tmp_inst;
	set_current_note(n);

	cursor_move_field_right();
	
}

void Editor::insert_script_source_at_cursor_field2(Uint8 instrument) {

	int tmp_inst;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	tmp_inst=n.instrument;

	if (tmp_inst==Note::EMPTY) tmp_inst=0;
        else tmp_inst++;

	tmp_inst=(tmp_inst / 10)*10;
	tmp_inst=tmp_inst+instrument;

	if (tmp_inst==0) tmp_inst=Note::EMPTY;
	else {

		if (tmp_inst>99) tmp_inst=99;
		tmp_inst--;
	}


	n.instrument=tmp_inst;
	set_current_note(n);
	
	cursor_move_field_right();
}


void Editor::insert_script_instrument_at_cursor_field1(Uint8 instrument) {

	int tmp_inst;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	tmp_inst=n.volume;

	if (tmp_inst==Note::EMPTY) tmp_inst=0;
        else tmp_inst++;

	tmp_inst=(tmp_inst % 10);
	tmp_inst=tmp_inst+instrument*10;

	if (tmp_inst==0) tmp_inst=Note::EMPTY;
	else {

		if (tmp_inst>99) tmp_inst=99;
		tmp_inst--;
	}


	n.volume=tmp_inst;
	set_current_note(n);

	cursor_move_field_right();
	
}

void Editor::insert_script_instrument_at_cursor_field2(Uint8 instrument) {

	int tmp_inst;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	tmp_inst=n.volume;

	if (tmp_inst==Note::EMPTY) tmp_inst=0;
        else tmp_inst++;

	tmp_inst=(tmp_inst / 10)*10;
	tmp_inst=tmp_inst+instrument;

	if (tmp_inst==0) tmp_inst=Note::EMPTY;
	else {

		if (tmp_inst>99) tmp_inst=99;
		tmp_inst--;
	}



	n.volume=tmp_inst;
	set_current_note(n);
	
	cursor_move_field_right();
	
}


void Editor::insert_volume_at_cursor_field1(Uint8 volume) {

	Uint8 tmpvol,rightvol;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
        tmpvol=n.volume;
	
	if (tmpvol==Note::EMPTY) {

		rightvol=0;

	} else if (tmpvol<65) {

		rightvol=tmpvol % 10;

	} else if (tmpvol<125) {

		rightvol=(tmpvol-65) % 10;

	} else if (tmpvol<193) {

		rightvol=(tmpvol-128) % 10;

	} else if (tmpvol<213) {

		rightvol=(tmpvol-193) % 10;
	}

	if (IS_NUMBER(volume)) {

                tmpvol=(volume-'0')*10+rightvol;
		if (tmpvol>64) tmpvol=64;  	
		if (volume_pan_mode) tmpvol+=128;

	} else if ((volume>='A') && (volume<='F')) {

		tmpvol=65+(volume-'A')*10+rightvol;				

	} else if ((volume>='G') && (volume<='H')) {

		tmpvol=193+(volume-'G')*10+rightvol;				

	} else return;

	n.volume=tmpvol;
	last_volume=tmpvol;
	set_current_note(n);

	cursor_move_field_right();
	
}

void Editor::insert_volume_at_cursor_field2(Uint8 volume) {

	Uint8 tmpvol,leftvol;

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
        tmpvol=n.volume;


 	if (tmpvol==Note::EMPTY) {

		leftvol=0;

	} else if (tmpvol<65) {

		leftvol=tmpvol/10*10;

	} else if (tmpvol<125) {

		leftvol=(((tmpvol-65) / 10)*10)+65;

	} else if (tmpvol<193) {

		leftvol=((tmpvol-128) / 10)*10;

	} else if (tmpvol<213) {

		leftvol=(((tmpvol-193) / 10)*10)+193;
	}

	if (leftvol<65) {

		tmpvol=leftvol+volume;
		if (tmpvol>64) tmpvol=64;
		if (volume_pan_mode) tmpvol+=128;

	} else {

		tmpvol=leftvol+volume;
	}

	n.volume=tmpvol;
	last_volume=tmpvol;
	set_current_note(n);

	cursor_move_down();
	cursor_move_field_left();
	
}


void Editor::insert_command_at_cursor(char command) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	
	
	
	if (IS_LETTER(command)) { // A-Z command?

        	n.command=(command-'A');

	} else { // 0-9 command?
	
		n.command=26+(command-'0');
	}

	last_command=n.command;
	last_parameter=n.parameter;

	set_current_note(n);	
	cursor_move_down();


	
}

void Editor::insert_parameter_at_cursor_field1(Uint8 parameter) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.parameter=parameter*0x10+n.parameter % 0x10;
	last_command=n.command;
	last_parameter=n.parameter;
	
	set_current_note(n);

	cursor_move_field_right();
}

void Editor::insert_parameter_at_cursor_field2(Uint8 parameter) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.parameter=parameter+n.parameter/0x10*0x10;
	last_command=n.command;
	last_parameter=n.parameter;

	set_current_note(n);
	cursor_move_down();
	cursor_move_field_left();
}


void Editor::insert_script_command_at_cursor(char command) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	
	n.command = command=='Q' ? '^' : 'v';
	
	set_current_note(n);	
	cursor_move_field_right();
}

void Editor::insert_script_parameter_at_cursor_field1(Uint8 parameter) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.parameter=parameter*0x10+n.parameter % 0x10;
	last_command=n.command;
	last_parameter=n.parameter;
	
	set_current_note(n);

	cursor_move_field_right();
}

void Editor::insert_script_parameter_at_cursor_field2(Uint8 parameter) {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.parameter=parameter+n.parameter/0x10*0x10;
	last_command=n.command;
	last_parameter=n.parameter;

	set_current_note(n);
	cursor_move_down();
	cursor_move_field_left();
}


bool Editor::key_press_at_cursor(unsigned int p_scode,unsigned int p_unicode,bool p_shift) { // A-Z (caps) / 0-9
	
	int previous_cursor_x=cursor_x;
	int previous_cursor_y=cursor_y;
	bool success;
	bool modified;
	
	Note n=get_current_note();
	bool is_script=(n.note==Note::SCRIPT);
	
	if (!(success = handle_scancode(p_scode,p_shift,modified))) {
		
		switch (cursor_field) {
			
			case 0: { success=insert_note_at_cursor(p_scode); } break;
			case 1: { if ( !is_script && IS_NUMBER(p_unicode) ) insert_octave_at_cursor(p_unicode-'0'); } break;
			
			case 2: {
			    if ( !is_script && IS_NUMBER(p_unicode) ) insert_instrument_at_cursor_field1(p_unicode-'0');
			    else if ( is_script && (IS_NUMBER(p_unicode) || p_unicode == '-' || p_unicode == '+') ) insert_script_source_at_cursor_field1(p_unicode); } break;
			    
			case 3: {
			    if ( !is_script && IS_NUMBER(p_unicode) ) insert_instrument_at_cursor_field2(p_unicode-'0');
			    else if ( is_script && IS_NUMBER(p_unicode) ) insert_script_source_at_cursor_field2(p_unicode-'0'); } break;
			    
			case 4: {
			    if ( !is_script && (IS_NUMBER(p_unicode) || IS_LETTER(p_unicode)) ) insert_volume_at_cursor_field1(UPPERC(p_unicode));
			    else if ( is_script && IS_NUMBER(p_unicode) ) insert_script_instrument_at_cursor_field1(p_unicode-'0'); } break;
			    
			case 5: {
			    if ( !is_script && IS_NUMBER(p_unicode) ) insert_volume_at_cursor_field2(p_unicode-'0');
			    else if ( is_script && IS_NUMBER(p_unicode) ) insert_script_instrument_at_cursor_field2(p_unicode-'0'); } break;
			    
			case 6: {
			    if ( !is_script && IS_LETTER(UPPERC(p_unicode)) ) insert_command_at_cursor(UPPERC(p_unicode)); 
			    else if ( is_script && (UPPERC(p_unicode)=='Q' || UPPERC(p_unicode)=='A') ) insert_script_command_at_cursor(UPPERC(p_unicode)); } break;
			    
			case 7: {
			    if ( !is_script && IS_HEX_NUMBER(UPPERC(p_unicode)) ) insert_parameter_at_cursor_field1(CHAR_TO_HEX(UPPERC(p_unicode)));
			    else if ( is_script && IS_HEX_NUMBER(UPPERC(p_unicode)) ) insert_script_parameter_at_cursor_field1(CHAR_TO_HEX(UPPERC(p_unicode))); } break;
			    
			case 8: {
			    if ( !is_script && IS_HEX_NUMBER(UPPERC(p_unicode)) ) insert_parameter_at_cursor_field2(CHAR_TO_HEX(UPPERC(p_unicode)));
			    else if ( is_script && IS_HEX_NUMBER(UPPERC(p_unicode)) ) insert_script_parameter_at_cursor_field2(CHAR_TO_HEX(UPPERC(p_unicode))); } break;
		}
		
		modified=true;
		
		if ((cursor_field==4 || cursor_field==5) && KeyBind::get_bind( p_scode)==KB_PATTERN_CURSOR_INSERT_NOTE_OFF_VOLPAN_TOGGLE) {
			
			volume_pan_mode=!volume_pan_mode;
			modified=false;
		}
	}
	
	if ( modified ) {
		
		bool was_script = is_script;
		
		n=song->get_pattern(get_current_pattern())->get_note(previous_cursor_x, previous_cursor_y);
		is_script=(n.note==Note::SCRIPT);
		
		if (is_script) {
		    
		    song->get_pattern(get_current_pattern())->scripted_clone(previous_cursor_x, previous_cursor_y);
		    set_flag_redraw_all();
		    
		} else if (song->get_pattern(get_current_pattern())->update_scripted_clones_sourcing_channel(previous_cursor_x)) {
		    
		    set_flag_redraw_all();
		    
		}
		
		if (was_script && !is_script) {
		    
		    // get rid of cloned notes
		    
		    song->get_pattern(get_current_pattern())->scripted_clone_remove(previous_cursor_x, previous_cursor_y);
		    
		    // redo cloning (special case)
		    
		    for ( int y = 0; y < song->get_pattern(get_current_pattern())->get_length(); ++y )
			if ( song->get_pattern(get_current_pattern())->get_note( previous_cursor_x, y ).note == Note::SCRIPT )
			    song->get_pattern(get_current_pattern())->scripted_clone( previous_cursor_x, y );
		    
		    set_flag_redraw_all();
		    
		}
	}
	
	return success;
}


void Editor::insert_noteoff_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	if (cursor_field<2) {

		n.clear();
		n.note=Note::OFF;
		set_current_note(n);
		player->play_note( cursor_x , n,true );
		cursor_move_down();

	} else if ((cursor_field>3) && (cursor_field<6)) {

		volume_pan_mode=!volume_pan_mode;
	}

	
}
void Editor::insert_mask_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	switch (cursor_field) {

		case 0:{

			n.note=last_note;
		} break;
		case 1: break;
		case 2:
		case 3:{

			n.instrument=last_instrument;
		} break;
		case 4:
		case 5:{

			n.volume=last_volume;
		} break;
			
		case 6:
		case 7:
		case 8:{

			n.parameter=last_parameter;
			n.command=last_command;
		} break;
	}

	set_current_note(n);
	cursor_move_down();
}

void Editor::get_mask_from_cursor() {

	Note n=get_current_note();
	switch (cursor_field) {

		case 0:{
			
			last_note=n.note;
		} break;
		case 1: break;
		case 2:
		case 3:{

			last_instrument=n.instrument;
		} break;
		case 4:
		case 5:{

			last_volume=n.volume;
		} break;
			
		case 6:
		case 7:
		case 8:{

			last_parameter=n.parameter;
			last_command=n.command;
		} break;
	}
	set_current_note(n);
}


void Editor::insert_notecut_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.clear();
	n.note=Note::CUT;
	player->play_note( cursor_x , n,true );
	set_current_note(n);
	cursor_move_down();
	
}

void Editor::insert_script_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	if (n.note!=Note::SCRIPT) n.clear();
	n.note=Note::SCRIPT;
	set_current_note(n);
	cursor_field++;
	cursor_field++;
}

void Editor::perform_raise_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.raise();
	set_current_note(n);
}

void Editor::perform_lower_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	n.lower();
	set_current_note(n);
}

void Editor::perform_raise_octave_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	
	if (n.note<Note::NOTES) {
		n.note+=12;
		if (n.note>=Note::NOTES) 
			n.note=Note::NOTES-1;
	} else if (n.note==Note::SCRIPT) {
		if (n.parameter>=0xF0)
		    n.parameter=0xFF;
		else
		    n.parameter+=0x10;
	}
	
	set_current_note(n);
}

void Editor::perform_lower_octave_at_cursor() {

	push_current_pattern_to_undo_list(PATTERN_INPUT_DATA);
	Note n=get_current_note();
	
	if (n.note<Note::NOTES) {
		if (n.note>11) 
			n.note-=12;
		else
			n.note=0;
	} else if (n.note==Note::SCRIPT) {
		if (n.parameter>0xF)
			n.parameter-=0x10;
		else
			n.parameter=0;
	}
	
	set_current_note(n);
}



