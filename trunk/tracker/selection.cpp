/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : reduzio@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//
//
// C++ Implementation: selection
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "selection.h"

int Selection::get_size() {

	return get_end()-get_begin();
}

void Selection::set_point_1(int p_idx) {

	if (p_idx<0)
		p_idx=0;
	if ( (max>=0) && (p_idx>max) )
		p_idx=max;

	point_1=p_idx;
}
void Selection::set_point_2(int p_idx) {

	if (p_idx<0)
		p_idx=0;
	if ( (max>=0) && (p_idx>max) )
		p_idx=max;

	point_2=p_idx;

}
void Selection::set_active(bool p_active) {

	active=p_active;
}


int Selection::get_point_1() {

	return point_1;
}
int Selection::get_point_2() {

	return point_2;

}
void Selection::set_max(int p_max) {

	max=p_max;
}

int Selection::get_begin() {

	return (point_1<point_2)?point_1:point_2;
}

int Selection::get_end() {

	return (point_1>point_2)?point_1:point_2;

}
bool Selection::is_active() {

	return active;
}

bool Selection::is_in_range(int p_idx) {

	return (active && (p_idx>=get_begin()) && (p_idx<=get_end()));

}

void Selection::clear() {
	active=false;
	point_1=0;
	point_2=0;
	max=-1;

}

Selection::Selection() {

	clear();
}

