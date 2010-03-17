//
// C++ Implementation: sample_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "containers/grid_container.h"
#include "widgets/separator.h"
#include "sample_screen.h"
#include "tracker/key_bindings.h"
#include <stdio.h>


void SampleScreen::update_timer_callback() {

	sample_editor->update_timer_callback();
	sample_status->update();
}


void SampleScreen::select(int p_which) {

	selecting_outside=true;
	sample_list->select( p_which );
	list_sample_selected( p_which );
	selecting_outside=false;
}

void SampleScreen::handle_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {

	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return;
	
	/* test if this is a note */
	int note=tracker->editor->get_note_from_charcode( kbind );
	if (note>=0) {
		
		if (p_press)
			tracker->virtual_piano->sample_press_key( note);
		else
			tracker->virtual_piano->sample_stop( note );
		
		return;
	} else {
		
		if (!p_press)
			return;
				  
		switch (kbind) {
			
			case KB_CURSOR_PAGE_UP: {
				
				select( selected -1 );
			} break;
			case KB_CURSOR_PAGE_DOWN: {
				
				select( selected +1 );
			} break;
			default:{}
		}
			
		
	}

}


void SampleScreen::parameter_changed() {

	if (updating)
		return;
	
	/* Why bothering changing one by one? changing all is easiser */
		
	
	Sample *s=tracker->song->get_sample( selected );
	
	if (!s)
		return;
	s->set_global_volume((int)global_volume->get_range()->get());
	s->set_default_volume((int)default_volume->get_range()->get());
	s->set_pan((int)default_pan->get_range()->get()+32);
	s->set_pan_enabled(use_default_pan->is_checked());
	s->set_vibrato_type((Sample::VibratoType)((int)vibrato_type->get_selected()) );

	s->set_vibrato_speed((int)vibrato_speed->get_range()->get());
	s->set_vibrato_depth((int)vibrato_depth->get_range()->get());
	s->set_vibrato_rate((int)vibrato_rate->get_range()->get());
	
	

}

void SampleScreen::parameter_changed_c(int p_v) {
	
	parameter_changed();
	
}

void SampleScreen::parameter_changed_p(double p_v) {
	
	parameter_changed();
}


void SampleScreen::list_sample_selected(int p_which) {

	selected=p_which;	
	Sample *s=tracker->song->get_sample( p_which );
	
	
	if (!s)
		return;
	
	updating=true;
	sample_editor->set_sample( s->get_sample_data() );
	global_volume->get_range()->set(s->get_global_volume());
	default_volume->get_range()->set(s->get_default_volume());
	default_pan->get_range()->set(s->get_pan()-32);
	use_default_pan->set_checked( s->is_pan_enabled() );
	vibrato_type->select( s->get_vibrato_type() );
	vibrato_speed->get_range()->set(s->get_vibrato_speed());
	vibrato_depth->get_range()->set(s->get_vibrato_depth());
	vibrato_rate->get_range()->set(s->get_vibrato_rate());
	
	tracker->virtual_piano->sample_set( s->get_sample_data() );
	
	if (!selecting_outside) {
		
		if (tracker->song->has_instruments()) {
			/* find instrument that has this sample */	
			int found=-1;
			
			for (int i=0;i<Song::MAX_INSTRUMENTS;i++) {
				
				Instrument *ins=tracker->song->get_instrument(i);
				if (!ins)
					continue;
				
				for (int j=0;j<Note::NOTES;j++) {
					
					if (ins->get_sample_number(j)==p_which) {
						
						found=i;
						break;
					}
				}
				
				if (found>=0)
					break;
			}
			
			if (found>=0) {
				
				tracker->editor->set_instrument_mask_value( found );
				instrument_changed_signal.call( found );
			}
			
		} else {
			
			tracker->editor->set_instrument_mask_value( p_which );
		}
		
	}
	
	updating=false;
}

void SampleScreen::update_components() {

	for (int i=0;i<Song::MAX_SAMPLES;i++)
		sample_list->set_string( tracker->song->get_sample( i )->get_name(), i );

	sample_list->select( selected );
	list_sample_selected(selected);
	
}


