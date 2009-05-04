//
// C++ Implementation: sound_driver_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bundles/window_box.h"
#include "bundles/margin_group.h"
#include "widgets/separator.h"
#include "sound_driver_dialog.h"
#include "mixer/sound_driver_manager.h"


void SoundDriverDialog::parameter_changed() {

	if (updating || updating_parameters)
		return;
	
	
	
	AudioParams &ap=SoundDriverManager::get_audio_params();
	ap.set_mix_freq( (int)mix_freq->get_line_edit()->get_text().to_double() );
	ap.set_buffer_size( (int)buffsize->get_line_edit()->get_text().to_double() );
	ap.set_stereo( stereo->is_checked() );
	
	switch( bits->get_selected() ) {
		
		case AudioParams::BITS_8: ap.set_bits( AudioParams::BITS_8 ); break;
		case AudioParams::BITS_16: ap.set_bits( AudioParams::BITS_16 ); break;
		case AudioParams::BITS_32: ap.set_bits( AudioParams::BITS_32 ); break;
		
	};
	
	if (SoundDriverManager::is_driver_active())
		SoundDriverManager::init_driver(); //restart current driver if active
	
	need_param_update=true; //a delay may be needed for this
//	update_parameters();
	update_status();
	
}


void SoundDriverDialog::check_driver_status() {
	
	if (need_param_update) {
		update_status();
		update_parameters();
		need_param_update=false;
	}
}

void SoundDriverDialog::parameter_changed_i(int p_idx) {
	
	parameter_changed();
	
}


void SoundDriverDialog::sound_driver_changed(int p_to_which) {

	if (updating || updating_parameters)
		return;
	
	SoundDriverManager::init_driver(p_to_which);
	update_parameters();
	update_status();
	
}
void SoundDriverDialog::restart() {
	
	SoundDriverManager::init_driver();
	update_parameters();
	update_status();
}
void SoundDriverDialog::disable() {
	
	SoundDriverManager::finish_driver();
	update_parameters();
	update_status();
}


void SoundDriverDialog::update_parameters() {

	updating_parameters=true;
	
	mix_freq->get_line_edit()->set_text( String::num(SoundDriverManager::get_audio_params().get_mix_freq()) );
	buffsize->get_line_edit()->set_text( String::num(SoundDriverManager::get_audio_params().get_buffer_size()) );
	stereo->set_checked( SoundDriverManager::get_audio_params().is_stereo() );
	switch( SoundDriverManager::get_audio_params().get_bits() ) {
		
		case AudioParams::BITS_8: bits->select( 0 ); break;
		case AudioParams::BITS_16: bits->select( 1 ); break;
		case AudioParams::BITS_32: bits->select( 2 ); break;
	};

	updating_parameters=false;
	
}

void SoundDriverDialog::update_status() {
	
	status->set_text( SoundDriverManager::is_driver_active()? "Active":"Disabled" );
	
}

void SoundDriverDialog::show() {

	updating=true;
	
	sound_driver->clear();
	
	for (int i=0;i<SoundDriverManager::get_driver_count();i++) {
		
		sound_driver->add_string( SoundDriverManager::get_driver(i)->get_name() );
	}
	
	sound_driver->select( SoundDriverManager::get_current_driver_index() );
	
	update_parameters();
	update_status();

	Window::show();
	
	updating=false;
}


void SoundDriverDialog::read_slot(String p_section,String p_entry,String p_value) {
	
	if (p_section!="SoundDriver")
		return; //not for this section
	
	AudioParams &ap=SoundDriverManager::get_audio_params();
	
	if (p_entry=="driver") {
		
		for (int i=0;i<SoundDriverManager::get_driver_count();i++) {
			
			if (p_value==SoundDriverManager::get_driver(i)->get_name()) {
				
				config_driver=i;
				break;
			}
		}
		
	} else if (p_entry=="enabled") {
		
		config_must_enable_driver=(p_value=="true");
	} else if (p_entry=="mix_freq") {
		
		ap.set_mix_freq( (int)p_value.to_double() );
		
	} else if (p_entry=="buffer_size") {
		
		ap.set_buffer_size( (int)p_value.to_double() );
		
	} else if (p_entry=="stereo") {
		
		ap.set_stereo( p_value=="true" );
		
	} else if (p_entry=="bits") {
		
		if (p_value=="8") {
			
			ap.set_bits( AudioParams::BITS_8 );
		} else if (p_value=="16") {
			
			ap.set_bits( AudioParams::BITS_16 );
		} else if (p_value=="32") {
			
			ap.set_bits( AudioParams::BITS_32 );
		}
	}
		
}

