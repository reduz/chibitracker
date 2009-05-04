//
// C++ Implementation: pattern
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "pattern.h"
#include "error_macros.h"
#include "mixer/audio_lock.h"

void Pattern::clear() {

	AudioLock::begin();

	if (event_count>0) {
		
		
		MemPool_Wrapper::get_singleton()->free_mem( mem_handle );
		mem_handle=MemPool_Handle();
		event_count=0;		
	}

	length=DEFAULT_LEN;
	AudioLock::end();
	
}

void Pattern::release_event_list() {
	MemPool_Wrapper::get_singleton()->unlock_mem(mem_handle);

	
}

bool Pattern::resize_event_list_to(Uint32 p_events) {
	

	//Module is slow in some cpus, so this should be fast enough
	Uint32 new_size=((p_events-1)&(~((1<<RESIZE_EVERY_BITS)-1)))+(1<<RESIZE_EVERY_BITS);
	
	ERR_FAIL_COND_V(new_size<p_events,true); //bugARM_INFO
	
	if (event_count==0 && new_size==0)
		return false; //nothing to do
	
	if (event_count==0) {
		
		ERR_FAIL_COND_V(!mem_handle.is_null(),true);
		mem_handle=MemPool_Wrapper::get_singleton()->alloc_mem( new_size*sizeof(Event) );
		ERR_FAIL_COND_V(mem_handle.is_null(),true);

	} else if (new_size==0) {
		
		MemPool_Wrapper::get_singleton()->free_mem( mem_handle );
		mem_handle=MemPool_Handle();
	} else {
		
		ERR_FAIL_COND_V(mem_handle.is_null(),true);
		
		ERR_FAIL_COND_V(MemPool_Wrapper::get_singleton()->realloc_mem( mem_handle, new_size*sizeof(Event) ),true);
		
	}
	
	event_count=p_events;
	
	return false;
}

Pattern::Event* Pattern::get_event_list() {
	

	ERR_FAIL_COND_V(MemPool_Wrapper::get_singleton()->lock_mem(mem_handle),0);
	
	Event* event_list=(Event*)MemPool_Wrapper::get_singleton()->get_mem(mem_handle);
	
	return event_list;
}

Sint32 Pattern::get_event_pos(Uint16 p_target_pos,Event *p_event_list) {
	
	ERR_FAIL_COND_V( p_event_list == NULL, -1 );
	
	if (event_count==0)
		return -1;
	
	int low = 0;
	int high = event_count -1;
	int middle;
	
	while( low <= high )
	{
		middle = ( low  + high ) / 2;

		if( p_target_pos == p_event_list[middle].pos ) { //match
			break;
		} else if( p_target_pos < p_event_list[middle].pos )
			high = middle - 1; //search low end of array
		else
			low = middle + 1; //search high end of array
	}

	/* adapt so we are behind 2 */

	if (p_event_list[middle].pos<p_target_pos)
		middle++;
	return middle;
	
	/* Linear search for now */

	/*
	Sint32 pos_idx=0;
		
	for (;pos_idx<event_count;pos_idx++) {
		
		if (event_list[pos_idx].pos>=p_target_pos)
			break;
		
	} */
	
	//return pos_idx;
}

bool Pattern::erase_event_at_pos(Uint16 p_pos) {
	
	if (event_count==0)
		return false;
	

	
	Event *event_list=get_event_list();

	Sint32 pos_idx = get_event_pos(p_pos,event_list);	
	if (pos_idx==-1) {
		release_event_list();
		ERR_FAIL_COND_V(pos_idx==-1,true);				
	}
	
	if (pos_idx==event_count || event_list[pos_idx].pos!=p_pos) {
		/* Nothing to Erase */
		release_event_list();
		return false;
	}
	
	for (Sint32 i=pos_idx;i<(event_count-1);i++) {
		
		event_list[i]=event_list[i+1];
	}
	
	release_event_list();
	resize_event_list_to(event_count-1); 
	
	return false;
}