void SampleScreen::sample_data_edit_menu_callback(int p_which) {
	

	sample_editor->fx_command( p_which );

}

void SampleScreen::sample_file_action_callback(int p_which) {
	
	Sample *smp = tracker->song->get_sample( selected );
	if (!smp)
		return;
	
	switch (p_which) {
		
		
		
		case MENU_CLEAR: {
			
			smp->reset();
			update_components();
		} break;
		case MENU_LOAD: {
			
			//file_dialog->add_filter("FT2 Instrument",".xi");
			sample_file_dialog->set_sample_to_load( selected );
			sample_file_dialog->show();
			
		} break;
		case MENU_SAVE: {
			if (tracker->song->get_sample( selected )->get_sample_data().is_null() )
				break;
			file_dialog->clear_filter_list();
			file_dialog->add_filter("IT Instrument",".its");
			file_dialog->add_filter("RIFF Wave File",".wav");
			file_dialog->show(FileDialog::MODE_SAVE);
			
		} break;
		case MENU_COPY: {
			
			clipboard=*smp;
			
		} break;
		case MENU_CUT: {
			
			clipboard=*smp;
			smp->reset();
			update_components();

		} break;
		case MENU_PASTE: {
			
			*smp=clipboard;
			update_components();
		} break;
		
		case MENU_MAKE_INSTRUMENT: {
			
			if (!tracker->song->has_instruments())
				break;
			
			tracker->song->make_instrument_from_sample( sample_list->get_selected() );
			instrument_list_changed_signal.call();
		} break;

		
	};

	
}

void SampleScreen::open_samples_callback() {
	
	update_components();
	instrument_list_changed_signal.call(); //may have made instruments
	/* Print errors.. */
}

void SampleScreen::save_sample_callback(String p_file) {

	if (file_dialog->get_mode()!=FileDialog::MODE_SAVE)
		return;
	
	tracker->file_manager.save_sample( p_file.utf8().get_data(), tracker->song->get_sample( selected ) );
}

void SampleScreen::set_in_window() {
	
	sample_file_dialog = new SampleFileDialog(get_window(),tracker);
	sample_file_dialog->samples_opened_signal.connect( this, &SampleScreen::open_samples_callback );

	
	file_dialog = new FileDialog(get_window());
	file_dialog->file_activated_signal.connect( this, &SampleScreen::save_sample_callback );
	
}


void SampleScreen::sample_name_edited(int p_which,String p_name) {
	
	Sample *smp=tracker->song->get_sample( p_which );	
	if (!smp)
		return;
	
	smp->set_name( p_name.ascii().get_data() );
}

void SampleScreen::set_sample_path(String p_path) {
	
	if (file_dialog)
		file_dialog->set_dir( p_path );
	if (sample_file_dialog)
		sample_file_dialog->set_dir( p_path );
}

