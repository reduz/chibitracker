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
#include "containers/center_container.h"
#include "widgets/separator.h"
#include "sample_editor.h"
#include "reverb.h"
#include <stdio.h>

void SampleEditor::update_timer_callback() {

	sample_viewer_positions->update();
}

void SampleEditor::data_modification_begin_notify_cbk() {


}

void SampleEditor::data_modification_end_notify_cbk() {

	sample_viewer->set_zoom_offset( sample_viewer->get_zoom_offset() );
	sample_viewer->sample_changed_notification();
	sample_viewer_zoom->update_zoom_bar();
	sample_editor_format->set_sample(sample);

}


void SampleEditor::set_sample(Sample_ID p_sample) {
	
	effects.set_sample(p_sample);
	sample_viewer->set_sample(p_sample);
	sample_viewer_zoom->set_sample(p_sample);
	sample_editor_format->set_sample(p_sample);
	//sample_editor_clipboard->set_sample(p_sample);
	//sample_editor_effects->set_sample(p_sample_data);

	sample=p_sample;
	selection.clear();

}
void SampleEditor::destructive_operation_begin_cbk() {
	
	
	destructive_operation_begin_signal.call();
}

void SampleEditor::destructive_operation_end_cbk() {

	if (!sample.is_null()) {
		if (selection.get_point_1()>=sm->get_size( sample )) {

			selection.set_point_1(sm->get_size( sample )-1);
		}
		if (selection.get_point_2()>=sm->get_size( sample )) {

			selection.set_point_2(sm->get_size( sample )-1);
		}
		if (selection.get_point_2()==selection.get_point_1()) {
			selection.clear();
		}
	} else {
		selection.clear();
	}

	//sample_editor_clipboard->set_sample(sample_data);
	sample_viewer->set_zoom_offset( sample_viewer->get_zoom_offset() );
	sample_viewer->sample_changed_notification();
//	sample_editor_effects->set_sample(sample_data); //if destructive, sorry!

	sample_viewer_zoom->update_zoom_bar();

	sample_editor_format->set_sample(sample);

	destructive_operation_end_signal.call();

}

void SampleEditor::sample_freq_changed_cbk() {
	
	sample_freq_changed_signal.call();
}

void SampleEditor::sample_modified_cbk() {
	
	sample_modified_signal.call();
}


