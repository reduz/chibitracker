//
// C++ Implementation: instrument_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "containers/center_container.h"
#include "instrument_screen.h"
#include "tracker/key_bindings.h"



void InstrumentScreen::select(int p_which) {

	instrument_list->select( p_which );
	instrument_selected( p_which );
}


void InstrumentScreen::handle_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {


	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return;
	
	/* test if this is a note */
	int note=tracker->editor->get_note_from_charcode( kbind );
	if (note>=0) {
		
		if (p_press)
			tracker->virtual_piano->instrument_press_key( note, 64 );
		else
			tracker->virtual_piano->instrument_stop_key( note );
		
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


void InstrumentScreen::update_components() {

	for (int i=0;i<Song::MAX_INSTRUMENTS;i++) {
		
		instrument_list->set_string( tracker->song->get_instrument(i)->get_name(), i );
	}

	instrument_selected( selected );
}

void InstrumentScreen::instrument_selected(int p_selected) {

	selected=p_selected;
	
	Instrument *ins=tracker->song->get_instrument( selected );
	if (!ins)
		return;
	
	updating=true;
	sample_table->set_instrument( ins );

	nna_group.set_current( ins->get_NNA_type() );
	dct_group.set_current( ins->get_DC_type() );
	dca_group.set_current( ins->get_DC_action() );
	
	vol_envelope->set_envelope( ins->get_volume_envelope() );
	
	vol_global_send->get_range()->set( (double) ins->get_volume_global_amount() );
	vol_random_send->get_range()->set( (double) ins->get_volume_random_variation() );
	vol_fadeout->get_range()->set( (double) ins->get_volume_fadeout() );

	pan_envelope->set_envelope( ins->get_pan_envelope() );
	
	pan_use_default->set_checked( ins->is_pan_default_enabled() );
	pan_default->get_range()->set( (double)ins->get_pan_default_amount() -32.0 );
	pan_ppsep->get_range()->set( (double) ins->get_pan_pitch_separation() );
	pan_ppcenter->get_range()->set( (double) ins->get_pan_pitch_center() );
	pan_random->get_range()->set( (double) ins->get_pan_random_variation());
			
			
	pitch_filter_envelope->set_envelope( ins->get_pitch_filter_envelope() );
	
	env_use_as_filter->set_checked( ins->is_pitch_use_as_filter() );
	filter_cutoff_default->set_checked( ins->filter_use_default_cutoff() );
	filter_cutoff->get_range()->set( (double) ins->get_filter_default_cutoff() );
	filter_reso_default->set_checked( ins->filter_use_default_resonance() );
	filter_reso->get_range()->set( (double) ins->get_filter_default_resonance() );
	
	updating=false;
	
	tracker->virtual_piano->instrument_set( selected );
	
	if (tracker->song->has_instruments()) {
		
		tracker->editor->set_instrument_mask_value( p_selected );
	}
}



void InstrumentScreen::parameter_changed() {
	
	if (updating)
		return;
	
	
	Instrument *ins=tracker->song->get_instrument( selected );
	if (!ins)
		return;
	updating=true;
	
	switch (nna_group.get_current()) {
		
		case Instrument::NNA_NOTE_CUT: ins->set_NNA_type( Instrument::NNA_NOTE_CUT ); break;
		case Instrument::NNA_NOTE_FADE: ins->set_NNA_type( Instrument::NNA_NOTE_FADE ); break;
		case Instrument::NNA_NOTE_OFF: ins->set_NNA_type( Instrument::NNA_NOTE_OFF ); break;
		case Instrument::NNA_NOTE_CONTINUE: ins->set_NNA_type( Instrument::NNA_NOTE_CONTINUE ); break;
	}
	
	switch (dca_group.get_current()) {
		
		case Instrument::DCA_NOTE_CUT: ins->set_DC_action( Instrument::DCA_NOTE_CUT ); break;
		case Instrument::DCA_NOTE_FADE: ins->set_DC_action( Instrument::DCA_NOTE_FADE ); break;
		case Instrument::DCA_NOTE_OFF: ins->set_DC_action( Instrument::DCA_NOTE_OFF ); break;
	}

	switch (dct_group.get_current()) {
		
		case Instrument::DCT_DISABLED: ins->set_DC_type( Instrument::DCT_DISABLED ); break;
		case Instrument::DCT_NOTE: ins->set_DC_type( Instrument::DCT_NOTE ); break;
		case Instrument::DCT_SAMPLE: ins->set_DC_type( Instrument::DCT_SAMPLE ); break;
		case Instrument::DCT_INSTRUMENT: ins->set_DC_type( Instrument::DCT_INSTRUMENT ); break;
	}
	
	
	ins->set_volume_global_amount( (int) vol_global_send->get_range()->get() );
	ins->set_volume_random_variation( (int)vol_random_send->get_range()->get() );
	
	ins->set_volume_fadeout( (int)vol_fadeout->get_range()->get() );
	
	
	ins->set_pan_default_enabled(pan_use_default->is_checked() );
	ins->set_pan_default_amount( (int)pan_default->get_range()->get() +32 );
	ins->set_pan_pitch_separation( (int)pan_ppsep->get_range()->get() );
	
	ins->set_pan_pitch_center((int)pan_ppcenter->get_range()->get() );
	
	ins->set_pan_random_variation((int)pan_random->get_range()->get());
			
			
	
	ins->set_pitch_use_as_filter( (int)env_use_as_filter->is_checked() );
	
	
	ins->set_filter_use_default_cutoff( filter_cutoff_default->is_checked() );
	ins->set_filter_default_cutoff( (int) filter_cutoff->get_range()->get() );
	
	ins->set_filter_use_default_resonance( filter_reso_default->is_checked() );
	ins->set_filter_default_resonance( (int) filter_reso->get_range()->get() );
	
	updating=false;
	
}

void InstrumentScreen::parameter_changed_i(int) {
	
	parameter_changed();
}
void InstrumentScreen::parameter_changed_p(double) {
	
	parameter_changed();
}

void InstrumentScreen::instrument_file_callback(String p_file) {
	
	file_dialog->hide();
	switch (file_dialog->get_mode()) {
		
		case FileDialog::MODE_SAVE: {
			

			Saver::Error err = tracker->file_manager.save_instrument( p_file.utf8().get_data(), tracker->song, selected );
			
			switch(err) {
					
				case Saver::SAVE_ERROR: error_msg->show("Error Saving (Can't Create file)"); break;
				case Saver::UNKNOWN_EXTENSION: error_msg->show("Unknown File Extension"); break;
				default:{};
			}
					
		} break;
		case FileDialog::MODE_OPEN_MULTI: {
			
			for (int i=0;i<file_dialog->get_file_count();i++) {
				
				if ((selected+i)>Song::MAX_INSTRUMENTS)
					break;
				
				String f=file_dialog->get_path(i);
				
				Loader::Error err = tracker->file_manager.load_instrument(  f.utf8().get_data(), tracker->song, selected+i );
				
				switch(err) {
					
					case Loader::FILE_CANNOT_OPEN: error_msg->show("Can't Open File!"); break;
					case Loader::FILE_CORRUPTED: error_msg->show("File Corrupted"); break;
					case Loader::FILE_OUT_OF_MEMORY: error_msg->show("Out Of Memory"); break;
					case Loader::FILE_UNRECOGNIZED: error_msg->show("Unknown File Type"); break;
					default:{};
				}
				
			}
		} break;
		default: {}
	};
	
	
	update_components();
}

void InstrumentScreen::instrument_file_callback_multi() {
	
	instrument_file_callback("");
}

void InstrumentScreen::instrument_file_action_callback(int p_which) {
	
	Instrument *ins = tracker->song->get_instrument( selected );
	if (!ins)
		return;
	
	switch (p_which) {
		
		
		case MENU_CLEAR: {
			
			ins->reset();
			update_components();
		} break;
		case MENU_CLEAR_ALL: {
			
			tracker->song->clear_instrument_with_samples( selected );
			sample_list_changed_signal.call();
			update_components();
			
		} break;
		case MENU_LOAD: {
			
			file_dialog->clear_filter_list();
			file_dialog->add_filter("IT Instrument",".iti");
			file_dialog->add_filter("FT2 Instrument",".xi");
			file_dialog->show( FileDialog::MODE_OPEN_MULTI );
			
		} break;
		case MENU_SAVE: {
			file_dialog->clear_filter_list();
			file_dialog->add_filter("IT Instrument",".iti");
			file_dialog->show( FileDialog::MODE_SAVE );
			
		} break;
		case MENU_COPY: {
			
			clipboard=*ins;
			
		} break;
		case MENU_CUT: {
			
			clipboard=*ins;
			ins->reset();
			update_components();

		} break;
		case MENU_PASTE: {
			
			*ins=clipboard;
			update_components();
		} break;

		case MENU_REMAP_TO_MANY: {
		
			tracker->song->separate_in_one_sample_instruments( selected );
			update_components();
		} break;
		case MENU_REMAP_ALL_TO_MANY: {
		
			for (int i=0;i<Song::MAX_INSTRUMENTS;i++) {
			
				tracker->song->separate_in_one_sample_instruments( i );
			}
			update_components();
		} break;
		
	};
	
}

void InstrumentScreen::set_in_window() {
	
	file_dialog = new FileDialog(get_window());

	file_dialog->add_filter("IT Instrument",".iti");
	file_dialog->add_filter("FT2 Instrument",".xi");
	file_dialog->file_activated_signal.connect( this, &InstrumentScreen::instrument_file_callback );
	file_dialog->multi_file_activated_signal.connect( this, &InstrumentScreen::instrument_file_callback_multi );
	
	error_msg = new MessageBox(get_window());
}

void InstrumentScreen::instrument_name_edited(int p_which,String p_name) {
	
	Instrument *ins=tracker->song->get_instrument( p_which );	
	if (!ins)
		return;
	
	ins->set_name( p_name.ascii().get_data() );
}

void InstrumentScreen::set_instrument_path(String p_path) {
	
	if (file_dialog)
		file_dialog->set_dir( p_path );
	
}

InstrumentScreen::InstrumentScreen(Tracker *p_tracker,bool p_mini) {
	
	mini_tabbox = p_mini?add(new TabBox,1):0;

	tracker=p_tracker;
	
	MarginGroup *mg=p_mini?mini_tabbox->add("List",new MarginGroup("Inst. List")):add(new MarginGroup("Instrument List"),3);
	
	mg->get_label_hb()->add( new Widget,1);
	edit = mg->get_label_hb()->add( new MenuBox("File.."));
	edit->add_item( "Clear" );
	edit->add_item( "Clear (With Samples)" );
	edit->add_separator();
	edit->add_item( "Load" );
	edit->add_item( "Save" );
	edit->add_separator();
	edit->add_item( "Copy" );
	edit->add_item( "Cut" );
	edit->add_item( "Paste" );
	edit->add_separator();
	edit->add_item( "Remap (separate samples)" );
	edit->add_item( "Remap All" );
	edit->item_selected_signal.connect( this, &InstrumentScreen::instrument_file_action_callback );
	
	HBoxContainer *slit_hb=mg->add(new HBoxContainer,1);
	
	instrument_list=slit_hb->add( new List, 1 );
	slit_hb->add( new VScrollBar, 0 )->set_range( instrument_list->get_range() );
	
	for (int i=0;i<Song::MAX_INSTRUMENTS;i++)
		instrument_list->add_string( "" );
	
	instrument_list->set_number_elements( 1 );
	instrument_list->set_number_min_integers( 2 );
	instrument_list->set_editable( true );
	instrument_list->set_max_string_length( 25 );
	instrument_list->set_incremental_search(false);	
	/* Instrument Global Tab */
	
	mg=p_mini?mini_tabbox->add("Edit",new MarginGroup("")):add(new MarginGroup("Parameters:"),5);
	
	
	sections=mg->add(new TabBox,1);
		
	HBoxContainer *mi_hb=sections->add( "General", new HBoxContainer );
	
	MarginGroup *mi_mg_table=mi_hb->add( new MarginGroup("Sample Table"),0 );
	
	HBoxContainer *st_hb=mi_mg_table->add( new HBoxContainer,1);
	sample_table=st_hb->add( new SampleInstrumentTable(tracker->editor),1);
	sample_table->set_range( st_hb->add( new VScrollBar, 0)->get_range() );
	
	
	VBoxContainer *mi_nna_vb = mi_hb->add( new VBoxContainer, 1);
	
	MarginGroup *mi_mg_nna=mi_nna_vb->add( new MarginGroup("New Note Action"),1);
	CenterContainer *mi_nna_cc=mi_mg_nna->add( new CenterContainer );
	
	VBoxContainer *mi_nna_vb_buttons = new VBoxContainer;
	
	mi_nna_cc->set_frame(mi_nna_vb_buttons);
	
	nna_group.add( mi_nna_vb_buttons->add( new Button("Note Cut"),0 ) );
	nna_group.add( mi_nna_vb_buttons->add( new Button("Note Continue"),0 ) );
	nna_group.add( mi_nna_vb_buttons->add( new Button("Note Off"),0 ) );
	nna_group.add( mi_nna_vb_buttons->add( new Button("Note Fade"),0 ) );
	nna_group.current_button_changed_signal.connect(this, &InstrumentScreen::parameter_changed_i );
	
	
	HBoxContainer *mi_nna_hb=mi_nna_vb->add( new HBoxContainer,1);
	
	MarginGroup *mi_nna_mg_dct=mi_nna_hb->add( new MarginGroup("Dup. Check"), 1 );
	
	CenterContainer *mi_dct_cc=mi_nna_mg_dct->add( new CenterContainer );
		
	VBoxContainer *mi_dct_vb_buttons = new VBoxContainer;
	
	mi_dct_cc->set_frame(mi_dct_vb_buttons);
	
	dct_group.add( mi_dct_vb_buttons->add( new Button("Disabled"),0 ) );
	dct_group.add( mi_dct_vb_buttons->add( new Button("Note"),0 ) );
	dct_group.add( mi_dct_vb_buttons->add( new Button("Sample"),0 ) );
	dct_group.add( mi_dct_vb_buttons->add( new Button("Instrument"),0 ) );
	dct_group.current_button_changed_signal.connect(this, &InstrumentScreen::parameter_changed_i );
	

	MarginGroup *mi_nna_mg_dca=mi_nna_hb->add( new MarginGroup("Dup. Action"), 1 );
	
	CenterContainer *mi_dca_cc=mi_nna_mg_dca->add( new CenterContainer );
		
	VBoxContainer *mi_dca_vb_buttons = new VBoxContainer;
	
	mi_dca_cc->set_frame(mi_dca_vb_buttons);
	
	dca_group.add( mi_dca_vb_buttons->add( new Button("Note Cut"),0 ) );
	dca_group.add( mi_dca_vb_buttons->add( new Button("Note Off"),0 ) );
	dca_group.add( mi_dca_vb_buttons->add( new Button("Note Fade"),0 ) );
	dca_group.current_button_changed_signal.connect(this, &InstrumentScreen::parameter_changed_i );
	
	/* Volume Section */
	
	VBoxContainer *vol_vb=sections->add( "Volume", new VBoxContainer );
	
	vol_envelope=vol_vb->add( new EnvelopeEditor(p_tracker->editor,p_mini) ,1 );
	
	mg=vol_vb->add( new MarginGroup("Volume Parameters"),0);
	GridContainer * gr = mg->add( new GridContainer(6), 1 );
	
	gr->add( new Label("Global"), false, false );
	
	
	vol_global_send = gr->add( new HSlider, true, false );
	vol_global_send->get_range()->set_max( 128 );
	vol_global_send->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );

	gr->add( new ValueLabel,false,false)->set_range(vol_global_send->get_range());
	
	
	
	gr->add( new Label("Fade"), false, false );
	
	vol_fadeout = gr->add( new HSlider, true, false );
	vol_fadeout->get_range()->set_max( 256 );
	vol_fadeout->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );
	
	gr->add( new ValueLabel,false,false)->set_range(vol_fadeout->get_range());
	
	gr->add( new Label("Random"), false, false );
	
	vol_random_send = gr->add( new HSlider, true, false );
	vol_random_send->get_range()->set_max( 100 );
	vol_random_send->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );
	gr->add( new ValueLabel,false,false)->set_range(vol_random_send->get_range());
	
	/* Pan Section */
	
	VBoxContainer *pan_vb=sections->add( "Pan", new VBoxContainer );
	
	pan_envelope=pan_vb->add( new EnvelopeEditor(p_tracker->editor,p_mini),1 );
	
	
	mg=pan_vb->add( new MarginGroup("Panning Parameters"),0);
	gr = mg->add( new GridContainer(6), 1 );
	
	pan_use_default = gr->add( new CheckButton("Default"), false, false );
	pan_use_default->pressed_signal.connect( this, &InstrumentScreen::parameter_changed );
	pan_default = gr->add( new HSlider, true, false );
	pan_default->get_range()->set_min(-32);	
	pan_default->get_range()->set_max(32);	
	pan_default->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );
	gr->add( new ValueLabel,false,false)->set_range(pan_default->get_range());
	
	gr->add( new Label("PPan Sep"), false, false );
	
	pan_ppsep = gr->add( new HSlider, true, false );

	pan_ppsep->get_range()->set_min(-32);	
	pan_ppsep->get_range()->set_max(32);	
	pan_ppsep->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );
	gr->add( new ValueLabel,false,false)->set_range(pan_ppsep->get_range());
	
	gr->add( new Label("Random"), false, false );
	
	pan_random = gr->add( new HSlider, true, false );
	pan_random->get_range()->set_max(64);	
	pan_random->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );
	gr->add( new ValueLabel,false,false)->set_range(pan_random->get_range());
	
	gr->add( new Label("  Center"), false, false );
	
	pan_ppcenter = gr->add( new HSlider, true, false );
	pan_ppcenter->get_range()->set_max(Note::NOTES-1);	
	pan_ppcenter->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );	
	gr->add( new ValueLabel,false,false)->set_range(pan_ppcenter->get_range());
	
	VBoxContainer *pitch_filter_vb=sections->add( "Pitch/Filter", new VBoxContainer );
	
	pitch_filter_envelope=pitch_filter_vb->add( new EnvelopeEditor(p_tracker->editor,p_mini),1 );
	
	mg=pitch_filter_vb->add( new MarginGroup("Filter"),0);
	gr = mg->add( new GridContainer(6), 1 );
	
	filter_cutoff_default = gr->add( new CheckButton("Cutoff"), false, false );
	filter_cutoff_default->pressed_signal.connect( this, &InstrumentScreen::parameter_changed );
	filter_cutoff=gr->add( new HSlider, true, false );
	filter_cutoff->get_range()->set_max( 127 );
	filter_cutoff->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );	
	gr->add( new ValueLabel,false,false )->set_range( filter_cutoff->get_range() );
	
	filter_reso_default = gr->add( new CheckButton("Resonance"), false, false );
	filter_reso_default->pressed_signal.connect( this, &InstrumentScreen::parameter_changed );
	filter_reso=gr->add( new HSlider, true, false );
	filter_reso->get_range()->set_max( 127 );
	filter_reso->get_range()->value_changed_signal.connect(this,&InstrumentScreen::parameter_changed_p );		
	gr->add( new ValueLabel,false,false )->set_range( filter_reso->get_range() );
	
	env_use_as_filter = pitch_filter_envelope->get_control_box()->add( new CheckButton("Use as Filter"), 1 );
	env_use_as_filter->pressed_signal.connect( this, &InstrumentScreen::parameter_changed );
	
	
	
	
	/* Final */
	
	instrument_list->selected_signal.connect( this, &InstrumentScreen::instrument_selected );
	instrument_list->item_edited_signal.connect( this, &InstrumentScreen::instrument_name_edited );

	/* File Dialog */
	
	
	updating=true;
	selected=0;
	instrument_selected(0);
	
	file_dialog=0;
}

void InstrumentScreen::update_timer_callback() {
	
	static int last_active_voices=0;
	
	int active_voices=tracker->mixer->get_active_voice_count();
	
	if (active_voices==0 && last_active_voices==0)
		return; //no need for updating
	
	last_active_voices=active_voices;
	
	vol_envelope->get_point_editor()->update();
	pan_envelope->get_point_editor()->update();
	pitch_filter_envelope->get_point_editor()->update();
}

InstrumentScreen::~InstrumentScreen()
{
}


