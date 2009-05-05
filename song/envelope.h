/***************************************************************************
                          envelope.h  -  description
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

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "config.h"

/**envelope?
  *@author Juan Linietsky
  */

/******************************
 envelope.h
 ----------

Proovides an envelope, and basic functions
for it that can be used for both player
and interface
********************************/


class Envelope {
	enum {

		MAX_POINTS=25
	};

	struct Point {
	
		Uint16 tick_offset;
		Sint16 value;
	};

	Point node[MAX_POINTS];
	
	Sint8 node_count;

	bool on;
	bool carry;

	bool loop_on;

	Uint8 loop_begin_node;
	Uint8 loop_end_node;

	bool sustain_loop_on;
	Uint8 sustain_loop_begin_node;
	Uint8 sustain_loop_end_node;

	
	Sint8 max_value;
	Sint8 min_value;
	

public:
	enum {
		
		NO_POINT=-5000,
	};
	
	void set_max(Sint8 p_max) { max_value=p_max; }
	Sint8 get_max() { return max_value; }
	void set_min(Sint8 p_min) { min_value=p_min; }
	Sint8 get_min() { return min_value; }

	Uint8 get_node_count();
	const Point& get_node(int p_idx);

	void set_position(int p_node,int p_x,int p_y);
	int add_position(int p_x,int p_y,bool p_move_loops=true);
	void del_position(int p_node);

	void set_loop_enabled(bool p_enabled);
	bool is_loop_enabled();
	void set_loop_begin(int pos);
	void set_loop_end(int pos);
	Uint8 get_loop_begin();
	Uint8 get_loop_end();

	void set_sustain_loop_enabled(bool p_enabled);
	bool is_sustain_loop_enabled();
	void set_sustain_loop_begin(int pos);
	void set_sustain_loop_end(int pos);
	Uint8 get_sustain_loop_begin();
	Uint8 get_sustain_loop_end();
	
	void set_enabled(bool p_enabled);
	bool is_enabled();
	
	void set_carry_enabled(bool p_enabled);
	bool is_carry_enabled();
	
	void reset();
	int get_height_at_pos(int pos);
	float get_interp_height_at_pos(float pos);
	
	
	Envelope();
		
};
#endif
