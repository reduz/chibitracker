//
// C++ Implementation: sample_status_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_status_list.h"
#include "base/painter.h"
#include "base/skin.h"
#include "interface/ctskin.h"

#include <stdio.h>

Size SampleStatusList::get_minimum_size_internal() {

	Size min=get_painter()->get_stylebox_min_size( stylebox( SB_LIST_NORMAL ) );
	min.width+=get_painter()->get_font_char_width(font( FONT_LIST ),'X')+constant( C_LIST_HMARGIN )*2;
	return min;

}
void SampleStatusList::draw(const Point& p_global,const Size& p_size,const Rect& p_exposed) {

	get_painter()->draw_stylebox( stylebox( SB_LIST_NORMAL ), Point() , p_size, p_exposed );



	int line_size=get_painter()->get_font_height( font( FONT_LIST ) )+constant( C_LIST_SPACING );

	int space=p_size.height;
	space-=get_painter()->get_stylebox_margin( stylebox( SB_LIST_NORMAL ) , MARGIN_TOP );
	space-=get_painter()->get_stylebox_margin( stylebox( SB_LIST_NORMAL ) , MARGIN_BOTTOM );

	if (line_size==0)
		return;
	
	int visible_lines=space/line_size;
	
	
	int offset=(int)get_range()->get();
	
	int voice_sample[Song::MAX_SAMPLES];
	
	for (int i=0;i<Song::MAX_SAMPLES;i++)
		voice_sample[i]=0;
	
	for (int i=0;i<tracker->player->get_voice_amount_limit();i++) {
		
		if (!tracker->player->is_voice_active(i))
			continue;
		
		int sidx=tracker->player->get_voice_sample_index(i);
		if (sidx<0 || sidx>=Song::MAX_SAMPLES)
			continue;
					
		int v=tracker->player->get_voice_volume(i);
		if (v==0)
			v=1;
		if (v>voice_sample[sidx])
			voice_sample[sidx]=v;
		
		
	}
	
	int margin_left=get_painter()->get_stylebox_margin( stylebox( SB_LIST_NORMAL ) , MARGIN_LEFT );
	margin_left+=constant( C_LIST_HMARGIN );
	int margin_top=get_painter()->get_stylebox_margin( stylebox( SB_LIST_NORMAL ) , MARGIN_TOP );
	
	
	for (int i=0;i<visible_lines;i++) {
		
		int samp_idx=i+offset;
		
		if (samp_idx>=Song::MAX_SAMPLES)
			break;
		
		
		if (tracker->song->get_sample( samp_idx )->get_sample_data().is_null())
			continue;
		
		int line_ofs=margin_top+line_size*i+get_painter()->get_font_ascent( font(FONT_LIST ));
		
		
		

		if (voice_sample[samp_idx]) {
			
			Color min=color(COLOR_SAMPLE_POS_MINVOL);
			Color max=color(COLOR_SAMPLE_POS_MAXVOL);
			
			Color col;
			col.r=(int)min.r+((int)max.r-(int)min.r)*voice_sample[samp_idx]/256;
			col.g=(int)min.g+((int)max.g-(int)min.g)*voice_sample[samp_idx]/256;
			col.b=(int)min.b+((int)max.b-(int)min.b)*voice_sample[samp_idx]/256;
			
			get_painter()->draw_text( font(FONT_LIST),Point(margin_left,line_ofs),"P",col);
		} else
			get_painter()->draw_text( font(FONT_LIST),Point(margin_left,line_ofs),"-",color(COLOR_LIST_FONT));
		
			
	} 
}


SampleStatusList::SampleStatusList(Tracker *p_tracker)
{
	tracker=p_tracker;
}


SampleStatusList::~SampleStatusList()
{
}


