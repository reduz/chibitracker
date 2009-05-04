
#ifndef PATTERN_H
#define PATTERN_H

#include "song/note.h"
#include "mempool_wrapper.h"

class Pattern {
public:
	
	
	enum { 
		WIDTH=64,
		DEFAULT_LEN=64,
		RESIZE_EVERY_BITS=4,
		MIN_ROWS=1, //otherwise clipboard wont work
		MAX_LEN=256
				
	};	
	
private:	
	struct Event {
		
		Uint16 pos; //column*WIDTH+row
		Uint8 note;
		Uint8 instrument;
		Uint8 volume;
		Uint8 command;
		Uint8 parameter;		
	};

	Uint16 length;
	Uint32 event_count;
	MemPool_Handle mem_handle;
	
	Event * get_event_list();
	void release_event_list();
	Sint32 get_event_pos(Uint16 p_target_pos,Event *p_event_list);
	bool erase_event_at_pos(Uint16 p_pos);
	
	bool resize_event_list_to(Uint32 p_events);
	
	void operator=(const Pattern& p_pattern); //no operator=
public:
	
	bool is_empty();
	void clear();

	bool set_note(Uint8 p_column, Uint16 p_row,const Note& p_note); //true if no more memory
	Note get_note(Uint8 p_column,Uint16 p_row);
	
	void copy_to(Pattern *p_pattern) const;
	void set_length(Uint16 p_rows);
	Uint16 get_length();
	Pattern();
	~Pattern();
	

};

#endif
