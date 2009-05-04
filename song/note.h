#ifndef NOTE_H
#define NOTE_H

#include "globals/config.h"

struct Note {

	enum {

		NOTES=120,
		OFF=254,
		CUT=253,
		EMPTY=255,
	};


	Uint8 note;
	Uint8 instrument;
	Uint8 volume;
	Uint8 command;
	Uint8 parameter;

	void clear() {

		note=EMPTY;
		instrument=EMPTY;
		volume=EMPTY;
		command=EMPTY;
		parameter=0;
	}
	
	void raise() {

		if (note<(NOTES-1)) note++;

	}

	void lower() {

		if ((note>0) && (note<NOTES)) note--;

	}

	bool operator== (const Note &rvalue) {

		return (
			 (note==rvalue.note) &&
			 (instrument==rvalue.instrument) &&
			 (volume==rvalue.volume) &&
			 (command==rvalue.command) &&
			 (parameter==rvalue.parameter)
			);
	}

	bool is_empty() const { return (note==EMPTY && instrument==EMPTY && volume==EMPTY && command==EMPTY && parameter==0); }
	Note() {

		clear();
	}
};


#endif

