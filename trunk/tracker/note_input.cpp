//
// C++ Implementation: note_input
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "note_input.h"

NoteInput *NoteInput::singleton=NULL;
 
 
NoteInput *NoteInput::get_singleton() {

	return singleton;		
}

NoteInput::NoteInput() {
	singleton=this;
}


NoteInput::~NoteInput()
{
}


