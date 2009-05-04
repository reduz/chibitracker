//
// C++ Interface: message_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUIMESSAGE_BOX_H
#define PIGUIMESSAGE_BOX_H

#include "base/window.h"
#include "widgets/label.h"
#include "containers/box_container.h"

namespace GUI {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class MessageBox : public Window{
	
	VBoxContainer *custom_vb;
	Label *text;
public:
	VBoxContainer *get_custom_vb() { return custom_vb; }
	void show(String p_text);
	MessageBox(Window *p_parent,SizeMode p_size_mode=SIZE_CENTER);
	~MessageBox();

};

}

#endif
