//
// C++ Interface: arrow_button
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUIARROWBUTTON_H
#define PIGUIARROWBUTTON_H

#include "widgets/base_button.h"


namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class ArrowButton : public BaseButton {
	
	Direction dir;
	bool no_minsize;

	Size get_minimum_size_internal();
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);

	virtual String get_type();
	
public:
	void set_dir(Direction p_dir);
	Direction get_dir () const;
	
	ArrowButton(Direction p_dir,bool p_no_minsize=false);
	~ArrowButton();

};

}

#endif
