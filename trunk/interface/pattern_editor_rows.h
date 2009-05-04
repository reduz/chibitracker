//
// C++ Interface: pattern_editor_rows
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PATTERN_EDITOR_ROWS_H
#define PATTERN_EDITOR_ROWS_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

#include "base/widget.h"
#include "tracker/editor.h"

using namespace GUI;
class PatternEditorRows : public Widget {
	
	Editor *editor;
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual Size get_minimum_size_internal();
	
	int last_pattern_played;
	int last_row_played;
public:
	
	void check_play_cursor();
	PatternEditorRows(Editor *p_editor);
	~PatternEditorRows();

};

#endif