void SampleEditor::fx_command(int p_command) {
	
	if (sample.is_null())
		return;
	
	if (p_command==CMD_REVERB) {
		
		printf("REVERBIE\n");
		reverb.win->show();
		return;
	} else if (p_command==CMD_ALIGN_SALL_SAMPLES) {
	
		current_fx=p_command;
		fx_param->get_range()->set_min( 2 );
		fx_param->get_range()->set_max( 64 );
		fx_param->get_range()->set( 8 );
					
		fx_group->set_label_text( "All Samples Alignment:" );
			
		fx_param_window->show();
			
		return;
	} else if (p_command==CMD_UNROLL_ALL_LOOPS) {
	
		current_fx=p_command;
		
		apply_unroll_all_loops();
			
		sample_modified_signal.call();
		return;
	} else if (p_command==CMD_AMPLIFY_ALL) {

		current_fx=p_command;
		fx_param->get_range()->set_min( 0 );
		fx_param->get_range()->set_max( 500 );
		fx_param->get_range()->set( 30 );

		fx_group->set_label_text( "All Samples Amplify:" );

		fx_param_window->show();

		return;

	} else if (p_command==CMD_CONVERT_ALL_TO_16) {


		apply_to16_all();
		return;
	}
	
	
	if (effects.command_has_parameter( p_command )) {
	
		current_fx=p_command;
		int cmd_def,cmd_min,cmd_max;
		effects.command_set_parameter_range( p_command, cmd_min,cmd_max,cmd_def );
		
		fx_param->get_range()->set_min( cmd_min );
		fx_param->get_range()->set_max( cmd_max );
		fx_param->get_range()->set( cmd_def );
		
		switch(p_command) {
			case SampleEditorEffects::CMD_AMPLIFY: {
						
				fx_group->set_label_text( "Amplify (%):" );
			} break;
			case SampleEditorEffects::CMD_PUNCH_ENVELOPE: {

				fx_group->set_label_text( "Attack Amp (%):" );
			} break;
			case SampleEditorEffects::CMD_ADD_SILENCE_AT_END: {
						
				fx_group->set_label_text( "Insert Silence (frames):" );
			} break;
			case SampleEditorEffects::CMD_FIX_LOOPING: {
			
				fx_group->set_label_text( "XFade Size (%):" );
		
			} break;
			case SampleEditorEffects::CMD_RESAMPLE: {
			
				fx_group->set_label_text( "New Length (frames):" );
			
			} break;			
			case SampleEditorEffects::CMD_RESAMPLE_FREQ: {
			
				fx_group->set_label_text( "New c5 Freq (hz):" );
			
			} break;			
			case SampleEditorEffects::CMD_LEFT_CHAN_DELAY: {
			
				fx_group->set_label_text( "Delay (frames):" );
			
			} break;
			case SampleEditorEffects::CMD_LOOP_TO_BOUNDARY: {
			
				fx_group->set_label_text( "Sample Alignment:" );
			
			} break;
			case SampleEditorEffects::CMD_MATCH_LOOP_END_BY_NOTE: {

				fx_group->set_label_text( "Sample Note#" );

			} break;

		}
		
		
		fx_param_window->show();
	} else {
		effects.command( p_command );
		sample_viewer->update();
		sample_viewer_zoom->update_zoom_bar();
		sample_editor_format->set_sample(sample);
		
	}
	
		
		
	
}


void SampleEditor::apply_fx() {

	if (current_fx==CMD_ALIGN_SALL_SAMPLES) {
	
		apply_alignment_to_all_samples( (int)fx_param->get_range()->get() );
		fx_param_window->hide();
		return;
	} else if (current_fx==CMD_AMPLIFY_ALL) {

		apply_amplify_to_all_samples( (int)fx_param->get_range()->get() );
		fx_param_window->hide();
		return;
	}
	
	fx_param_window->hide();
	effects.command( current_fx,(int)fx_param->get_range()->get() );
	
	sample_viewer->update();
	sample_viewer_zoom->update_zoom_bar();
	sample_editor_format->set_sample(sample);
	
}

void SampleEditor::apply_unroll_all_loops() {

	Song * s= editor->get_song();
	
	
	for (int i=0;i<Song::MAX_SAMPLES;i++) {
	
		Sample_ID smp = s->get_sample(i)->get_sample_data();
		if (!smp.is_null()) {
		
			effects.set_sample( smp );
			effects.command( SampleEditorEffects::CMD_LOOP_UNROLL, 0);
		}
	}
	
	effects.set_sample( sample );
}


void SampleEditor::apply_alignment_to_all_samples(int p_align) {

	Song * s= editor->get_song();
	
	
	for (int i=0;i<Song::MAX_SAMPLES;i++) {
	
		Sample_ID smp = s->get_sample(i)->get_sample_data();
		if (!smp.is_null()) {
		
			effects.set_sample( smp );
			effects.command( SampleEditorEffects::CMD_LOOP_TO_BOUNDARY, p_align );
		}
	}
	
	effects.set_sample( sample );
	sample_modified_signal.call();

}


void SampleEditor::apply_to16_all() {

	Song * s= editor->get_song();


	for (int i=0;i<Song::MAX_SAMPLES;i++) {

		Sample_ID smp = s->get_sample(i)->get_sample_data();
		if (!smp.is_null()) {


			if (!SampleManager::get_singleton()->is_16bits(smp)) {
				effects.set_sample( smp );
				effects.command( SampleEditorEffects::CMD_TOGGLE_DEPTH );
			}
		}
	}

	effects.set_sample( sample );
	sample_modified_signal.call();
}