bool Pattern::set_note(Uint8 p_column, Uint16 p_row,const Note& p_note) {

	ERR_FAIL_COND_V(p_column>=WIDTH,true);
	ERR_FAIL_COND_V(p_row>=length,true);
	
	Sint32 new_pos;
	Uint16 target_pos=p_row*WIDTH+p_column;
	
	AudioLock::begin();
	
	if (p_note.is_empty()) {
		bool res=erase_event_at_pos(target_pos);
		AudioLock::end();
		return res;;
	}
	
	Event *event_list=0;
	
	if (event_count==0) {
		/* If no events, create the first */
		
		if (resize_event_list_to(1)) {
			AudioLock::end();
			ERR_PRINT("Can't resize event list to 1");
			return true;
		}

		event_list=get_event_list();
		if (event_list==0) {
			
			AudioLock::end();
			ERR_PRINT("Can't get event list");
			return true;
		}
		
		new_pos=0;				
		
	} else {
		/* Prepare to add */
		
		event_list=get_event_list();
		if (event_list==0) {
			
			AudioLock::end();
			ERR_PRINT("Can't get event list");
			return true;
		}
		
		Sint32 pos_idx = get_event_pos(target_pos,event_list);	
		
		if (pos_idx==-1) {
			release_event_list();
			AudioLock::end();
			ERR_PRINT("Can't find add position");
			return true;
		}
			
		
		if (pos_idx==event_count || event_list[pos_idx].pos!=target_pos) { 
			/* If the note being modified didnt exist, then we add it */
			release_event_list();
			//resize, and return if out of mem
			if (resize_event_list_to( event_count+1)) {
				
				AudioLock::end();
				ERR_PRINT("Can't resize event list");
				return true;
			}
			event_list=get_event_list();
			if (event_list==0) {
				
				AudioLock::end();
				ERR_PRINT("Can't get event list");
				return true;
			}
			
			//make room for new pos, this wont do a thing if pos_idx was ==event_count
			for(Sint32 i=(event_count-1);i>pos_idx;i--) {
				event_list[i]=event_list[i-1];
				
			}
			
		} /* Else it means that position is taken, so we just modify it! */
		
				  
		new_pos=pos_idx;
	}
	
	event_list[new_pos].pos=target_pos;
	event_list[new_pos].note=p_note.note;
	event_list[new_pos].instrument=p_note.instrument;
	event_list[new_pos].volume=p_note.volume;
	event_list[new_pos].command=p_note.command;
	event_list[new_pos].parameter=p_note.parameter;
	
	release_event_list();
	
	AudioLock::end();
	return false;
	
}
Note Pattern::get_note(Uint8 p_column,Uint16  p_row) {

	
	ERR_FAIL_COND_V(p_column>=WIDTH,Note());
	ERR_FAIL_COND_V(p_row>=length,Note());

	if (event_count==0)
		return Note();
	

	Event *event_list=get_event_list();
	
	ERR_FAIL_COND_V(event_list==0,Note());
	
	Uint16 target_pos=p_row*WIDTH+p_column;
	Sint32 pos_idx = get_event_pos(target_pos,event_list);
	if (pos_idx==-1) {	
		release_event_list();
		ERR_PRINT("Can't find event pos");
		return Note();
	}
	
	if (pos_idx>=event_count || event_list[pos_idx].pos!=target_pos) {
		/* no note found */
		release_event_list();
		return Note();
	}
	
	Note n;
	n.note=event_list[pos_idx].note;
	n.instrument=event_list[pos_idx].instrument;
	n.volume=event_list[pos_idx].volume;
	n.command=event_list[pos_idx].command;
	n.parameter=event_list[pos_idx].parameter;
	
	release_event_list();
	return n;
	
}

void Pattern::set_length(Uint16 p_rows) {
	
	AudioLock::begin();
	
	if (event_count==0) {
		
		if (p_rows>=MIN_ROWS)
			length=p_rows;
		AudioLock::end();
			
		return;	
		
	}
	
	if (p_rows<MIN_ROWS) {
		AudioLock::end();
		return;
	}
	
	if (p_rows<length) {
			
		Event* event_list=get_event_list();
		if (event_list==0) {
			AudioLock::end();
			ERR_PRINT("get_event_list() Failed");
			return;
		}
			

		Uint16 target_pos=p_rows*WIDTH;
		Sint32 pos_idx = get_event_pos(target_pos,event_list);	
		release_event_list();
		
		if (pos_idx==-1) {
			AudioLock::end();
			ERR_FAIL_COND(pos_idx==-1);				
		}
			
		if (resize_event_list_to(pos_idx)) {
			AudioLock::end();
			ERR_PRINT("resize_event_list_to(pos_idx) Failed");
			return;
		}
		
	}
	
	length=p_rows;
	AudioLock::end();
	
}

void Pattern::copy_to(Pattern *p_pattern) const {
	
	
	
	
	p_pattern->clear();
	p_pattern->length=length;
	
	
	if (!event_count)
		return;
	
	AudioLock::begin();
	
	int bufsiz=MemPool_Wrapper::get_singleton()->get_mem_size( mem_handle );
	MemPool_Handle aux_mem_handle=MemPool_Wrapper::get_singleton()->alloc_mem( bufsiz );
	
	if (aux_mem_handle.is_null()) {
		
		ERR_PRINT("own handle is null");
		AudioLock::end();
		return;		
	}
			
	
	if (MemPool_Wrapper::get_singleton()->lock_mem(aux_mem_handle)) {
		ERR_PRINT("Unable to lock aux new handle");		
		AudioLock::end();
		return;
		
	}
	
	if (MemPool_Wrapper::get_singleton()->lock_mem(mem_handle)) {
		
		ERR_PRINT("Unable to lock own handle");
		AudioLock::end();
		return;
	}
	
	Uint8* src=(Uint8*)MemPool_Wrapper::get_singleton()->get_mem(mem_handle);
	Uint8* dst=(Uint8*)MemPool_Wrapper::get_singleton()->get_mem(aux_mem_handle);
	
	for (int i=0;i<bufsiz;i++) 
		dst[i]=src[i];
	
	MemPool_Wrapper::get_singleton()->unlock_mem(mem_handle);
	MemPool_Wrapper::get_singleton()->unlock_mem(aux_mem_handle);
	
	p_pattern->mem_handle=aux_mem_handle;
	p_pattern->event_count=event_count;
	
	AudioLock::end();
}
Uint16 Pattern::get_length() {
	
	
	return length;
}
Pattern::Pattern() {
	
	
	length=DEFAULT_LEN;
	event_count=0;
	clear();
	
}
bool Pattern::is_empty() {
	
	return mem_handle.is_null();
}

Pattern::~Pattern() {
	
	clear();
}
