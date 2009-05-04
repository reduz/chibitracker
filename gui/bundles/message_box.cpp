//
// C++ Implementation: message_box
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "message_box.h"
#include "containers/center_container.h"
#include "bundles/window_box.h"
#include "widgets/button.h"
#include <stdio.h>
namespace GUI {

void MessageBox::show(String p_text) {
	

	text->set_text( p_text );
	Window::show();
}

MessageBox::MessageBox(Window *p_parent,SizeMode p_size_mode) : Window(p_parent,Window::MODE_POPUP, p_size_mode) {
	
	
	WindowBox *vbc = GUI_NEW( WindowBox("!!") );
	set_root_frame( vbc );
	custom_vb= vbc->add(GUI_NEW( VBoxContainer),1);
	text = vbc->add(GUI_NEW(Label));
	vbc->add( GUI_NEW( CenterContainer) )->set( GUI_NEW( Button("Ok")) )->pressed_signal.connect( static_cast<Window*>(this), &Window::hide );
	
}


MessageBox::~MessageBox()
{
}


}