SampleScreen::SampleScreen(Tracker *p_tracker,bool p_mini) {

	
	mini_tabbox = p_mini?add(new TabBox,1):0;

	tracker=p_tracker;

	MarginGroup *mg=p_mini?mini_tabbox->add("List",new MarginGroup("Sample List")):add(new MarginGroup("Sample List"),3);
	
	mg->get_label_hb()->add( new Widget, 1);
	edit = mg->get_label_hb()->add( new MenuBox("File.."));
	edit->add_item( "Clear" );
	edit->add_separator();
	edit->add_item( "Load" );
	edit->add_item( "Save" );
	edit->add_separator();
	edit->add_item( "Copy" );
	edit->add_item( "Cut" );
	edit->add_item( "Paste" );
	edit->add_separator();
	edit->add_item( "Make Instrument" );
	edit->item_selected_signal.connect( this, &SampleScreen::sample_file_action_callback );
	
	HBoxContainer *slit_hb=mg->add(new HBoxContainer,1);
	
	sample_status=slit_hb->add( new SampleStatusList(p_tracker),0);
	sample_list=slit_hb->add( new List, 1 );
	slit_hb->add( new VScrollBar, 0 )->set_range( sample_list->get_range() );
	sample_status->set_range( sample_list->get_range() );
	
	for (int i=0;i<Song::MAX_SAMPLES;i++)
		sample_list->add_string( "" );
	
	sample_list->set_number_elements( 1 );
	sample_list->set_number_min_integers( 2 );
	sample_list->set_editable( true );
	sample_list->set_max_string_length( 25 );
	sample_list->set_incremental_search(false);
	
	mg=p_mini?mini_tabbox->add("Edit",new MarginGroup("Edit:")):add(new MarginGroup("Parameters:"),5);
	
	mg->get_label_hb()->add( new Widget, 1);
	
//	mg->get_label_hb()->add( new Widget, 1);
	sample_edit = mg->get_label_hb()->add( new MenuBox("Edit"));
	sample_edit->add_item("Copy",SampleEditorEffects::CMD_COPY);
	sample_edit->add_item("Cut",SampleEditorEffects::CMD_CUT);
	sample_edit->add_item("Paste",SampleEditorEffects::CMD_PASTE);
	sample_edit->add_item("Trim",SampleEditorEffects::CMD_TRIM);
	sample_edit->add_separator();
	sample_edit->add_item("PreLoop Cut",SampleEditorEffects::CMD_PRELOOP_CUT);
	sample_edit->add_item("PostLoop Cut",SampleEditorEffects::CMD_POSTLOOP_CUT);
	sample_edit->add_separator();
	sample_edit->add_item("Selection->Loop",SampleEditorEffects::CMD_SEL_TO_LOOP);
	sample_edit->add_item("Align Loop by Resizing",SampleEditorEffects::CMD_LOOP_TO_BOUNDARY);
	sample_edit->add_separator();
	sample_edit->add_item("Unroll Loop",SampleEditorEffects::CMD_LOOP_UNROLL);
	
	sample_edit->item_selected_signal.connect(this,&SampleScreen::sample_data_edit_menu_callback);
	sample_edit->add_separator();
	sample_edit->add_item("Insert Silence at End",SampleEditorEffects::CMD_ADD_SILENCE_AT_END);
	sample_edit->add_separator();
	sample_edit->add_item("Align Loop (ALL SAMPLES)",SampleEditor::CMD_ALIGN_SALL_SAMPLES);
	sample_edit->add_item("Unroll Loop (ALL SAMPLES)",SampleEditor::CMD_UNROLL_ALL_LOOPS);
	
	
	sample_fx = mg->get_label_hb()->add( new MenuBox("FX"));
	sample_fx->add_item("Toggle Sign",SampleEditorEffects::CMD_TOGGLE_SIGN);
	sample_fx->add_item("Toggle Depth",SampleEditorEffects::CMD_TOGGLE_DEPTH);
	sample_fx->add_item("Toggle Stereo",SampleEditorEffects::CMD_TOGGLE_STEREO);
	sample_fx->add_item("Swap Stereo Chans",SampleEditorEffects::CMD_SWAP_STEREO_CHANS);
	sample_fx->add_separator();
	sample_fx->add_item("Amplify",SampleEditorEffects::CMD_AMPLIFY);
	sample_fx->add_item("Fade In",SampleEditorEffects::CMD_FADE_IN);
	sample_fx->add_item("Fade Out",SampleEditorEffects::CMD_FADE_OUT);
	sample_fx->add_separator();
	sample_fx->add_item("Center",SampleEditorEffects::CMD_CENTER);
	sample_fx->add_item("Reverse",SampleEditorEffects::CMD_REVERSE);
	sample_fx->add_separator();
	sample_fx->add_item("Fix Looping",SampleEditorEffects::CMD_FIX_LOOPING);
	sample_fx->add_item("Resample (Frames)",SampleEditorEffects::CMD_RESAMPLE);
	sample_fx->add_item("Resample (hz)",SampleEditorEffects::CMD_RESAMPLE_FREQ);
	sample_fx->add_item("Delay Left Channel",SampleEditorEffects::CMD_LEFT_CHAN_DELAY);
	sample_fx->add_item("Match Loop End",SampleEditorEffects::CMD_MATCH_LOOP_END_BY_NOTE);
	sample_fx->add_separator();
	sample_fx->add_item("Reverb",SampleEditor::CMD_REVERB);
	
	sample_fx->item_selected_signal.connect(this,&SampleScreen::sample_data_edit_menu_callback);
	
	sample_properties =mg->add( new VBoxContainer, 1 );
	
	sample_list->selected_signal.connect( this, &SampleScreen::list_sample_selected );
	sample_list->item_edited_signal.connect( this, &SampleScreen::sample_name_edited );
	
	sample_editor =  sample_properties->add( new SampleEditor(tracker->editor, tracker->mixer,p_mini), 1);
	
	if (!p_mini)
		sample_properties->add( new HSeparator,0 );
	/* Sample stuff */
	
	HBoxContainer *hbc = p_mini?sample_editor->get_mini_tabbox()->add("Vol/Pan/Vib",new HBoxContainer):sample_properties->add( new HBoxContainer, 0 );
	
	mg = hbc->add( new MarginGroup("Volume"),1);
	
	GridContainer *gc = mg->add( new GridContainer(3), 1);
	
	gc->add( new Label("Global"), false, false );
	
	global_volume=gc->add( new HSlider, true, false );
	gc->add( new ValueLabel, false, false )->set_range( global_volume->get_range() );
	
	global_volume->get_range()->value_changed_signal.connect( this, &SampleScreen::parameter_changed_p );
	
	gc->add( new Label("Default"), false, false );
	
	default_volume=gc->add( new HSlider, true, false );
	gc->add( new ValueLabel, false, false )->set_range( default_volume->get_range() );
	default_volume->get_range()->value_changed_signal.connect( this, &SampleScreen::parameter_changed_p );
	
	use_default_pan=gc->add( new CheckButton("Pan"), false, false );
	
	use_default_pan->pressed_signal.connect( this , &SampleScreen::parameter_changed );
	
	default_pan=gc->add( new HSlider, true, false );
	gc->add( new ValueLabel, false, false )->set_range( default_pan->get_range() );
	
	default_pan->get_range()->value_changed_signal.connect( this, &SampleScreen::parameter_changed_p );
	
	mg = hbc->add( new MarginGroup("Vibrato"),1);
	
	gc = mg->add( new GridContainer(3), 1);
	
	gc->add( new Label("Wave"), false, false );
	
	vibrato_type=gc->add( new ComboBox, true, false );
	
	vibrato_type->add_string( "Sine" );
	vibrato_type->add_string( "Saw" );
	vibrato_type->add_string( "Square" );
	vibrato_type->add_string( "Random" );
	
	vibrato_type->selected_signal.connect( this, &SampleScreen::parameter_changed_c );
	
	gc->add( new Widget, false, false );
	gc->add( new Label("Speed"), false, false );
	
	vibrato_speed=gc->add( new HSlider, true, false );
	gc->add( new ValueLabel, false, false )->set_range( vibrato_speed->get_range() );
	
	vibrato_speed->get_range()->value_changed_signal.connect( this, &SampleScreen::parameter_changed_p );
	
	gc->add( new Label("Depth"), false, false );
	
	vibrato_depth=gc->add( new HSlider, true, false );
	gc->add( new ValueLabel, false, false )->set_range( vibrato_depth->get_range() );
	
	vibrato_depth->get_range()->value_changed_signal.connect( this, &SampleScreen::parameter_changed_p );
	
	gc->add( new Label("Rate"), false, false );
	
	vibrato_rate=gc->add( new HSlider, true, false );
	gc->add( new ValueLabel, false, false )->set_range( vibrato_rate->get_range() );
	
	vibrato_rate->get_range()->value_changed_signal.connect( this, &SampleScreen::parameter_changed_p );
	
	global_volume->get_range()->set_max(64);
	default_volume->get_range()->set_max(64);
	default_pan->get_range()->set_min(-32);
	default_pan->get_range()->set_max(32);
	vibrato_speed->get_range()->set_max(64);
	vibrato_depth->get_range()->set_max(32);
	vibrato_rate->get_range()->set_max(64);
	
	selecting_outside=false;
	updating=false;	
	selected=0;	
	
	sample_file_dialog=0;
	file_dialog=0;
}


SampleScreen::~SampleScreen()
{
}


