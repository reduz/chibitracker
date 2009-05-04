//
// C++ Implementation: sample_viewer_positions
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_viewer_positions.h"
#include "base/painter.h"
#include "ctskin.h"


Size SampleViewerPositions::get_minimum_size_internal() {

	return Size( 0, constant( C_SAMPLE_POSITON_ARROW_SIZE ) );
}

void SampleViewerPositions::resize(const Size& p_new_size) {
	
	size=p_new_size;
}
void SampleViewerPositions::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	Painter *p=get_painter();
	
	p->draw_fill_rect( Point(0,0), size, color(COLOR_SAMPLE_EDITOR_BG) );
	
	if (!mixer)
		return;
	Sample_ID sample=viewer->get_sample();
	if (sample.is_null())
		return;
	
	Color pos_col=color(COLOR_SAMPLE_EDITOR_CENTER);
	
	for (int i=0;i<mixer->get_total_voice_count();i++) {
		
		if (!mixer->is_voice_active(i) ) 
			continue;
		if (mixer->get_voice_sample_id( i )!=sample ) 
			continue;
		
		int pos=mixer->get_voice_sample_pos_index( i );
				
		int screen_pos=viewer->sample_to_screen_pos( pos );
		
		if (screen_pos<0 || screen_pos>size.width)
			continue;
		
		p->draw_arrow( Point( screen_pos-size.height/2, 0 ), Size( size.height,size.height), DOWN,pos_col );
	}
	
}

void SampleViewerPositions::set_mixer(Mixer *p_mixer) {
	
	mixer=p_mixer;	
}

void SampleViewerPositions::set_viewer(SampleViewer *p_viewr) {
	
	viewer=p_viewr;
	
}
SampleViewerPositions::SampleViewerPositions() {
	
	viewer=0;
	mixer=0;
	
}


SampleViewerPositions::~SampleViewerPositions()
{
}


