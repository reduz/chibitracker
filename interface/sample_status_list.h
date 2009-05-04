//
// C++ Interface: sample_status_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAMPLE_STATUS_LIST_H
#define SAMPLE_STATUS_LIST_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

#include "base/window.h"
#include "widgets/line_edit.h"
#include "widgets/range_owner.h"
#include "tracker/tracker.h"
using namespace GUI;

class SampleStatusList : public RangeOwner {
	
	Tracker *tracker;
	virtual Size get_minimum_size_internal();
	virtual void draw(const Point& p_global,const Size& p_size,const Rect& p_exposed);
	
public:
	
	SampleStatusList(Tracker *p_tracker);
	~SampleStatusList();

};

#endif
