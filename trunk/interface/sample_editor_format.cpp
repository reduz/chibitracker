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
#include "sample_editor_format.h"
#include <math.h>
#include "widgets/button.h"
#include "widgets/separator.h"


void SampleEditorFormat::tune_freq_changed_cbk(double p_frq) {
	
	if (updating)
		return;
	if (reseting_tune_freq)
		return;
	
	float freq=tune_center_freq;
	p_frq/=100;
	
	freq*=pow(2.0,p_frq/12.0);
	
	updating_tune_freq=true;
	spin_base_freq->get_range()->set(freq);
	updating_tune_freq=false;
	
	
}

void SampleEditorFormat::reset_tune_freq() {
	
	if (updating_tune_freq)
		return;
	
	reseting_tune_freq=true;
	
	transp->get_range()->set(0);
	tune_center_freq=sm->get_c5_freq(sample);
	
	reseting_tune_freq=false;
	
}
void SampleEditorFormat::transpose_up_1_cbk() {

	if (sample.is_null())
		return;

	float freq=sm->get_c5_freq(sample);
	freq*=pow(2.0,1.0/12.0);
	sm->set_c5_freq(sample,(int)freq);
	update_format();
	sample_base_freq_changed.call();
	reset_tune_freq();
	
}
void SampleEditorFormat::transpose_up_12_cbk() {

	if (sample.is_null())
		return;

	float freq=sm->get_c5_freq(sample);
	freq*=2.0;
	sm->set_c5_freq(sample,(int)freq);
	update_format();
	sample_base_freq_changed.call();
	reset_tune_freq();
}


void SampleEditorFormat::transpose_down_1_cbk() {

	if (sample.is_null())
		return;

	float freq=sm->get_c5_freq(sample);
	freq/=pow(2.0,1.0/12.0);
	sm->set_c5_freq(sample,(int)freq);
	update_format();
	sample_base_freq_changed.call();
	reset_tune_freq();
}
void SampleEditorFormat::transpose_down_12_cbk() {

	if (sample.is_null())
		return;

	float freq=sm->get_c5_freq(sample);
	freq/=2.0;
	sm->set_c5_freq(sample,(int)freq);
	update_format();
	sample_base_freq_changed.call();
	reset_tune_freq();
}

void SampleEditorFormat::base_freq_changed(double p_frq) {
	
	if (updating) return;

	if (sample.is_null())
		return;

	sm->set_c5_freq(sample,(int)p_frq);
	reset_tune_freq();	
	sample_modified.call();

}

void SampleEditorFormat::loop_begin_changed_cbk(double p_idx) {

	if (updating) return;
	if (sample.is_null())
		return;
	sm->set_loop_begin(sample,(int)spin_loop_begin->get_range()->get());
	update_loop();

}
void  SampleEditorFormat::loop_end_changed_cbk(double p_idx) {

	if (updating) return;
	if (sample.is_null())
		return;
	sm->set_loop_end(sample,(int)spin_loop_end->get_range()->get());
	update_loop();
}

void  SampleEditorFormat::loop_type_changed_cbk(int p_idx) {
	
	if (updating) return;
	if (sample.is_null())
		return;
	switch (p_idx) {

		case 0:{
			sm->set_loop_type( sample, LOOP_NONE );
		} break;
		case 1:{
			
			sm->set_loop_type( sample, LOOP_FORWARD );
		} break;
		case 2:{
			sm->set_loop_type( sample, LOOP_BIDI );
		} break;
	}
	sample_modified.call();
}

void SampleEditorFormat::update_format() {
	
	updating=true;
	
	if (sample.is_null()) {
		
		entry_size->set_text("0");
		entry_format->set_text("Unused");
		spin_base_freq->get_range()->set(0);
		combo_loop_type->select(0);
	} else {
	
	
		reset_tune_freq();
		
		entry_size->set_text(String::num( sm->get_size( sample ) ) );
		
		String format=sm->is_16bits(sample)?"16 Bits, ":"8 Bits, ";
		format+=sm->is_stereo(sample)?"Stereo":"Mono";
		
		entry_format->set_text(format);
		spin_base_freq->get_range()->set(sm->get_c5_freq(sample));
	
		combo_loop_type->select( sm->get_loop_type(sample) );

	
	}
	updating=false;
	sample_modified.call();
}

