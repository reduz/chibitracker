//
// C++ Interface: center_container
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUICENTER_CONTAINER_H
#define PIGUICENTER_CONTAINER_H


#include "base/container.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class CenterContainer : public Container {
	
	Size get_minimum_size_internal();
	void resize_internal(const Size& p_new_size);
	
public:
	
	void set_frame(Frame *p_frame);
	
	template<class T>
	T* set( T*p_frame) {
		
		set_frame(p_frame);
		return p_frame;
	}
	
	CenterContainer();
	~CenterContainer();

};

}

#endif
