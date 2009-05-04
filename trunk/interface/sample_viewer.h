/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : reduzio@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SAMPLE_VIEWER_H
#define SAMPLE_VIEWER_H

#include "base/widget.h"
#include "mixer/sample_manager.h"
#include "tracker/selection.h"

using namespace GUI;


/**
 *
 * Juan Linietsky
 **/
class SampleViewer : public Widget
{
	
	SampleManager *sm;
	
	Sample_ID sample;
	
	int screen_pos_to_sample_pos(int p_pos);

	//int get_factor();
	Size ssize;

	void draw_pos(int p_prev_min,int p_prev_max,int p_min,int p_max,int p_pos,bool p_in_loop,bool p_in_selection,int p_channel);

	virtual void resize(const Size& p_new_size);
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);  	
	virtual void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	virtual void mouse_doubleclick(const Point& p_pos,int p_modifier_mask);

	float zoom;
	int offset;

	bool grabbing_mouse;
	int closer_point; //1 or 2

	enum GrabType {

		SET_SELECTION,
		SET_SELECTION_BEGIN,
		SET_SELECTION_END
	};

	GrabType grab_type;


	Selection *selection;

	
	
public:


	//QSizePolicy sizePolicy() const;
	void sample_changed_notification() { update(); }
	int get_zoom(); ///< zoom .. 1000 = 100%
	void set_zoom(int p_factor); ///< zoom .. 1000 = 100%
	void set_selection(Selection *p_selection) { selection=p_selection; }
	Selection *get_selection() { return selection; }

	int get_sample_size();
	int get_zoom_offset();
	void set_zoom_offset(int p_offset);
	int get_max_zoom_offset();

	SampleViewer();
	void set_sample(Sample_ID p_sample);
	Sample_ID get_sample() { return sample; }
	
	int sample_to_screen_pos(int p_sample_pos);
	
	~SampleViewer();
};


#endif
