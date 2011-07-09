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

#ifndef SAMPLE_EDITOR_H
#define SAMPLE_EDITOR_H


#include "base/window.h"
#include "widgets/button.h"
#include "widgets/label.h"
#include "widgets/scroll_bar.h"
#include "bundles/tab_box.h"

#include "interface/sample_viewer_positions.h"
#include "interface/sample_viewer_zoom.h"
#include "interface/sample_editor_format.h"
#include "interface/sample_editor_clipboard.h"
#include "interface/sample_editor_effects.h"
#include "tracker/editor.h"

#include "bundles/tab_box.h"

//#include "plugins/edit_effects/simple_edit_effects.h"


/**
 *
 * Juan Linietsky
 **/
class SampleEditor : public VBoxContainer {


	Editor *editor;
	SampleManager *sm;
	Sample_ID sample;

	SampleViewer * sample_viewer;

	
	SampleViewerZoom * sample_viewer_zoom;
	SampleViewerPositions * sample_viewer_positions;
	SampleEditorFormat *sample_editor_format;

	SampleEditorClipboard *sample_editor_clipboard;
	//SampleEditor_Effects *sample_editor_effects;

	Selection selection;

	
	
	
	/*
	struct Edit_Effects {

		Edit_Effect_Reverse reverse;
		Edit_Effect_PreLoop_Cut pre_cut;
		Edit_Effect_PostLoop_Cut post_cut;
		Edit_Effect_SelToLoop sel_to_loop;
		Edit_Effect_Center center;
		Edit_Effect_Normalize normalize;
		Edit_Effect_Toggle_Sign toggle_sign;
		Edit_Effect_Toggle_Depth toggle_depth;
		Edit_Effect_Tunner tunner;

		Edit_Effect_FadeIn fade_in;
		Edit_Effect_FadeOut fade_out;

	} edit_effects;
	*/


	void destructive_operation_begin_cbk();
	void destructive_operation_end_cbk();
	void data_modification_begin_notify_cbk();
	void data_modification_end_notify_cbk();
	void sample_freq_changed_cbk();
	void sample_modified_cbk();
	
	SampleEditorEffects effects;

	Window *fx_param_window;
	SpinBox *fx_param;
	MarginGroup *fx_group;
	int current_fx;
	void apply_fx();
	
	void set_in_window();
	
	struct ReverbWin {
		
		Window *win;
		HSlider *room;
		HSlider *damp;
		HSlider *wet;
		HSlider *dry;
		HSlider *predly;
		HSlider *predly_fb;
		HSlider *hpf;
	} reverb;
	void apply_reverb();
	void apply_alignment_to_all_samples(int p_align);
	void apply_unroll_all_loops();
	void apply_amplify_to_all_samples(int p_align);
	void apply_to16_all();

	TabBox *mini_tabbox;
public:
	
	enum {
		CMD_REVERB=SampleEditorEffects::CMD_MAX,
		CMD_ALIGN_SALL_SAMPLES,
		CMD_UNROLL_ALL_LOOPS,
		CMD_CONVERT_ALL_TO_16,
		CMD_AMPLIFY_ALL,
	};
	
	Signal<> sample_freq_changed_signal;
	Signal<> sample_modified_signal;

	Signal<> destructive_operation_begin_signal;
	Signal<> destructive_operation_end_signal;
	
	SampleViewer* get_sample_viewer() { return sample_viewer; }
	void set_sample(Sample_ID p_sample);

	void fx_command(int p_command);
	
	void update_timer_callback();
	
	TabBox *get_mini_tabbox() { return mini_tabbox; };

	SampleEditor(Editor *p_editor,Mixer *p_mixer,bool p_mini=false);
	~SampleEditor();
};





#endif

