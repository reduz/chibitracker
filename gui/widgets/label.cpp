//
// C++ Implementation: label
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "label.h"
#include "base/painter.h"
#include "base/skin.h"
namespace GUI {


Size Label::get_minimum_size_internal() {
	
	if (!get_painter()) {
		return Size();
	}
	
	Size min;
	
	min.width=get_painter()->get_font_string_width( font(FONT_LABEL), text ) + constant( C_LABEL_MARGIN )*2;
	min.height=get_painter()->get_font_height( font(FONT_LABEL) ) + constant( C_LABEL_MARGIN )*2;
	
	return min;
	
}


void Label::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	Size string_size;
	
	string_size.width=get_painter()->get_font_string_width( font(FONT_LABEL), text );
	string_size.height=get_painter()->get_font_height( font(FONT_LABEL) );
	
	int y_ofs=constant( C_LABEL_MARGIN )+((p_size.height-constant( C_LABEL_MARGIN )*2)-string_size.height)/2+get_painter()->get_font_ascent( font(FONT_LABEL) );
	int x_ofs=0;
	
	switch (align) {
		
		case ALIGN_LEFT: {
			
			x_ofs=constant( C_LABEL_MARGIN );
		} break;
		case ALIGN_CENTER: {
			
			x_ofs=constant( C_LABEL_MARGIN )+((p_size.width-constant( C_LABEL_MARGIN )*2)-string_size.width)/2;
			
		} break;
		case ALIGN_RIGHT: {
			
			x_ofs=p_size.width-constant( C_LABEL_MARGIN )-string_size.width;
		} break;
		
	}
	
	get_painter()->draw_text( font(FONT_LABEL), Point( x_ofs, y_ofs ), text, color(COLOR_LABEL_FONT) );
	
	
	
	
}

void Label::set_align(Align p_align) {
	
	align=p_align;
	update();
}
Label::Align Label::get_align(){
	
	return align;
}



void Label::set_text(const String& p_string) {
	
	if (text==p_string)
		return;
	
	text=p_string;
	check_minimum_size();
	update();
	
}
String Label::get_text() {
	
	return text;
}

String Label::get_type() {

	return "Label";
}

Label::Label(String p_text,Align p_align)
{
	align=p_align;
	text=p_text;
	
}


Label::~Label()
{
}


}
