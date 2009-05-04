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

#ifndef ENVELOPE_POINT_EDITOR_H
#define ENVELOPE_POINT_EDITOR_H


#include "base/widget.h"
#include "song/envelope.h"
#include "tracker/editor.h"

using namespace GUI;
/**
 *
 * Juan Linietsky
 **/
class EnvelopePointEditor : public Widget {

	enum {

		MIN_DRAG_DISTANCE=8 // min drag distance
	};

	float zoom;
	
	int get_font_h_spacing();
	

	Envelope *envelope;

	struct Grab {

		bool moving_point;
		int node;
		int x,y;
	} grab;

	bool adjust_zoom_to_window();
	void paint_envelope_points(Painter *p);

	void move_mouse_to_selected_node();

	bool recalc_pending;


	void draw_envelope_pos(int p_x,Painter *p);
        
	Size size;
	/* Events & Ddefault Handlers */
	virtual void resize(const Size& p_new_size);
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask); ///< either pressed or released,
	
	virtual void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	
	Editor *editor;

public:

	Signal<> envelope_changed;



	void set_envelope(Envelope *p_envelope);
	
	
	EnvelopePointEditor(Editor *p_editor);
	~EnvelopePointEditor();



};

#endif
