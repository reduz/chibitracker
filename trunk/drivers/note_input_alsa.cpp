//
// C++ Implementation: note_input_alsa
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "note_input_alsa.h"


#ifdef ALSA_ENABLED


void NoteInputAlsa::poll_events() const {

	while(snd_seq_event_input_pending(handle, 1)>0) {
	

		snd_seq_event_t *event;
		int er;
	
		Note note;
	
		er = snd_seq_event_input(handle, &event);
	
			if (er < 0 || event == NULL) {
				continue;
			}
	
		switch(event->type) {
	
			case SND_SEQ_EVENT_NOTEON: {
				if (event->data.note.note<120) {
				
					note.note=event->data.note.note;
					note.vol=(int)event->data.note.velocity * 64 / 128;
					note.off=event->data.note.velocity==0;
				}
			} break;
	
	
			case SND_SEQ_EVENT_NOTEOFF: {
	
				if (event->data.note.note<120) {
				
					note.note=event->data.note.note;
					note.vol=(int)event->data.note.velocity * 64 / 128;
					note.off=true;
				}
			} break;
	
			default: {
			} break;
		}

		snd_seq_free_event(event);
		
		if (note.note!=255 && ((rb_write+1)&RB_MASK)!=rb_read) {
		
			rb[rb_write]=note;
			rb_write=(rb_write+1)&RB_MASK;
		}
	}

}

bool NoteInputAlsa::has_notes_pending() const {

	poll_events();

	return rb_read!=rb_write;

}
NoteInput::Note NoteInputAlsa::pop_note() const {


	poll_events();
	
	if (rb_write==rb_read) {
	
		return Note();
	}

	Note ret = rb[rb_read];
	rb_read=(rb_read+1)&RB_MASK;
	return ret;
}


NoteInputAlsa::NoteInputAlsa() {

	rb_read=0;
	rb_write=0;
	
	int port;

	/* open that bitch */
	if (snd_seq_open(&handle, "hw", SND_SEQ_OPEN_INPUT, 0)<0) 
		return;


	client = snd_seq_client_id(handle); //get client?
	//fd = get_file_descriptor();
	snd_seq_set_client_pool_input(handle, 1000); /* enough? */


	port = snd_seq_create_simple_port(handle, "ChibiTracker",
					  SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
					  SND_SEQ_PORT_TYPE_MIDI_GENERIC);
	if (port < 0) {
		return;
	}

	port_index=port;	
}


NoteInputAlsa::~NoteInputAlsa(){

	snd_seq_close(handle);
}

#endif
