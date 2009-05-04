//
// C++ Interface: note_input_alsa
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NOTE_INPUT_ALSA_H
#define NOTE_INPUT_ALSA_H

#ifdef ALSA_ENABLED
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
#include "tracker/note_input.h"
#include <alsa/asoundlib.h>

class NoteInputAlsa : public NoteInput {


	enum {
		
		RB_SIZE=512,
		RB_MASK=RB_SIZE-1
	};
	
	mutable Note rb[RB_SIZE];
	mutable int rb_read;
	mutable int rb_write;
	
	snd_seq_t *handle;	/* The snd_seq handle to /dev/snd/seq */
	int client;		/* The client associated with this context */
	int port_index;	/* created sequencer ports */
	int fd;			/* The file descriptor */
	int used;		/* number of current connection */
	bool active;		/* */	
	
	void poll_events() const;
public:

	virtual bool has_notes_pending() const;
	virtual Note pop_note() const;

    NoteInputAlsa();
    ~NoteInputAlsa();

};

#endif
#endif
