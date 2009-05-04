//
// C++ Implementation: icon
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "icon.h"
#include "base/painter.h"
namespace GUI {


Size Icon::get_minimum_size_internal() {
	
	
	if (bitmap<0)
		return Size();
	
	return get_painter()->get_bitmap_size( bitmap );
}

void Icon::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	if (bitmap<0)
		return;	
	
	Point ofs=(p_size-get_painter()->get_bitmap_size( bitmap ))/2;
	
	get_painter()->draw_bitmap( bitmap, ofs );
	
}

String Icon::get_type() {
	
	return "Icon";
}


void Icon::set_bitmap(BitmapID p_bitmap) {
	
	bitmap=p_bitmap;
	check_minimum_size();
	update();
}


Icon::Icon(BitmapID p_bitmap) {
	
	bitmap=p_bitmap;
}


Icon::~Icon()
{
}


}
