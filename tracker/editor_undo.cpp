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
                          editor_undo.cpp  -  description
                             -------------------
    begin                : Sun Apr 15 2001
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

void Editor::push_current_pattern_to_undo_list(PatternOperationType p_reason) {

	bool dont_collapse=(p_reason==PATTERN_SELECTION_TIME_SCALE);
	push_pattern_to_undo_list(current_pattern,*song->get_pattern(current_pattern),p_reason,false,dont_collapse);
}

void Editor::push_pattern_to_undo_list(int p_index,const Pattern &p_pattern,PatternOperationType p_reason,bool p_is_redo,bool p_dont_collapse) {

	int i;

	if (undo_max==(MAX_UNDO_OPERATIONS-1)) {

		delete undo_buffer[undo_max].pattern_data;
	
	} else {

		undo_max++;
	}

	for (i=undo_max;i>0;i--) {

		undo_buffer[i]=undo_buffer[i-1];
	}


	
	undo_buffer[0].pattern_data = new Pattern();
	p_pattern.copy_to(undo_buffer[0].pattern_data);;
	undo_buffer[0].pattern_index=p_index;
	undo_buffer[0].reason=p_reason;
	undo_buffer[0].is_redo=p_is_redo;
	undo_display_pending=true;
	undo_op_count++;
}

void Editor::undo_index(int p_undo_index) {


	Pattern aux_pattern;
	int i;

	if ((p_undo_index<0) || (p_undo_index>undo_max)) return;

	song->get_pattern(current_pattern)->copy_to(&aux_pattern);

	// If we are trying to undo a change that was NOT made in the current pattern, we have to change patterns first.
	if (undo_buffer[p_undo_index].pattern_index != current_pattern)
		current_pattern = undo_buffer[p_undo_index].pattern_index;
	
	undo_buffer[p_undo_index].pattern_data->copy_to(song->get_pattern(current_pattern));
	for (i=0;i<undo_max;i++) {

		undo_buffer[i]=undo_buffer[i+1];
	}
	undo_max--;

	undo_display_pending=true;
	
}

int Editor::get_undo_max() {

	return undo_max+1;
}
int Editor::get_undo_count() {
	
	return undo_op_count;
}

int Editor::get_undo_pattern(int p_undo_index) {

	if ((p_undo_index<0) || (p_undo_index>undo_max)) return 0;

	return undo_buffer[p_undo_index].pattern_index;

}

Editor::PatternOperationType Editor::get_undo_info(int p_undo_index) {

	if ((p_undo_index<0) || (p_undo_index>undo_max)) return PATTERN_NONE;

	return undo_buffer[p_undo_index].reason;
}

bool Editor::get_undo_is_redo(int p_undo_index) {

	if ((p_undo_index<0) || (p_undo_index>undo_max)) return false;

	return undo_buffer[p_undo_index].is_redo;
}
