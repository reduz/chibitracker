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
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "envelope_editor.h"

void EnvelopeEditor::envelope_changed() {

	update_envelope_variables();
}


void EnvelopeEditor::loop_begin_changed(double p_val) {

	if (updating) return;

	envelope->set_loop_begin((int)p_val);
	update_envelope_variables();

}

void EnvelopeEditor::loop_end_changed(double p_val) {

	if (updating) return;
	envelope->set_loop_end((int)p_val);

	update_envelope_variables();
}

void EnvelopeEditor::sustain_loop_begin_changed(double p_val) {

	if (updating) return;
	envelope->set_sustain_loop_begin((int)p_val);

	update_envelope_variables();
}

void EnvelopeEditor::sustain_loop_end_changed(double p_val) {
	if (updating) return;
	envelope->set_sustain_loop_end((int)p_val);

	update_envelope_variables();
}

void EnvelopeEditor::envelope_enabled_check(bool p_on) {
	if (updating) return;

	envelope->set_enabled(p_on);
	envelope_point_editor->update();
}

void EnvelopeEditor::envelope_carry_check(bool p_on) {
	
	if (updating) return;

	envelope->set_carry_enabled( p_on );
	
}
	

void EnvelopeEditor::loop_enabled_check(bool p_on) {

	if (updating) return;

	envelope->set_loop_enabled(p_on);
	envelope_point_editor->update();

}
void EnvelopeEditor::sustain_enabled_check(bool p_on) {
	if (updating) return;

	envelope->set_sustain_loop_enabled(p_on);
	envelope_point_editor->update();

}


void EnvelopeEditor::update_envelope_variables() {

	if (!envelope) return;

	updating=true;
	enabled->set_checked(envelope->is_enabled());
	carry->set_checked(envelope->is_carry_enabled());

	loop_begin->get_range()->set_min(0);
	loop_begin->get_range()->set_max(envelope->get_loop_end());
	loop_begin->get_range()->set(envelope->get_loop_begin());
	loop_end->get_range()->set_min(envelope->get_loop_begin());
	loop_end->get_range()->set_max(envelope->get_node_count()-1);
	loop_end->get_range()->set(envelope->get_loop_end());

	loop_enabled->set_checked(envelope->is_loop_enabled());

	sustain_loop_begin->get_range()->set_min(0);
	sustain_loop_begin->get_range()->set_max(envelope->get_sustain_loop_end());
	sustain_loop_begin->get_range()->set(envelope->get_sustain_loop_begin());
	sustain_loop_end->get_range()->set_min(envelope->get_sustain_loop_begin());
	sustain_loop_end->get_range()->set_max(envelope->get_node_count()-1);
	sustain_loop_end->get_range()->set(envelope->get_sustain_loop_end());

	sustain_loop_enabled->set_checked(envelope->is_sustain_loop_enabled());

	envelope_point_editor->update();
	updating=false;

}


void EnvelopeEditor::set_envelope(Envelope *p_envelope) {


	envelope=p_envelope;
	envelope_point_editor->set_envelope(envelope);
	update_envelope_variables();
}

HBoxContainer * EnvelopeEditor::get_control_box() {

	return control_hbox;
}



EnvelopeEditor::EnvelopeEditor(Editor *p_editor,bool p_mini) {

	editor =p_editor;

	MarginGroup *mg = add( new MarginGroup(p_mini?"":"Envelope"),1 );
	envelope_point_editor = mg->add( new EnvelopePointEditor(p_editor), 1);
	
	
	mg = add( new MarginGroup(p_mini?"":"Control"),0 );

	control_hbox = mg->add( new HBoxContainer, 1);
	enabled = control_hbox->add( new CheckButton("Enable Envelope"),1);
	carry = control_hbox->add( new CheckButton("Carry"),1);

	mg = add( new MarginGroup(p_mini?"":"Loop"),0);
	
	GridContainer *table = mg->add( new GridContainer(4) );

	loop_enabled = table->add( new CheckButton("Enable Loop"),false,false);
	table->add( new Widget, true,false); //spacer
	
	sustain_loop_enabled = table->add( new CheckButton("Sustain Loop"),false,false);

	table->add( new Widget,true,false ); //spacer

	table->add(new Label("  Begin"),false,false);
	
	loop_begin=table->add( new SpinBox, true,false);
	
	
	table->add(new Label("  Begin"),false,false);
	
	sustain_loop_begin=table->add( new SpinBox, true,false);

	
	table->add(new Label("  End"),false,false);
	
	loop_end=table->add( new SpinBox, true,false);
	
	
	table->add(new Label("  End"),false,false);
	
	sustain_loop_end=table->add( new SpinBox, true,false);
	

	envelope=NULL;

	envelope_point_editor->envelope_changed.connect(  this, &EnvelopeEditor::envelope_changed );

	loop_begin->get_range()->value_changed_signal.connect<EnvelopeEditor>( this, &EnvelopeEditor::loop_begin_changed );
	loop_end->get_range()->value_changed_signal.connect( this, &EnvelopeEditor::loop_end_changed );
	
	sustain_loop_begin->get_range()->value_changed_signal.connect( this, &EnvelopeEditor::sustain_loop_begin_changed );
	sustain_loop_end->get_range()->value_changed_signal.connect( this, &EnvelopeEditor::sustain_loop_end_changed );
	
	enabled->toggled_signal.connect( this, &EnvelopeEditor::envelope_enabled_check );
	carry->toggled_signal.connect( this, &EnvelopeEditor::envelope_carry_check );
	loop_enabled->toggled_signal.connect( this, &EnvelopeEditor::loop_enabled_check );
	sustain_loop_enabled->toggled_signal.connect( this, &EnvelopeEditor::sustain_enabled_check );

	
	
}

EnvelopeEditor::~EnvelopeEditor()
{
}
