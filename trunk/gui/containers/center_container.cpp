//
// C++ Implementation: center_container
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "center_container.h"

namespace GUI {


Size CenterContainer::get_minimum_size_internal() {
	
	Size min;
		
	Element *e=get_element_list();
	
	if (e) {
		
		min=e->frame->get_minimum_size();
	}
	
	return min;
}

void CenterContainer::resize_internal(const Size& p_new_size) {
	
		
	Element *e=get_element_list();
	
	if (e) {

		Size min;
		min=e->frame->get_minimum_size();
		e->rect=Rect( (p_new_size-min)/2 , min );
		e->frame->resize_tree(min);
	}
	
}
	
	
void CenterContainer::set_frame(Frame *p_frame) {
	
	if (get_element_list()) {
		
		PRINT_ERROR("Only ONE child allowed into CenterContainers...");
		return;
	}
	
	add_frame_internal( p_frame );
	check_minimum_size();
}

CenterContainer::CenterContainer()
{
}


CenterContainer::~CenterContainer()
{
}


}
