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

#ifndef SAMPLE_VIEWER_ZOOM_H
#define SAMPLE_VIEWER_ZOOM_H


#include "containers/box_container.h"
#include "widgets/button.h"
#include "widgets/label.h"
#include "widgets/scroll_bar.h"
#include "widgets/slider.h"

using namespace GUI;
#include "interface/sample_viewer.h"

/**
 *
 * Juan Linietsky
 **/
 
 
 
class SampleViewerZoom : public VBoxContainer {

	SampleViewer * sample_viewer;

        bool updating_zoom;

	HBoxContainer * zoom_box;

	Button *zoom_more;
	Button *zoom_less;
	Button *zoom_all;
	Button *zoom_select;

	HScrollBar *zoom_view_offset;

	Sample_ID sample;
	
	HSlider * zoom_slider;

	void zoom_more_cbk();
	void zoom_less_cbk();
	void zoom_all_cbk();
	void zoom_selection_cbk();
	void zoom_slider_cbk(double p_val);
	void zoom_offset_changed_cbk(double p_ofs);

public:

	void update_zoom_bar();

	void set_sample(Sample_ID p_sample);
	
	SampleViewerZoom(SampleViewer *p_sample_viewer);
	
	~SampleViewerZoom();
};

 
 
#endif
