/**************************f*************************************************
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
#include "base/painter.h"
#include "sample_viewer.h"
#include "mixer/audio_lock.h"
#include "error_macros.h"
#include "ctskin.h"
#include <stdio.h>
//#include <math.h>



int SampleViewer::sample_to_screen_pos(int p_sample_pos) {

	p_sample_pos-=offset;
		
	double pos=(double)p_sample_pos*ssize.width/((double)sm->get_size( sample )*zoom);
	return (int)pos;
	
}
int SampleViewer::screen_pos_to_sample_pos(int p_pos) {


	float pos=p_pos;

	pos=(double)pos*((double)sm->get_size( sample )*zoom)/(double)ssize.width;
	pos+=(float)offset;

	return (int)pos;

}

void SampleViewer::mouse_doubleclick(const Point& p_pos,int p_modifier_mask) {
	
	if (sample.is_null())
		return;
//	if (selection->is_active())
		//return;
	selection->set_point_1(0);
	selection->set_point_2(sm->get_size( sample ) -1);
	selection->set_active(true);
	update();
//	printf("tryuing to activate sel!\n");
	
	
}


void SampleViewer::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {
	
	if (!sm->check(sample) || sm->get_size(sample)==0)
		return;
	
	if (!selection)
		return;

	if (grabbing_mouse) {

		switch( grab_type ){

			case SET_SELECTION: {

				selection->set_point_2( screen_pos_to_sample_pos( p_pos.x ) );

			} break;
			case SET_SELECTION_BEGIN:
			case SET_SELECTION_END: {

				if (closer_point==1)
					selection->set_point_1( screen_pos_to_sample_pos( p_pos.x ) );
				else
					selection->set_point_2( screen_pos_to_sample_pos( p_pos.x ) );

			} break;
		}

		update();
	}
		
}


void SampleViewer::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	if (!sm->check(sample) || sm->get_size(sample)==0)
		return;
	
	if (!selection)
		return;



	if (p_press) {
		
		if (grabbing_mouse)
			return; //ignore it

		update();
	
		switch (p_button) {
	
			case BUTTON_LEFT: {
	
				grab_type=SET_SELECTION;
				selection->clear();
				selection->set_active(true);
				selection->set_max( sm->get_size( sample ) );
				selection->set_point_1( screen_pos_to_sample_pos( p_pos.x ) );
				selection->set_point_2( screen_pos_to_sample_pos( p_pos.x ) );
				grabbing_mouse=true;
	
	
	
			} break;
			case BUTTON_RIGHT: {
	
				grab_type=SET_SELECTION_END;
	
				if (!selection->is_active())
					break;
				grabbing_mouse=true;
	
				closer_point=(selection->get_point_1()>selection->get_point_2())?1:2;
	
			} break;
			case BUTTON_MIDDLE: {
				grab_type=SET_SELECTION_BEGIN;
				if (!selection->is_active())
					break;
				grabbing_mouse=true;
	
				closer_point=(selection->get_point_1()<selection->get_point_2())?1:2;
			} break;
		}
	} else { //button release	


		if (grabbing_mouse) {

			bool ungrab=false;

			switch(grab_type) {

				case SET_SELECTION: {

					if (p_button==BUTTON_LEFT)
						ungrab=true;

				} break;
				case SET_SELECTION_BEGIN: {

					if (p_button==BUTTON_MIDDLE)
						ungrab=true;



				} break;
				case SET_SELECTION_END: {

					if (p_button==BUTTON_RIGHT)
						ungrab=true;

				} break;
			}

			if (ungrab) {


				grabbing_mouse=false;
				if (selection->get_begin()==selection->get_end())
					selection->set_active(false);
				update();

			}

			grabbing_mouse=false;
		}

		

	} 

}





int SampleViewer::get_zoom() {

	return (int)(zoom*1000.0);
}
void SampleViewer::set_zoom(int p_factor) {

	float azoom=(float)p_factor/1000.0;
	
	float zoom_min=0.0001;
        if (azoom<zoom_min) //sucks
		azoom=zoom_min;
	if (azoom>1.0f)
		azoom=1.0f;

	zoom=azoom;

	update();
}

int SampleViewer::get_zoom_offset() {

	return offset;
}
void SampleViewer::set_zoom_offset(int p_offset) {

	offset=p_offset;
	if (offset<0) offset=0;
	if (offset>get_max_zoom_offset())
		offset=get_max_zoom_offset();

	update();
}
int SampleViewer::get_max_zoom_offset() {

	return sm->check(sample) ?sm->get_size( sample )- int ((double)sm->get_size( sample )*(double)zoom):0;
}

void SampleViewer::resize(const Size& p_new_size) {
	
	ssize=p_new_size;	
}
	
	
void SampleViewer::draw_pos(int p_prev_min,int p_prev_max,int p_min,int p_max,int p_pos,bool p_in_loop,bool p_in_selection,int p_channel) {
	
	int chans=sm->is_stereo( sample)?2:1;
	
	Painter *p=get_painter();
	Color bg_col;
	Color fg_col;
			
	if (p_in_loop) {
	
		bg_col=p_in_selection?color(COLOR_SAMPLE_EDITOR_LOOP_BG_SEL):color(COLOR_SAMPLE_EDITOR_LOOP_BG);
		fg_col=p_in_selection?color(COLOR_SAMPLE_EDITOR_LOOP_FG_SEL):color(COLOR_SAMPLE_EDITOR_LOOP_FG);
		
	} else {

		bg_col=p_in_selection?color(COLOR_SAMPLE_EDITOR_BG_SEL):color(COLOR_SAMPLE_EDITOR_BG);
		fg_col=p_in_selection?color(COLOR_SAMPLE_EDITOR_FG_SEL):color(COLOR_SAMPLE_EDITOR_FG);
	   }
		

	
	int cheight=ssize.height/chans;
	int cofs=p_channel*cheight;
		
	
	p->draw_fill_rect(Point(p_pos,cofs),Size(1,cheight+1),bg_col);
		
	int min_px=(p_min+32768)*cheight/65536;
	int prev_min_px=(p_prev_min+32768)*cheight/65536;
		
		
	int max_px=(p_max+32768)*cheight/65536;
	int prev_max_px=(p_prev_max+32768)*cheight/65536;
		
		
	min_px=MIN(min_px,prev_max_px);
	min_px+=cofs;		
		
	max_px=MAX(max_px,prev_min_px);
	max_px+=cofs;		

		
	p->draw_fill_rect( Point(p_pos,min_px),Size(1,max_px-min_px+1),fg_col);
	p->draw_fill_rect( Point(p_pos,cofs+cheight/2), Size(1,1), color(p_in_selection?COLOR_SAMPLE_EDITOR_CENTER_SEL:COLOR_SAMPLE_EDITOR_CENTER) );
}
	

	
void SampleViewer::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	Painter *p=get_painter();
	if (!sm->check(sample) || sm->get_size(sample)==0) {
		
		p->draw_fill_rect(Point(),p_size,color(COLOR_SAMPLE_EDITOR_NOSAMPLE));
		return;
	}
		
	AudioLock::begin();
	
	if (sm->lock_data(sample)) {
		
		AudioLock::end();
		
		ERR_PRINT("Can't lock sample!");
		return;
	}
	
	int sample_len=sm->get_size(sample);
	bool is_16=sm->is_16bits( sample );
	bool is_stereo=sm->is_stereo( sample );
	int loop_begin=sm->get_loop_begin( sample );
	int loop_end=sm->get_loop_end( sample );
	
	void *data_ptr=sm->get_data(sample);
	
	if (data_ptr==0) {
		
		AudioLock::end();
		
		ERR_PRINT("data_ptr==NULL , serious bug!");
		return;
		
	}
	
	int chans=is_stereo?2:1;
	
	int from=offset;
	int len=(int)(sample_len*zoom);
	
	
	for (int c=0;c<chans;c++) {
		
		Sint16 pre=0;
		
		
		Sint16 prev_max_peak=0;
		Sint16 prev_min_peak=0;
		Sint16 max_peak=-32768;
		Sint16 min_peak=32767; //pretty difficult values..
		int prev_screen_pos=0;
		bool has_loop_pos=false;
		for (int i=0;i<len;i++) {
			
			int pos=offset+i;
			if (pos>=sample_len)
				break; //abort just in case
			Sint16 val=0;
			if (is_16) {
				
				val=((Sint16*)data_ptr)[(chans)*pos+c];
				
			} else {
				
				val=((Sint8*)data_ptr)[(chans)*pos+c];
				val<<=8;
			}
			

			if (pos==loop_begin || pos==loop_end)
				has_loop_pos=true;
			
			if (val<min_peak)
				min_peak=val;
			
			if (val>max_peak)
				max_peak=val;
			
			int screen_pos=i*p_size.width/len;
			
			if (i==len-1)
				screen_pos=p_size.width;
			
			if (prev_screen_pos!=screen_pos || (i==len-1)) {
				
				bool is_selected=selection && selection->is_active() && pos>=selection->get_begin() && pos<=selection->get_end();
				
				for (int sp=(prev_screen_pos+1);sp<=screen_pos;sp++)
					draw_pos(prev_min_peak,prev_max_peak,min_peak,max_peak,sp,has_loop_pos,is_selected,c);
				
				has_loop_pos=false;
				prev_min_peak=min_peak;
				prev_max_peak=max_peak;
				max_peak=-32768;
				min_peak=32767; //pretty difficult values..
				prev_screen_pos=screen_pos;
				
			}
		}
	}
	
	
	sm->unlock_data(sample);
	AudioLock::end();
}
	
/*
void SampleViewer::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	Painter *p=get_painter();
	if (!sm->check(sample) || sm->get_size(sample)==0) {
		
		p->draw_fill_rect(Point(),p_size,color(COLOR_SAMPLE_EDITOR_NOSAMPLE));
		return;
	}
		
	
	//if (sample_data->is_peak_cache_dirty())
	//	sample_data->generate_peak_cache();
		
	

	int cur_width=ssize.width;
	int cur_height=ssize.height;


	int factor=1;

	for (int i=0;i<cur_width;i++) {

		draw_screen_pos(i,p,factor);
	}

}
*/
int SampleViewer::get_sample_size() {
	
	if (!sm->check(sample))
		return 0;
	return sm->get_size(sample);
	
}
void SampleViewer::set_sample(Sample_ID p_sample) {

	sample=p_sample;
	zoom=1;
	offset=0;

	update();
}


SampleViewer::SampleViewer()  {

	zoom=1; //1:1
	offset=0;

	set_bg_on_updates( false );
	
	selection=NULL;
	grabbing_mouse=false;
	grab_type=SET_SELECTION;
	
	sm=SampleManager::get_singleton();
}


SampleViewer::~SampleViewer()
{


}

