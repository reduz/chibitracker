//
// C++ Implementation: scroll_box
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "scroll_box.h"

namespace GUI {

void ScrollBox::set_expand_h(bool p_enable) {
	
	scroll->set_expand_h(p_enable);
}
void ScrollBox::set_expand_v(bool p_enable) {
	
	scroll->set_expand_v(p_enable);
	
}
bool ScrollBox::get_expand_h() {
	
	return scroll->get_expand_h();
	
}
bool ScrollBox::get_expand_v() {
	
	return scroll->get_expand_v();
	
}

void ScrollBox::set_scroll_h(bool p_enable) {
	
	scroll->set_scroll_h(p_enable);	
}
void ScrollBox::set_scroll_v(bool p_enable) {
	
	scroll->set_scroll_v(p_enable);	
	
}
bool ScrollBox::get_scroll_h() {
	
	return scroll->get_scroll_h();
}
bool ScrollBox::get_scroll_v() {
	
	return scroll->get_scroll_v();
	
}

ScrollContainer * ScrollBox::get_scroll_container() {
	
	return scroll;
}

Range* ScrollBox::get_h_range() {
	
	return scroll->get_h_range();
}
Range* ScrollBox::get_v_range() {
	
	return scroll->get_v_range();	
}

HScrollBar *ScrollBox::get_h_scrollbar() {
	
	return hbar;
}
VScrollBar *ScrollBox::get_v_scrollbar() {
	
	return vbar;
}

void ScrollBox::set_frame(Frame *p_frame) {
	
	scroll->set_frame(p_frame);	
}

ScrollBox::ScrollBox() : GridContainer(2)
{
	
	scroll = GridContainer::add( GUI_NEW( ScrollContainer), true, true );
	vbar = GridContainer::add( GUI_NEW( VScrollBar), false, true );
	hbar = GridContainer::add( GUI_NEW( HScrollBar), true, false );
	
	vbar->set_range(scroll->get_v_range());
	hbar->set_range(scroll->get_h_range());
	
	vbar->set_auto_hide(true);
	hbar->set_auto_hide(true);
}


ScrollBox::~ScrollBox()
{
}


}
