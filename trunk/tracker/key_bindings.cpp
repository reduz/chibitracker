//
// C++ Implementation: key_bindings
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "key_bindings.h"

KeyBind* KeyBind::singleton=0;


int KeyBind::get_keybind_count() {
	
	return KB_MAX;	
}
unsigned int KeyBind::get_keybind_code(int p_index) {
	
	if (p_index<0 || p_index>=KB_MAX)
		return 0;
	
	return singleton->key_binds[p_index].bind_code;
}

void KeyBind::set_keybind_code(int p_index,unsigned int p_code) {
	
	if (p_index<0 || p_index>=KB_MAX)
		return;
	
	singleton->key_binds[p_index].bind_code=p_code;
	
}
const char* KeyBind::get_keybind_description(int p_index) {
	
	if (p_index<0 || p_index>=KB_MAX)
		return "Error";
	
	return singleton->key_binds[p_index].description;
	
}

unsigned int KeyBind::get(KeyBindList p_bind) {

	if (!singleton)
		return NO_BIND;

	return singleton->key_binds[p_bind].bind_code;
}
void KeyBind::set(KeyBindList p_bind,unsigned int p_bind_code) {

	if (!singleton)
		return;

	if (!singleton)
		return ;

	singleton->key_binds[p_bind].bind_code=p_bind_code;
	
}

KeyBindList KeyBind::get_bind(unsigned int p_bind_code) {
	
	for (int i=0;i<KB_MAX;i++) {
		if (singleton->key_binds[i].bind_code==p_bind_code)
			return (KeyBindList)i;
	}
	
	return KB_MAX;
}

