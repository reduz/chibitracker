//
// C++ Interface: pattern_editor_oscs
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PATTERN_EDITOR_OSCS_H
#define PATTERN_EDITOR_OSCS_H

#include "base/window.h"
#include "widgets/line_edit.h"
#include "tracker/tracker.h"

using namespace GUI;

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PatternEditorOscs : public Widget {
	
	Tracker *tracker;
	virtual Size get_minimum_size_internal();
	virtual void draw(const Point& p_global,const Size& p_size,const Rect& p_exposed);
	
public:
	PatternEditorOscs(Tracker *p_tracker);
	~PatternEditorOscs();

};

#endif