void SoundDriverDialog::read_finished() {
	
	if (config_driver>=0 && config_must_enable_driver) {
		
		SoundDriverManager::init_driver(config_driver);
	}	
	
	update_parameters();
}

void SoundDriverDialog::save_slot() {
	
	AudioParams &ap=SoundDriverManager::get_audio_params();
	
	config->set_section( "SoundDriver" );
	config->add_entry( "driver",SoundDriverManager::get_driver()->get_name(),"Identifier" );
	config->add_entry( "enabled",SoundDriverManager::is_driver_active()?"true":"false" );
	config->add_entry( "mix_freq",String::num(ap.get_mix_freq()),"hz" );
	config->add_entry( "bits",(ap.get_bits()==AudioParams::BITS_8)?"8":((ap.get_bits()==AudioParams::BITS_16)?"16":"32"),"8,16,32");
	config->add_entry( "buffer_size",String::num(ap.get_buffer_size()),"In frames, power of 2 is best" );
	config->add_entry( "stereo",ap.is_stereo()?"true":"false" );

		
}

SoundDriverDialog::SoundDriverDialog(Window *p_parent,ConfigApi *p_config) : Window(p_parent,MODE_POPUP,SIZE_CENTER) {
	
	config=p_config;
	WindowBox *vb = new WindowBox("Sound Driver");		
	
	set_root_frame( vb );
	
	MarginGroup *mg = vb->add( new MarginGroup("Selected Driver:"));
	sound_driver=mg->add(new ComboBox);
	sound_driver->selected_signal.connect( this, &SoundDriverDialog::sound_driver_changed );
	
	HBoxContainer *hbc = mg->add( new HBoxContainer );
	hbc->add( new Button("Disable"),1)->pressed_signal.connect(this,&SoundDriverDialog::disable);
	hbc->add( new Button("Reset"),1)->pressed_signal.connect(this,&SoundDriverDialog::restart);
	vb->add(new HSeparator);
	mg = vb->add( new MarginGroup("Mixing Frequency:"));
	mix_freq=mg->add(new ComboBox);
	mix_freq->add_string("4096");
	mix_freq->add_string("8192");
	mix_freq->add_string("11025");
	mix_freq->add_string("22050");
	mix_freq->add_string("44100");
	mix_freq->add_string("48000");
	mix_freq->add_string("96000");
	mix_freq->add_string("192000");
	mix_freq->selected_signal.connect( this, &SoundDriverDialog::parameter_changed_i );

	mg = vb->add( new MarginGroup("Format:"));
	bits=mg->add(new ComboBox);
	bits->add_string( "8 Bits" );
	bits->add_string( "16 Bits" );
	bits->add_string( "24/32 Bits" );
	bits->selected_signal.connect( this, &SoundDriverDialog::parameter_changed_i );
	
	stereo=mg->add(new CheckButton("Stereo"));
	stereo->pressed_signal.connect(this, &SoundDriverDialog::parameter_changed );
	
	mg = vb->add( new MarginGroup("Buffer Size (frames):"));
	buffsize=mg->add(new ComboBox);
	buffsize->add_string("128");
	buffsize->add_string("256");
	buffsize->add_string("512");
	buffsize->add_string("1024");
	buffsize->add_string("2048");
	buffsize->add_string("4096");
	buffsize->add_string("8192");
	buffsize->add_string("16384");
	buffsize->add_string("32768");
	buffsize->selected_signal.connect( this, &SoundDriverDialog::parameter_changed_i );
	mg = vb->add( new MarginGroup("Status:"));
	status=mg->add(new LineEdit("Active"));
	status->set_editable( false );
	
	updating=false;
	updating_parameters=false;
	
	config->write_entry_signal.connect( this, &SoundDriverDialog::save_slot );
	config->read_entry_signal.connect( this, &SoundDriverDialog::read_slot );
	config->read_finished_signal.connect( this, &SoundDriverDialog::read_finished );
	
	config_must_enable_driver=false;
	config_driver=-1;
}


SoundDriverDialog::~SoundDriverDialog()
{
}


