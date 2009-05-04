//
// C++ Interface: main_vu
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAIN_VU_H
#define MAIN_VU_H

#include "base/widget.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class MainVU : public Widget {
	
	int val_l;
	int val_r;
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	
public:
	enum { VU_MAX=1024 };
	
	
	void set(int p_val_l,int p_val_r); //MAX is VU_MAX
	void compute(int p_time_elapsed_ms);
	
	MainVU();
	~MainVU();
};

#endif