KeyBind::KeyBind() {

	singleton=this;

	key_binds[KB_CURSOR_MOVE_UP].description="Cursor - Move Up";
	key_binds[KB_CURSOR_MOVE_DOWN].description="Cursor - Move Down";
	key_binds[KB_CURSOR_MOVE_LEFT].description="Cursor - Move Left";
	key_binds[KB_CURSOR_MOVE_RIGHT].description="Cursor - Move Right";
	key_binds[KB_CURSOR_PAGE_UP].description="Cursor - Page Up";
	key_binds[KB_CURSOR_PAGE_DOWN].description="Cursor - Page Down";
	
	key_binds[KB_CURSOR_MOVE_UP_1_ROW].description="Cursor - Move Up One Row";
	key_binds[KB_CURSOR_MOVE_DOWN_1_ROW].description="Cursor - Move Down One Row";
	
	key_binds[KB_CURSOR_MOVE_TRACK_LEFT].description="Cursor - Move Left a Whole Track";
	key_binds[KB_CURSOR_MOVE_TRACK_RIGHT].description="Cursor - Move Right a Whole Track";
	
	key_binds[KB_CURSOR_HOME].description="Cursor - Home";
	key_binds[KB_CURSOR_END].description="Cursor - End";
	key_binds[KB_CURSOR_INSERT].description="Cursor - Insert";
	key_binds[KB_CURSOR_DELETE].description="Cursor - Delete";
	key_binds[KB_CURSOR_TAB].description="Cursor - Tab";
	key_binds[KB_CURSOR_BACKTAB].description="Cursor - BackTab";
	key_binds[KB_CURSOR_FIELD_CLEAR].description="Cursor - Clear Field";
	
	key_binds[KB_PATTERN_PAN_WINDOW_UP].description="Pattern - Pan Window Up";
	key_binds[KB_PATTERN_PAN_WINDOW_DOWN].description="Pattern - Pan Window Down";
	
	
	key_binds[KB_PATTERN_CURSOR_INSERT_NOTE_OFF_VOLPAN_TOGGLE].description="Pattern - Insert Note Off/Toggle PanMask";
	key_binds[KB_PATTERN_CURSOR_INSERT_NOTE_CUT].description="Pattern - Cursor Insert Note Cut";
	key_binds[KB_PATTERN_CURSOR_PLAY_FIELD].description="Pattern - Cursor Play Field";
	key_binds[KB_PATTERN_CURSOR_PLAY_ROW].description="Pattern - Cursor Play Row";
	
	key_binds[KB_PATTERN_MARK_BLOCK_BEGIN].description="Pattern - Mark Block Begin";
	key_binds[KB_PATTERN_MARK_BLOCK_END].description="Pattern - Mark Block End";
	key_binds[KB_PATTERN_MARK_COLUMN_ALL].description="Pattern - Mark Column/All";
	key_binds[KB_PATTERN_BLOCK_COPY].description="Pattern - Block Copy";
	key_binds[KB_PATTERN_BLOCK_PASTE_INSERT].description="Pattern - Block Paste Insert";
	key_binds[KB_PATTERN_BLOCK_PASTE_OVERWRITE].description="Pattern - Block Paste Overwrite";
	key_binds[KB_PATTERN_BLOCK_PASTE_MIX].description="Pattern - Block Paste Mix";
	key_binds[KB_PATTERN_BLOCK_UNMARK].description="Pattern - Block Unmark";
	key_binds[KB_PATTERN_BLOCK_CUT].description="Pattern - Block Cut";
	key_binds[KB_PATTERN_RAISE_NOTES].description="Pattern - Block/Cursor Raise";
	key_binds[KB_PATTERN_LOWER_NOTES].description="Pattern - Block/Cursor Lower";
	key_binds[KB_PATTERN_RAISE_12_NOTES].description="Pattern - Block/Cursor Raise Octave";
	key_binds[KB_PATTERN_LOWER_12_NOTES].description="Pattern - Block/Cursor Lower Octave";
	key_binds[KB_PATTERN_TIME_SCALE].description="Pattern - Block Time Scale";
	key_binds[KB_PATTERN_TIME_SCALE_LINEAR].description="Pattern - Block Time Scale (Linear)";
	key_binds[KB_PATTERN_TRANSPOSE_MENU].description="Pattern - Reassign Notes Menu";
		
	key_binds[KB_PATTERN_BLOCK_SET_CURRENT_INSTRUMENT].description="Pattern - Block Set Instrument";
	key_binds[KB_PATTERN_BLOCK_SET_CURRENT_VOLUME].description="Pattern - Block Set Volume";
	key_binds[KB_PATTERN_BLOCK_WIPE_STRAY_VOLUMES].description="Pattern - Block Wipe Stray Volumes";
	key_binds[KB_PATTERN_BLOCK_RAMP_VOLUMES].description="Pattern - Block Ramp Volumes";
	key_binds[KB_PATTERN_BLOCK_AMPLIFY_VOLUMES].description="Pattern - Block Amplify Volumes";
	key_binds[KB_PATTERN_BLOCK_RAMP_WIPE_EFFECTS].description="Pattern - Block Ramp/Wipe Effects";
	key_binds[KB_PATTERN_BLOCK_DOUBLE_BLOCK_LENGTH].description="Pattern - Double Block Length";
	key_binds[KB_PATTERN_BLOCK_HALVE_BLOCK_LENGTH].description="Pattern - Halve Block Length";
	key_binds[KB_PATTERN_FIELD_TOGGLE_MASK].description="Pattern - Cursor Toggle Mask";
	key_binds[KB_PATTERN_FIELD_COPY_CURRENT].description="Pattern - Cursor Remember Field";
	key_binds[KB_PATTERN_FIELD_WRITE_CURRENT].description="Pattern - Write Field Memory";
	key_binds[KB_PATTERN_NEXT].description="Pattern - Goto Next Pattern";
	key_binds[KB_PATTERN_PREV].description="Pattern - Goto Prev Pattern";
	key_binds[KB_PATTERN_NEXT_ORDER].description="Pattern - Goto Next Order";
	key_binds[KB_PATTERN_PREV_ORDER].description="Pattern - Goto Prev Order";
	key_binds[KB_PATTERN_MUTE_TOGGLE_CHANNEL].description="Pattern - Toggle Mute Channel";
	key_binds[KB_PATTERN_SOLO_TOGGLE_CHANNEL].description="Pattern - Toggle Solo Channel";
	key_binds[KB_PATTERN_NEXT_INSTRUMENT].description="Pattern - Next Instrument";
	key_binds[KB_PATTERN_PREV_INSTRUMENT].description="Pattern - Prev Instrument";
	
	key_binds[KB_ORDERLIST_INSERT_SEPARATOR].description="Orderlist Insert Break";
	key_binds[KB_PATTERN_UNDO].description="Pattern - Undo Latest Action";
	key_binds[KB_PIANO_C0].description="Virtual Piano - C-0";
	key_binds[KB_PIANO_Cs0].description="Virtual Piano - C#0";
	key_binds[KB_PIANO_D0].description="Virtual Piano - D-0";
	key_binds[KB_PIANO_Ds0].description="Virtual Piano - D#0";
	key_binds[KB_PIANO_E0].description="Virtual Piano - E-0";
	key_binds[KB_PIANO_F0].description="Virtual Piano - F-0";
	key_binds[KB_PIANO_Fs0].description="Virtual Piano - F#0";
	key_binds[KB_PIANO_G0].description="Virtual Piano - G-0";
	key_binds[KB_PIANO_Gs0].description="Virtual Piano - G#0";
	key_binds[KB_PIANO_A0].description="Virtual Piano - A-0";
	key_binds[KB_PIANO_As0].description="Virtual Piano - A#0";
	key_binds[KB_PIANO_B0].description="Virtual Piano - B-0";
	
	key_binds[KB_PIANO_C1].description="Virtual Piano - C-1";
	key_binds[KB_PIANO_Cs1].description="Virtual Piano - C#1";
	key_binds[KB_PIANO_D1].description="Virtual Piano - D-1";
	key_binds[KB_PIANO_Ds1].description="Virtual Piano - D#1";
	key_binds[KB_PIANO_E1].description="Virtual Piano - E-1";
	key_binds[KB_PIANO_F1].description="Virtual Piano - F-1";
	key_binds[KB_PIANO_Fs1].description="Virtual Piano - F#1";
	key_binds[KB_PIANO_G1].description="Virtual Piano - G-1";
	key_binds[KB_PIANO_Gs1].description="Virtual Piano - G#1";
	key_binds[KB_PIANO_A1].description="Virtual Piano - A-1";
	key_binds[KB_PIANO_As1].description="Virtual Piano - A#1";
	key_binds[KB_PIANO_B1].description="Virtual Piano - B-1";
	
	key_binds[KB_PIANO_C2].description="Virtual Piano - C-2";
	key_binds[KB_PIANO_Cs2].description="Virtual Piano - C#2";
	key_binds[KB_PIANO_D2].description="Virtual Piano - D-2";
	key_binds[KB_PIANO_Ds2].description="Virtual Piano - D#2";
	key_binds[KB_PIANO_E2].description="Virtual Piano - E-2";

	key_binds[KB_PATTERN_CURSOR_SPACING_0].description="Pattern - Set Cursor Spacing to 0";
	key_binds[KB_PATTERN_CURSOR_SPACING_1].description="Pattern - Set Cursor Spacing to 1";
	key_binds[KB_PATTERN_CURSOR_SPACING_2].description="Pattern - Set Cursor Spacing to 2";
	key_binds[KB_PATTERN_CURSOR_SPACING_3].description="Pattern - Set Cursor Spacing to 3";
	key_binds[KB_PATTERN_CURSOR_SPACING_4].description="Pattern - Set Cursor Spacing to 4";
	key_binds[KB_PATTERN_CURSOR_SPACING_5].description="Pattern - Set Cursor Spacing to 5";
	key_binds[KB_PATTERN_CURSOR_SPACING_6].description="Pattern - Set Cursor Spacing to 6";
	key_binds[KB_PATTERN_CURSOR_SPACING_7].description="Pattern - Set Cursor Spacing to 7";
	key_binds[KB_PATTERN_CURSOR_SPACING_8].description="Pattern - Set Cursor Spacing to 8";
	key_binds[KB_PATTERN_CURSOR_SPACING_9].description="Pattern - Set Cursor Spacing to 9";
	
	key_binds[KB_OCTAVE_RAISE].description="Global - Raise Editing Octave";
	key_binds[KB_OCTAVE_LOWER].description="Global - Lower Editing Octave";
	
	key_binds[KB_GOTO_PATTERN_SCREEN].description="Global - Go to Pattern Screen (Pattern)";
	key_binds[KB_GOTO_PATTERN_SCREEN_ORDERLIST].description="Global - Go to Pattern Screen (Orders)";
	key_binds[KB_GOTO_SAMPLE_SCREEN].description="Global - Go to Samples Screen";
	key_binds[KB_GOTO_INSTRUMENT_SCREEN].description="Global - Go to Instruments Screen";
	key_binds[KB_GOTO_VARIABLES_SCREEN].description="Global - Go to Variables Screen";
	key_binds[KB_PATTERN_SCREEN_OPTIONS].description="Pattern - Options Dialog";
	key_binds[KB_PATTERN_SCREEN_HIDE_ORDERLIST].description="Pattern - Hide Orderlist";
	key_binds[KB_PATTERN_SCREEN_SONG_FOLLOW].description="Pattern - Toggle Song Follow";
	
	key_binds[KB_PLAYER_PLAY_SONG].description="Player - Play Song";
	key_binds[KB_PLAYER_STOP_SONG].description="Player - Stop Song";
	key_binds[KB_PLAYER_FF_SONG].description="Player - FastForward Song";
	key_binds[KB_PLAYER_RW_SONG].description="Player - Rewind Song";
	key_binds[KB_PLAYER_PLAY_PATTERN].description="Player - Play Pattern (Loop)";
	key_binds[KB_PLAYER_PLAY_CURSOR].description="Player - Play From Cursor";
	key_binds[KB_PLAYER_PLAY_ORDER].description="Player - Play From Current Order";
	key_binds[KB_CURSOR_WRITE_MASK].description="Cursor - Write Mask";
	
	key_binds[KB_FILE_OPEN].description="File - Open";
	key_binds[KB_FILE_SAVE].description="File - Save";
	key_binds[KB_FILE_SAVE_AS].description="File - Save As";
	key_binds[KB_DISPLAY_TOGGLE_FULLSCREEN].description="Display - Toggle FullScreen";
	key_binds[KB_QUIT].description="ChibiTracker - Quit";
	
}


KeyBind::~KeyBind()
{
}