void SampleEditorFormat::update_loop() {
	
	updating=true;
	if (sample.is_null()) {
		
		spin_loop_begin->get_range()->set_max(0);
		spin_loop_begin->get_range()->set(0);
		spin_loop_end->get_range()->set_max(0);
		spin_loop_end->get_range()->set(0);
	} else {
		
	
		
	
		spin_loop_begin->get_range()->set_min(0);
		spin_loop_begin->get_range()->set_max(sm->get_loop_end(sample)-1);
		spin_loop_begin->get_range()->set(sm->get_loop_begin(sample));
	
		spin_loop_end->get_range()->set_min(sm->get_loop_begin(sample)+1);
		spin_loop_end->get_range()->set_max(sm->get_size( sample ));
		spin_loop_end->get_range()->set(sm->get_loop_end(sample));


		if (sample_viewer)
			sample_viewer->update();

	}
	updating=false;
	sample_modified.call();
	
	
	
}

void SampleEditorFormat::set_sample(Sample_ID p_sample) {

	sample=p_sample;
	update_format();
	update_loop();

}
void SampleEditorFormat::set_sample_viewer(SampleViewer *p_sample_viewer) {

	sample_viewer=p_sample_viewer;
}

SampleEditorFormat::SampleEditorFormat() {


	GridContainer *grid = add(new GridContainer(4),1);
	
	
	label_format = grid->add( new Label("Format:"), false, false );
	entry_format = grid->add( new LineEdit, true, false );
	entry_format->set_editable(false);
	
	
	label_loop_type = grid->add( new Label("Loop:"), false, false );
	combo_loop_type = grid->add( new ComboBox, true, false );

	combo_loop_type->add_string("Disabled");
	combo_loop_type->add_string("Forward ->");
	combo_loop_type->add_string("BiDi   <->");
	
	label_size = grid->add( new Label("Length:"), false, false );
	entry_size = grid->add( new LineEdit, true, false );
	entry_size->set_editable(false);
	
	label_loop_begin = grid->add( new Label(" Begin:"), false, false );
	spin_loop_begin = grid->add( new SpinBox, true, false );
	
	
	label_base_freq = grid->add( new Label("C5 Frq:"), false, false );
	spin_base_freq = grid->add( new SpinBox, true, false );
	
	label_loop_end = grid->add( new Label(" End:"), false, false );
	spin_loop_end = grid->add( new SpinBox, true, false );
	
	combo_loop_type->selected_signal.connect( this, &SampleEditorFormat::loop_type_changed_cbk );

	spin_loop_begin->get_range()->value_changed_signal.connect( this, &SampleEditorFormat::loop_begin_changed_cbk);
	
	spin_loop_end->get_range()->value_changed_signal.connect( this, &SampleEditorFormat::loop_end_changed_cbk);
	
	spin_base_freq->get_range()->value_changed_signal.connect( this, &SampleEditorFormat::base_freq_changed);
	spin_base_freq->get_range()->set_max(999999.0);
	
	updating=false;
	sample_viewer=NULL;

	
	HBoxContainer *hbtr=add( new HBoxContainer, 0);
	
	hbtr->add(new Label("Tune:"),0);
	transp = hbtr->add( new HSlider, 1 );
	transp->get_range()->set_min(-100);
	transp->get_range()->set_max(100);
	transp->get_range()->set(0);
	transp->get_range()->value_changed_signal.connect( this, &SampleEditorFormat::tune_freq_changed_cbk);
	hbtr->add( new ValueLabel, 0 )->set_range(transp->get_range());
	
	hbtr->add( new Button("Set"),0)->pressed_signal.connect( this, &SampleEditorFormat::reset_tune_freq);
	
	hbtr->add( new HSeparator, 0);
	
	hbtr->add( new Button("-1"),0)->pressed_signal.connect( this, &SampleEditorFormat::transpose_down_1_cbk);
	hbtr->add( new Button("+1"),0)->pressed_signal.connect( this, &SampleEditorFormat::transpose_up_1_cbk);
	
	hbtr->add( new HSeparator, 0);
	
	hbtr->add( new Button("-12"),0)->pressed_signal.connect( this, &SampleEditorFormat::transpose_down_12_cbk);
	hbtr->add( new Button("+12"),0)->pressed_signal.connect( this, &SampleEditorFormat::transpose_up_12_cbk);
	
	
	sm = SampleManager::get_singleton();

	updating_tune_freq=false;
	reseting_tune_freq=false;
}

SampleEditorFormat::~SampleEditorFormat()
{
}


