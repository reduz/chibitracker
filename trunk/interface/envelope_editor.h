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

#ifndef ENVELOPE_EDITOR_H
#define ENVELOPE_EDITOR_H



#include "containers/box_container.h"
#include "bundles/margin_group.h"
#include "widgets/label.h"
#include "widgets/check_button.h"
#include "containers/grid_container.h"
#include "bundles/spin_box.h"

#include "interface/envelope_point_editor.h"


/**
 *
 * Juan Linietsky
 **/
class EnvelopeEditor : public VBoxContainer {


	Editor *editor;

	EnvelopePointEditor *envelope_point_editor;

	HBoxContainer *control_hbox;

        CheckButton *enabled;
	CheckButton *carry;


        CheckButton *loop_enabled;
	SpinBox * loop_begin;
	SpinBox * loop_end;

        CheckButton *sustain_loop_enabled;
	SpinBox * sustain_loop_begin;
	SpinBox * sustain_loop_end;

	bool updating;

	Envelope *envelope;

	void update_envelope_variables();

	/* Slots */
	void envelope_changed();
	void loop_begin_changed(double p_val);
	void loop_end_changed(double p_val);
	void sustain_loop_begin_changed(double p_val);
	void sustain_loop_end_changed(double p_val);

	void envelope_enabled_check(bool p_on);
	void envelope_carry_check(bool p_on);
	void loop_enabled_check(bool p_on);
	void sustain_enabled_check(bool p_on);
public:

	EnvelopePointEditor* get_point_editor() { return envelope_point_editor; }
	HBoxContainer * get_control_box();
	void set_envelope(Envelope *p_envelope);

	EnvelopeEditor(Editor *editor,bool p_mini=false);
	~EnvelopeEditor();
};

#endif
