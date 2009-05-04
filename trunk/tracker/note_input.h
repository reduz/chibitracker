//
// C++ Interface: note_input
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NOTE_INPUT_H
#define NOTE_INPUT_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

#include "song/note.h"

class NoteInput {

	static NoteInput *singleton;
public:

	static NoteInput *get_singleton();
	struct Note {
	
		int note;
		int vol;
		bool off;
		Note() { note=255; off=false; }
	};

	virtual bool has_notes_pending() const=0;
	virtual Note pop_note() const=0;
	
	NoteInput();
	virtual ~NoteInput();

};

#endif
