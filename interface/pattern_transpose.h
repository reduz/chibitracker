//
// C++ Interface: pattern_transpose
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PATTERN_TRANSPOSE_H
#define PATTERN_TRANSPOSE_H

#include "base/window.h"
#include "bundles/combo_box.h"
#include "bundles/margin_group.h"
#include "tracker/tracker.h"
#include "widgets/button.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class PatternTranspose : public Window {

	ComboBox *trans[12];
	Editor *editor;
	
	void raise();
	void lower();
	void reset();
	void apply();
	Button *apply_b;
public:
	
	void show();
	Signal<>  update_pattern;
	PatternTranspose(Window *p_parent,Editor *p_editor);
	~PatternTranspose();

};

#endif
