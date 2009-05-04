//
// C++ Implementation: playback_button
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//


#include "playback_button.h"
#include "base/painter.h"


#include "ctskin.h"


Size PlaybackButton::get_minimum_size_internal() {
	
	Size min=get_painter()->get_stylebox_min_size( stylebox( SB_PLAYBACK_BUTTON_NORMAL ) );
	
	if (constant( C_PLAYBACK_BUTTON_ARROW_SIZE )>0 && !no_minsize ) {
		
		min.width+=constant( C_PLAYBACK_BUTTON_ARROW_SIZE );
		min.height+=constant( C_PLAYBACK_BUTTON_ARROW_SIZE );
		
	}
	
	min.height+=constant( C_PLAYBACK_BUTTON_DISPLACEMENT );
	min.width+=constant( C_PLAYBACK_BUTTON_DISPLACEMENT );
	return min;
	
	
}
void PlaybackButton::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	Painter *p=get_painter();
	
	/* Draw Outline */
			
	//if disabled...
	bool draw_displaced=false;
	switch( get_draw_mode() ) {
		
		
		case DRAW_HOVER: { //optative, otherwise draw normal
			
			if (stylebox( SB_PLAYBACK_BUTTON_HOVER ).mode!=StyleBox::MODE_NONE) {
				
				p->draw_stylebox( stylebox( SB_PLAYBACK_BUTTON_HOVER ) , Point() , p_size, p_exposed );
				break;
			}
		}
		
		case DRAW_NORMAL: {
			
			p->draw_stylebox( stylebox( SB_PLAYBACK_BUTTON_NORMAL ) , Point() , p_size, p_exposed );
			
		} break;
		
		case DRAW_PRESSED: {
			
			p->draw_stylebox( stylebox( SB_PLAYBACK_BUTTON_PRESSED ) , Point() , p_size, p_exposed );
			draw_displaced=true;			
		} break;
		
	}
			
	
	Rect area_rect=Rect( p_pos, p_size );
	

	
	area_rect.pos.x=p->get_stylebox_margin( stylebox( SB_PLAYBACK_BUTTON_NORMAL ), MARGIN_LEFT );
	area_rect.pos.y=p->get_stylebox_margin( stylebox( SB_PLAYBACK_BUTTON_NORMAL ), MARGIN_TOP );
	area_rect.size-=area_rect.pos;
	
	area_rect.size.x-=p->get_stylebox_margin( stylebox( SB_PLAYBACK_BUTTON_NORMAL ), MARGIN_RIGHT );
	area_rect.size.y-=p->get_stylebox_margin( stylebox( SB_PLAYBACK_BUTTON_NORMAL ), MARGIN_BOTTOM );
	
	area_rect.size.x-=constant( C_PLAYBACK_BUTTON_DISPLACEMENT );
	area_rect.size.y-=constant( C_PLAYBACK_BUTTON_DISPLACEMENT );
	if (draw_displaced) 
		area_rect.pos+=Point( constant( C_PLAYBACK_BUTTON_DISPLACEMENT ), constant( C_PLAYBACK_BUTTON_DISPLACEMENT ) );
	
	
	if (constant( C_PLAYBACK_BUTTON_ARROW_SIZE )>0) {
		
		Point pos=area_rect.pos;
		Size size=area_rect.size;
		
		switch(type) {
			
			case TYPE_REWIND: {
				
				size.x/=2;
				get_painter()->draw_arrow( pos, size, LEFT, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ) );
				pos.x+=size.x;
				get_painter()->draw_arrow( pos, size, LEFT, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ) );
				
			} break;
			case TYPE_PLAY: {
				
				get_painter()->draw_arrow( pos, size, RIGHT, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ), false );
				
			} break;
			case TYPE_STOP: {
				
				int ofs=size.width/8;
				size.width-=ofs*2;
				size.height-=ofs*2;
				pos.x+=ofs;
				pos.y+=ofs;

				get_painter()->draw_fill_rect( pos, size, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ) );
				
			} break;
			case TYPE_FORWARD: {
				size.x/=2;
				get_painter()->draw_arrow( pos, size, RIGHT, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ) );
				pos.x+=size.x;
				get_painter()->draw_arrow( pos, size, RIGHT, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ) );
				
			} break;
			case TYPE_PLAY_PATTERN: {
				
				get_painter()->draw_arrow( pos, size, DOWN, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ), false );
				
			} break;
			case TYPE_PLAY_CURSOR: {
				
				get_painter()->draw_arrow( pos, Size(size.x,size.y/2), DOWN, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ), false );
				get_painter()->draw_arrow( pos+Point(0,size.y/2), Size(size.x,size.y/2), DOWN, color( COLOR_PLAYBACK_BUTTON_ARROW_COLOR ), false );
				
			} break;
		}
			
	} else {
		
		//draw some pixmap
	}

	if (has_focus())
		p->draw_stylebox( stylebox( SB_PLAYBACK_BUTTON_FOCUS ) , Point() , p_size, p_exposed);
	
	
}

String PlaybackButton::get_type() {
	
	
	return "PlaybackButton";
}


PlaybackButton::PlaybackButton(Type p_type,bool p_no_minsize)
{
	
	no_minsize=p_no_minsize;
	type=p_type;
	
	set_focus_mode( FOCUS_NONE ); //can't focus this, sorry
	set_fill_vertical( false );
}


PlaybackButton::~PlaybackButton()
{
}


