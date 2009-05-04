//
// C++ Implementation: pattern_editor_oscs
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pattern_editor_oscs.h"
#include "ctskin.h"
#include "base/painter.h"
#include "mixer/sample_manager.h"

Size PatternEditorOscs::get_minimum_size_internal() {

	return Size(0,constant( C_PATTERN_EDITOR_OSCS_HEIGHT ));
}
void PatternEditorOscs::draw(const Point& p_global,const Size& p_size,const Rect& p_exposed) {
	
	get_painter()->draw_fill_rect(Point(),p_size,color(COLOR_PATTERN_EDITOR_BG));
}


PatternEditorOscs::PatternEditorOscs(Tracker *p_tracker)
{
	tracker=p_tracker;
}


PatternEditorOscs::~PatternEditorOscs()
{
}


