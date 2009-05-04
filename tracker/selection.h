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
// C++ Interface: selection
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SELECTION_H_
#define SELECTION_H_

class Selection {

	bool active;
	int point_1;
	int point_2;
	int max;

public:

	void set_point_1(int p_idx);
	void set_point_2(int p_idx);
	void set_active(bool p_active);
	void set_max(int p_max);

	int get_point_1();
	int get_point_2();

	int get_begin();
	int get_end();

	int get_size();

	bool is_in_range(int p_idx);

	bool is_active();

	void clear();

	Selection();
};

#endif /* selection.h */