void SampleEditor::apply_amplify_to_all_samples(int p_align) {

	Song * s= editor->get_song();


	for (int i=0;i<Song::MAX_SAMPLES;i++) {

		Sample_ID smp = s->get_sample(i)->get_sample_data();
		if (!smp.is_null()) {

			effects.set_sample( smp );
			effects.command( SampleEditorEffects::CMD_AMPLIFY, p_align );
		}
	}

	effects.set_sample( sample );
	sample_modified_signal.call();

}

void SampleEditor::apply_reverb() {
	
	reverb.win->hide();
	
	
	int ch=sm->is_stereo( sample )?2:1;
	
	for (int c=0;c<ch;c++) {
		
		ReShaked::Reverb rev; //borrowed from reshaked :)
		float rs=reverb.room->get_range()->get();
		rs-=(float)c*0.03;
		if (rs<=0.01)
			rs=01;
		rev.set_room_size( reverb.room->get_range()->get() );
		rev.set_damp( reverb.damp->get_range()->get() );
		rev.set_wet( reverb.wet->get_range()->get() );
		rev.set_dry( reverb.dry->get_range()->get() );
		rev.set_predelay( reverb.predly->get_range()->get() );
		rev.set_predelay_feedback( reverb.predly_fb->get_range()->get() );
		rev.set_highpass( reverb.hpf->get_range()->get() );
		rev.set_mix_rate( sm->get_c5_freq( sample ) );
		
		for (int i=0;i<sm->get_size( sample );i++ ) {
			
			float in=sm->get_data( sample, i , c );
			in/=32768;
			float out;
			rev.process(&in,&out,1);
			Sint16 od=(Sint16)(out*32767);
			sm->set_data( sample, i , od, c );
			
		}
	}
	
	sample_viewer->update();
	sample_viewer_zoom->update_zoom_bar();
	sample_editor_format->set_sample(sample);
	
}

void SampleEditor::set_in_window() {
	
	fx_param_window = new Window(get_window(),Window::MODE_POPUP,Window::SIZE_CENTER);
	VBoxContainer *vbc = new VBoxContainer;
	fx_param_window->set_root_frame( vbc );
	fx_group=vbc->add( new MarginGroup("Param"),0);
	fx_param=fx_group->add( new SpinBox, 0);
	vbc->add(new CenterContainer)->set( new Button("Apply"))->pressed_signal.connect(this,&SampleEditor::apply_fx);
	current_fx=-1;
	
	/* Reve */
		
	reverb.win=new Window(get_window(),Window::MODE_POPUP,Window::SIZE_CENTER);
	vbc = new VBoxContainer;
	reverb.win->set_root_frame( vbc );
	reverb.room=vbc->add(new MarginGroup("Room Size"))->add( new HSlider );
	reverb.room->get_range()->config(0,1,0.7,0.01);
	reverb.damp=vbc->add(new MarginGroup("Damping"))->add( new HSlider );
	reverb.damp->get_range()->config(0,1,0.5,0.01);
	reverb.wet=vbc->add(new MarginGroup("Wet"))->add( new HSlider );
	reverb.wet->get_range()->config(0,1,0.4,0.01);
	reverb.dry=vbc->add(new MarginGroup("Dry"))->add( new HSlider );
	reverb.dry->get_range()->config(0,1,0.8,0.01);
	reverb.predly=vbc->add(new MarginGroup("PreDelay"))->add( new HSlider );
	reverb.predly->get_range()->config(0,500,200,1);
	reverb.predly_fb=vbc->add(new MarginGroup("PreDelay Fbk"))->add( new HSlider );
	reverb.predly_fb->get_range()->config(0,1,0,0.01);
	reverb.hpf=vbc->add(new MarginGroup("HPF"))->add( new HSlider );
	reverb.hpf->get_range()->config(0,1,0.2,0.01);
	vbc->add(new CenterContainer)->set( new Button("Apply"))->pressed_signal.connect(this,&SampleEditor::apply_reverb);

		
	
	
	
}


