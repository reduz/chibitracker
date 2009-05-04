//
// C++ Implementation: bitmap_button
//
// Description: 
//
//
// Author: Juan,,, <red@hororo>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bitmap_button.h"
#include "base/painter.h"

namespace GUI {


Size BitmapButton::get_minimum_size_internal() {

	return get_painter()->get_bitmap_size( normal );
}

void BitmapButton::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	switch( get_draw_mode() ) {
	
		case DRAW_NORMAL: {
		
			if (normal)
				get_painter()->draw_bitmap( normal, Point() );
		} break;
		case DRAW_PRESSED: {
		
			BitmapID bm = pressed>=0 ? pressed : normal;
			if (bm)
				get_painter()->draw_bitmap( bm, Point() );
		
		} break;
		case DRAW_HOVER: {
			BitmapID bm = hover>=0 ? hover : normal;
			if (bm)
				get_painter()->draw_bitmap( bm, Point() );
		} break;
	
	}

	if (icon>=0)
		get_painter()->draw_bitmap( icon, (p_size-get_painter()->get_bitmap_size(icon))/2 );

}

void BitmapButton::set_normal_bitmap(BitmapID p_bitmap) {

	normal=p_bitmap;
}
void BitmapButton::set_pressed_bitmap(BitmapID p_bitmap) {

	pressed=p_bitmap;

}
void BitmapButton::set_hover_bitmap(BitmapID p_bitmap) {

	hover=p_bitmap;

}

void BitmapButton::set_icon_bitmap(BitmapID p_bitmap) {

	icon=p_bitmap;

}

BitmapButton::BitmapButton(BitmapID p_normal, BitmapID p_pressed, BitmapID p_hover,BitmapID p_icon) {

	normal=p_normal;	
	pressed=p_pressed;
	hover=p_hover;
	icon=p_icon;
}


BitmapButton::~BitmapButton()
{
}


}
