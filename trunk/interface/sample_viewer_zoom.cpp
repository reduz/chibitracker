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
//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "widgets/value_label.h"
#include "sample_viewer_zoom.h"

#include <math.h>



void SampleViewerZoom::update_zoom_bar() {

	if (!sample_viewer || sample.is_null())
		return;
	
	int zoom_offset=sample_viewer->get_zoom_offset();
	int max_zoom_offset=sample_viewer->get_max_zoom_offset();
	int sample_data_size=sample_viewer->get_sample_size();
	int pagestep=sample_data_size-max_zoom_offset;

	zoom_view_offset->get_range()->set_max(sample_viewer->get_sample_size());
	zoom_view_offset->get_range()->set_page(pagestep);
	zoom_view_offset->get_range()->set(zoom_offset);

	updating_zoom=true;
	zoom_slider->get_range()->set(sample_viewer->get_zoom()/10.0);
	updating_zoom=false;


}

void SampleViewerZoom::zoom_slider_cbk(double p_val) {
	
	if (!sample_viewer || sample.is_null())
		return;

	if (updating_zoom) return;
	
	updating_zoom=true;
	float zoom_percent=p_val;
	zoom_percent/=100.0;

	float old_zoom=sample_viewer->get_zoom()/1000.0;
	float new_zoom=zoom_percent;

	float old_center=sample_viewer->get_zoom_offset()+((float)sample_viewer->get_sample_size()*old_zoom)/2.0;
	float new_offset=old_center-((float)sample_viewer->get_sample_size()*new_zoom)/2.0;

	sample_viewer->set_zoom((int)(new_zoom*1000.0));
	sample_viewer->set_zoom_offset((int)new_offset);

	update_zoom_bar();
	updating_zoom=false;

}

void SampleViewerZoom::zoom_more_cbk() {
	
	if (!sample_viewer || sample.is_null())
		return;
	if (sample_viewer->get_sample_size()==0)
		return;

	updating_zoom=true;

	float old_zoom=sample_viewer->get_zoom()/1000.0f;
	float new_zoom=old_zoom/2.0f;
	
	float old_center=sample_viewer->get_zoom_offset()+((float)sample_viewer->get_sample_size()*old_zoom)/2.0;
	float new_offset=old_center-((float)sample_viewer->get_sample_size()*new_zoom)/2.0;

	sample_viewer->set_zoom((int)(new_zoom*1000.0));
	sample_viewer->set_zoom_offset((int)new_offset);
	zoom_slider->get_range()->set(sample_viewer->get_zoom()/10.0);
	
	update_zoom_bar();
	updating_zoom=false;
}

void SampleViewerZoom::zoom_less_cbk() {
	
	if (!sample_viewer || sample.is_null())
		return;
	if (sample_viewer->get_sample_size()==0)
		return;

	updating_zoom=true;

	float old_zoom=sample_viewer->get_zoom()/1000.0;
	if (old_zoom<0.001)
		old_zoom=0.001;
	float new_zoom=old_zoom*2.0f;

	float old_center=sample_viewer->get_zoom_offset()+((float)sample_viewer->get_sample_size()*old_zoom)/2.0;
	float new_offset=old_center-((float)sample_viewer->get_sample_size()*new_zoom)/2.0;

	sample_viewer->set_zoom((int)(new_zoom*1000.0));
	sample_viewer->set_zoom_offset((int)new_offset);

	zoom_slider->get_range()->set(new_zoom*100.0);

	update_zoom_bar();
	updating_zoom=false;

}
void SampleViewerZoom::zoom_all_cbk() {
	
	if (!sample_viewer || sample.is_null())
		return;
	if (sample_viewer->get_sample_size()==0)
		return;

	updating_zoom=true;
	sample_viewer->set_zoom(1000);
	sample_viewer->set_zoom_offset(0);
	zoom_slider->get_range()->set(100.0);
	update_zoom_bar();
	updating_zoom=false;

}
void SampleViewerZoom::zoom_selection_cbk() {
	
	if (!sample_viewer || sample.is_null())
		return;
	if (sample_viewer->get_sample_size()==0)
		return;

	Selection *selection=sample_viewer->get_selection();
	if (!selection || !selection->is_active())
		return;

	updating_zoom=true;

	float selection_size=selection->get_end()-selection->get_begin();

	float zoom_request=selection_size/(float)sample_viewer->get_sample_size();
	sample_viewer->set_zoom((int)(zoom_request*1000));
	sample_viewer->set_zoom_offset(selection->get_begin());
	sample_viewer->update();
	zoom_slider->get_range()->set(sample_viewer->get_zoom()/10.0);
//	zoom_view_offset->setValue(selection->get_begin());

	update_zoom_bar();
	updating_zoom=false;

}
void SampleViewerZoom::zoom_offset_changed_cbk(double p_ofs) {

	if (!sample_viewer || sample.is_null())
		return;
	if (updating_zoom)
		return;
	sample_viewer->set_zoom_offset((int)p_ofs);
	////printf("Want new zoom %i\n",p_ofs);
}

void SampleViewerZoom::set_sample(Sample_ID p_sample) {

	sample=p_sample;
	if (sample_viewer)  {

		sample_viewer->set_sample(p_sample);
		update_zoom_bar();
	}
}


SampleViewerZoom::SampleViewerZoom(SampleViewer *p_sample_viewer) {

	sample_viewer=p_sample_viewer;
	
	zoom_view_offset = add(new HScrollBar,0);

	zoom_box = add(new HBoxContainer,0);

	zoom_box->add( new Label("Zoom"));
	zoom_more = zoom_box->add( new Button("+"),0 );
	
	zoom_slider = zoom_box->add( new HSlider,1);
	zoom_slider->get_range()->set(100);
	zoom_slider->get_range()->set_min(1);
	

	ValueLabel *vl=zoom_box->add( new ValueLabel,0 );
	vl->set_range( zoom_slider->get_range() );
	vl->set_suffix("%");

	zoom_less = zoom_box->add( new Button("-"),0 );
	zoom_all = zoom_box->add( new Button("all"),0 );
	zoom_select = zoom_box->add( new Button("sel"),0 );
	
	
	zoom_more->pressed_signal.connect( this, &SampleViewerZoom::zoom_more_cbk );
	zoom_less->pressed_signal.connect( this, &SampleViewerZoom::zoom_less_cbk );
	zoom_all->pressed_signal.connect( this, &SampleViewerZoom::zoom_all_cbk );
	zoom_select->pressed_signal.connect( this, &SampleViewerZoom::zoom_selection_cbk );
	
	zoom_view_offset->get_range()->value_changed_signal.connect( this, &SampleViewerZoom::zoom_offset_changed_cbk);
	
	zoom_slider->get_range()->value_changed_signal.connect( this, &SampleViewerZoom::zoom_slider_cbk);
	
	updating_zoom=false;

}

SampleViewerZoom::~SampleViewerZoom()
{
}


