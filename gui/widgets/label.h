//
// C++ Interface: label
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUILABEL_H
#define PIGUILABEL_H

#include "base/widget.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Label : public Widget {
public:	
	
	enum Align {
		
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};
private:
	Align align;
	
	String text;
	
	Size get_minimum_size_internal();
protected:

	virtual String get_type();	
public:
	
	//void mouse_enter() { update(); } for testing
	
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	
	void set_align(Align p_align);
	Align get_align();
	
	void set_text(const String& p_string);
	String get_text();
	
	Label(String p_text="",Align p_align=ALIGN_LEFT);

    ~Label();

};

}

#endif