SampleEditor::SampleEditor(Editor *p_editor,Mixer *p_mixer,bool p_mini) {

	editor=p_editor;
	/*
	sample_editor_clipboard = add("Edit",new SampleEditorClipboard );
	
	sample_editor_clipboard->set_selection(&selection);
	
	sample_editor_clipboard->destructive_operation_begin.connect( this, &SampleEditor::destructive_operation_begin_cbk );
	sample_editor_clipboard->destructive_operation_end.connect( this, &SampleEditor::destructive_operation_end_cbk );
	
	sample_editor_clipboard->sample_modified.connect( this, &SampleEditor::sample_modified_cbk );
	*/
	
	effects.set_selection( &selection );
	
	VBoxContainer *viewer_vb = add(new VBoxContainer,1 );
	viewer_vb->set_separation( 0 );
	
	sample_viewer_positions = viewer_vb->add( new SampleViewerPositions, 0 );
	
	sample_viewer = viewer_vb->add( new SampleViewer, 1 );
	sample_viewer->set_minimum_size( Size(0,30 ) );
	sample_viewer->set_selection(&selection);
	
	sample_viewer_positions->set_viewer( sample_viewer );
	sample_viewer_positions->set_mixer( p_mixer );

	sample_viewer_zoom=add( new SampleViewerZoom(sample_viewer), 0 );
	
	if (!p_mini)
		add( new HSeparator,0 );

	mini_tabbox=p_mini?add(new TabBox):0;

	if (p_mini) 
		sample_editor_format=mini_tabbox->add("Format:",new SampleEditorFormat);
	else
		sample_editor_format=add( new MarginGroup("Format:"), 0)->add( new SampleEditorFormat ,0 );
	
	sample_editor_format->set_sample_viewer(sample_viewer);
	
	
	sample_editor_format->sample_base_freq_changed.connect( this, &SampleEditor::sample_freq_changed_cbk );
	
	sample_editor_format->sample_modified.connect( this, &SampleEditor::sample_modified_cbk );



	/*
	sample_editor_effects = new SampleEditor_Effects(tabs);
	sample_editor_effects->setMargin(5);
	tabs->addTab(sample_editor_effects,"Effects");
	sample_editor_effects->set_selection(&selection);

	
	QObject::connect(sample_editor_effects,SIGNAL(data_modification_begin_notify()),this,SLOT(data_modification_begin_notify_cbk()));
	QObject::connect(sample_editor_effects,SIGNAL(data_modification_end_notify()),this,SLOT(data_modification_end_notify_cbk()));

	QObject::connect(sample_editor_effects,SIGNAL(destructive_operation_begin()),this,SIGNAL(destructive_operation_begin()));
	QObject::connect(sample_editor_effects,SIGNAL(destructive_operation_end()),this,SLOT(destructive_operation_end_cbk()));
	QObject::connect(sample_editor_effects,SIGNAL(sample_modified()),this,SIGNAL(sample_modified()));
	*/
	/*
	add_edit_effect(&edit_effects.sel_to_loop);
	add_edit_effect(&edit_effects.fade_in);
	add_edit_effect(&edit_effects.fade_out);
	add_edit_effect(&edit_effects.reverse);
	add_edit_effect(&edit_effects.center);
	add_edit_effect(&edit_effects.normalize);
	add_edit_effect(&edit_effects.pre_cut);
	add_edit_effect(&edit_effects.post_cut);
	add_edit_effect(&edit_effects.tunner);
	add_edit_effect(&edit_effects.toggle_sign);
	add_edit_effect(&edit_effects.toggle_depth);
	*/

	sm=SampleManager::get_singleton();
}

SampleEditor::~SampleEditor() {
	
}


