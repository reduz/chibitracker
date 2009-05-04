//
// C++ Interface: interface_help
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef INTERFACE_HELP_H
#define INTERFACE_HELP_H

#include "base/window.h"
#include "widgets/text_edit.h"
#include "widgets/scroll_bar.h"
#include "containers/box_container.h"


enum HelpType{

	HELP_PATTERN_KEYS,
	HELP_SKINS,
};

using namespace GUI;

class InterfaceHelp : public Window {


	
public:


	InterfaceHelp(Window *p_parent,HelpType p_type);

};

#endif
