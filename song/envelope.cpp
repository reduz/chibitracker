/***************************************************************************
                          envelope.cpp  -  description
                             -------------------
    begin                : Sun Mar 25 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "envelope.h"
#include "error_macros.h"
#include "mixer/audio_lock.h"

Envelope::Envelope() {


	reset();
}

void Envelope::reset() {


	AudioLock::begin();
	on=false;
	carry=false;
	loop_on=false;
	loop_begin_node=0;
	loop_end_node=0;
	sustain_loop_on=false;
	sustain_loop_begin_node=0;
	sustain_loop_end_node=0;
	node_count=0;
	AudioLock::end();
}

int Envelope::get_height_at_pos(int pos) {

	if (node_count && pos>node[node_count-1].tick_offset)
		return node[node_count-1].value;
	
	int begin_x,begin_y;
	int end_x,end_y,xdif;
	int count=0;
	int limit=-1;

	if (node_count<2) return NO_POINT;

	while ((count<node_count) && (limit==-1)) {

		if (node[count].tick_offset>=pos) limit=count;
		count++;
	}

	if (pos==0) return node[0].value;

	if (limit==-1) return NO_POINT;

	begin_x=node[limit-1].tick_offset;
	end_x=node[limit].tick_offset;
	begin_y=node[limit-1].value;
	end_y=node[limit].value;

	xdif=end_x-begin_x;
	return begin_y+((pos-begin_x)*(end_y-begin_y))/(xdif?xdif:1);
}

float Envelope::get_interp_height_at_pos(float pos) {

	if (node_count && pos>node[node_count-1].tick_offset)
		return node[node_count-1].value;

	int begin_x,begin_y;
	int end_x,end_y,xdif;
	int count=0;
	int limit=-1;

	if (node_count<2) return NO_POINT;

	while ((count<node_count) && (limit==-1)) {

		if (node[count].tick_offset>=pos) limit=count;
		count++;
	}

	if (pos==0) return node[0].value;

	if (limit==-1) return NO_POINT;

	begin_x=node[limit-1].tick_offset;
	end_x=node[limit].tick_offset;
	begin_y=node[limit-1].value;
	end_y=node[limit].value;

	xdif=end_x-begin_x;
	return begin_y+((pos-begin_x)*(end_y-begin_y))/(xdif?xdif:1);
}

void Envelope::set_position(int p_node,int p_x,int p_y) {

	if (p_node>=node_count) return;
	
	AudioLock::begin();

	if (p_node==0) {

		p_x=0;

	} else if (p_x<=node[p_node-1].tick_offset) {

		p_x=node[p_node-1].tick_offset+1;

	} else if ((p_node<(node_count-1)) && (p_x>=node[p_node+1].tick_offset)) {

		p_x=node[p_node+1].tick_offset-1;
	}

	if (p_x>=9999) p_x=9999;

	if (p_y>max_value) p_y=max_value;
	if (p_y<min_value) p_y=min_value;

	
	node[p_node].tick_offset=p_x;
        node[p_node].value=p_y;
	
	AudioLock::end();
	
}

int Envelope::add_position(int p_x,int p_y,bool p_move_loops) {

	if (node_count==MAX_POINTS) return -1;

	
	int i,new_node;

	// if this is assigning an existing node, let's quit.
	for (i=0;i<node_count;i++) if (p_x==node[i].tick_offset) return -1;
	AudioLock::begin();

	i=0;
	while ((i<node_count) && (p_x>=node[i].tick_offset)) i++;
	
	new_node=i;
	node_count++;

	if (p_move_loops) {
		if (loop_begin_node>=new_node) loop_begin_node++;
		if (loop_end_node>=new_node) loop_end_node++;
		if (sustain_loop_begin_node>=new_node) sustain_loop_begin_node++;
		if (sustain_loop_end_node>=new_node) sustain_loop_end_node++;
	}
	for (i=node_count-1;i>new_node;i--) node[i]=node[i-1];

	AudioLock::end();
		
        set_position(new_node,p_x,p_y);


	
	return new_node;
	
}

void Envelope::set_loop_begin(int pos) {

	if ((pos<0) || (pos>=node_count)) return;

	AudioLock::begin();
	
	loop_begin_node=pos;

	if (loop_end_node<loop_begin_node) loop_end_node=loop_begin_node;

	AudioLock::end();

}

void Envelope::set_loop_end(int pos) {

	if ((pos<0) || (pos>=node_count)) return;

	AudioLock::begin();
	
        loop_end_node=pos;
	
	if (loop_end_node<loop_begin_node) loop_begin_node=loop_end_node;

	AudioLock::end();
	

}


void Envelope::set_sustain_loop_begin(int pos) {

	if ((pos<0) || (pos>=node_count)) return;

	AudioLock::begin();
	
	sustain_loop_begin_node=pos;

	if (sustain_loop_end_node<sustain_loop_begin_node) sustain_loop_end_node=sustain_loop_begin_node;

	AudioLock::end();

}

void Envelope::set_sustain_loop_end(int pos) {

	if ((pos<0) || (pos>=node_count)) return;

	AudioLock::begin();
	
        sustain_loop_end_node=pos;
	
	if (sustain_loop_end_node<sustain_loop_begin_node) sustain_loop_begin_node=sustain_loop_end_node;

	AudioLock::end();

}

void Envelope::set_loop_enabled(bool p_enabled) {
	
	loop_on=p_enabled;
}
bool Envelope::is_loop_enabled() {
	
	return loop_on;
}


void Envelope::set_sustain_loop_enabled(bool p_enabled) {
	
	sustain_loop_on=p_enabled;
}
bool Envelope::is_sustain_loop_enabled() {
	
	return sustain_loop_on;
}

void Envelope::del_position(int p_node) {

	if ((node_count<3) || (p_node<=0) || (p_node>=node_count)) return;

	AudioLock::begin();
	
	int i;

	if (loop_begin_node>=p_node) loop_begin_node--;
	if (loop_end_node>=p_node) loop_end_node--;
	if (sustain_loop_begin_node>=p_node) sustain_loop_begin_node--;
	if (sustain_loop_end_node>=p_node) sustain_loop_end_node--;

	for (i=p_node;i<node_count-1;i++) node[i]=node[i+1];

	node_count--;
	
	AudioLock::end();
	
}

Uint8 Envelope::get_loop_begin() {
	
	
	return loop_begin_node;
}
Uint8 Envelope::get_loop_end() {
	
	return loop_end_node;	
}

Uint8 Envelope::get_sustain_loop_begin() {
	
	
	return sustain_loop_begin_node;
}
Uint8 Envelope::get_sustain_loop_end() {
	
	return sustain_loop_end_node;	
}



void Envelope::set_enabled(bool p_enabled) {
	
	on=p_enabled;
}

bool Envelope::is_enabled() {
	
	return on;	
}

void Envelope::set_carry_enabled(bool p_enabled) {
	
	carry=p_enabled;
}
bool Envelope::is_carry_enabled() {

	return carry;
}

Uint8 Envelope::get_node_count() {
	
	return node_count;	
}

const Envelope::Point& Envelope::get_node(int p_idx) {
	
	static Point errpoint;
	ERR_FAIL_INDEX_V(p_idx,node_count,errpoint);
	
	return node[p_idx];
	
}
