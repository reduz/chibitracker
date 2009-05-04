//
// C++ Interface: window_top
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUIWINDOW_TOP_H
#define PIGUIWINDOW_TOP_H

#include "base/widget.h"

namespace GUI {

/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class WindowTop : public Widget {
	
	String text;
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	Size get_minimum_size_internal();

public:

	void set_text(String p_text);
	WindowTop(String p_text="");
	~WindowTop();
};

}

#endif
