//
// C++ Interface: sample_viewer_positions
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAMPLE_VIEWER_POSITIONS_H
#define SAMPLE_VIEWER_POSITIONS_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

#include "base/widget.h"
#include "mixer/sample_manager.h"
#include "mixer/mixer.h"
#include "interface/sample_viewer.h"
class SampleViewerPositions : public Widget {


	Size size;
	Mixer *mixer;
	
	void resize(const Size& p_new_size);
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	SampleViewer *viewer;
	
	Size get_minimum_size_internal();
public:
	
	void set_mixer(Mixer *p_mixer);
	void set_viewer(SampleViewer *p_viewr);
	
	
	SampleViewerPositions();
	
	~SampleViewerPositions();

};

#endif
