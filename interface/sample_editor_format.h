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

#ifndef SAMPLE_EDITOR_FORMAT_H
#define SAMPLE_EDITOR_FORMAT_H


#include "containers/grid_container.h"
#include "widgets/slider.h"
#include "widgets/label.h"
#include "widgets/line_edit.h"
#include "widgets/value_label.h"
#include "bundles/combo_box.h"
#include "bundles/spin_box.h"
#include "mixer/sample_manager.h"
#include "interface/sample_viewer.h"


/**
 *
 * Juan Linietsky
 **/
 

 
class SampleEditorFormat : public VBoxContainer {


	int tune_center_freq;
	
	SampleManager *sm;
	bool updating;
	bool reseting_tune_freq;
	bool updating_tune_freq;
	Label * label_size;
	Label * label_format;
	Label * label_base_freq;

	Label * label_loop_type;
	Label * label_loop_begin;
	Label * label_loop_end;

	LineEdit *entry_size;
	LineEdit *entry_format;

  	ComboBox* combo_loop_type;
	SpinBox*spin_loop_begin;
	SpinBox *spin_loop_end;
	SpinBox *spin_base_freq;
	
	Sample_ID sample;
	
	SampleViewer *sample_viewer;

	void update_format();
	void update_loop();
	void reset_tune_freq();

	void tune_freq_changed_cbk(double p_frq);
	void loop_begin_changed_cbk(double p_idx);
	void loop_end_changed_cbk(double p_idx);
	void loop_type_changed_cbk(int p_idx);

	void base_freq_changed(double p_frq);

	void transpose_up_1_cbk();
	void transpose_up_12_cbk();

	void transpose_down_1_cbk();
	void transpose_down_12_cbk();

	HSlider *transp;
	
public:
	Signal<> sample_base_freq_changed;
	Signal<> sample_modified;


	void set_sample(Sample_ID p_sample);
	void set_sample_viewer(SampleViewer *p_sample_viewer);
	
	SampleEditorFormat();
	~SampleEditorFormat();
};



#endif
