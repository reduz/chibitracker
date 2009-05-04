//
// C++ Implementation: mixer_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bundles/window_box.h"
#include "mixer_dialog.h"

void MixerDialog::update_vars() {

	updating=true;
	
	interpolation->select( sw_mixer->get_mix_interpolation() );
	filters->set_checked( sw_mixer->use_filters() );
	volume_ramp->set_checked( sw_mixer->use_volume_ramp() );
	oversampling->set_checked( sw_mixer->use_oversampling() );
	declicker->set_checked( sw_mixer->use_declicker() );
	clipping->set_checked( sw_mixer->use_clipping() );
	effects->set_checked( sw_mixer->use_effects() );
	globalvol->get_range()->set( sw_mixer->get_global_volume() );
	
	updating=false;

}

void MixerDialog::update_params() {
	
	if (!sw_mixer)
		return;
	
	if (updating)
		return;
	
	switch (interpolation->get_selected()) {
		
		case INTERPOLATION_RAW: sw_mixer->set_mix_interpolation( INTERPOLATION_RAW ) ; break;
		case INTERPOLATION_LINEAR: sw_mixer->set_mix_interpolation( INTERPOLATION_LINEAR ) ; break;
		case INTERPOLATION_CUBIC: sw_mixer->set_mix_interpolation( INTERPOLATION_CUBIC ) ; break;
	};
		
	sw_mixer->set_use_filters( filters->is_checked() );
	sw_mixer->set_use_declicker( declicker->is_checked() );
	sw_mixer->set_use_clipping( clipping->is_checked() );
	sw_mixer->set_use_volume_ramp( volume_ramp->is_checked() );
	sw_mixer->set_use_oversampling( oversampling->is_checked() );
	sw_mixer->set_use_effects( effects->is_checked() );
	sw_mixer->set_global_volume( (int)globalvol->get_range()->get() );
	
}
void MixerDialog::update_params_i(int) {
	
	update_params();
}

void MixerDialog::update_params_p(double) {
	
	update_params();
}
	
void MixerDialog::set_mixer(Mixer_Soft *p_sw_mixer) {

	sw_mixer=p_sw_mixer;
	
	update_vars();
}

void MixerDialog::read_slot(String p_section,String p_entry,String p_value) {
	
	if (p_section!="SoftwareMixer")
		return;
	
	
	if (p_entry=="interpolation") {
		
		if (p_value=="none")
			sw_mixer->set_mix_interpolation( INTERPOLATION_RAW );
		else if (p_value=="linear")
			sw_mixer->set_mix_interpolation( INTERPOLATION_LINEAR );
		else if (p_value=="cubic")
			sw_mixer->set_mix_interpolation( INTERPOLATION_CUBIC );
		
		
		
	} else if (p_entry=="filters") {
		
		sw_mixer->set_use_filters( p_value=="true" );
	} else if (p_entry=="volume_ramp") {
		
		sw_mixer->set_use_volume_ramp( p_value=="true" );
	}  else if (p_entry=="oversampling") {
		
		sw_mixer->set_use_oversampling( p_value=="true" );
	} else if (p_entry=="effects") {
		
		sw_mixer->set_use_effects( p_value=="true" );
	} else if (p_entry=="declicker") {
		
		sw_mixer->set_use_declicker( p_value=="true" );
	} else if (p_entry=="clipping") {
		
		sw_mixer->set_use_clipping( p_value=="true" );
	} else if (p_entry=="global_volume") {
		
		sw_mixer->set_global_volume(p_value.to_int());
	}
	
}

void MixerDialog::read_finished() {
	
	update_vars();
}


void MixerDialog::save_slot() {
	
	
	config->set_section( "SoftwareMixer" );
	
	switch (sw_mixer->get_mix_interpolation()) {
		
		case INTERPOLATION_RAW: config->add_entry("interpolation","none","none,linear,cubic"); break;
		case INTERPOLATION_LINEAR: config->add_entry("interpolation","linear","none,linear,cubic"); break;
		case INTERPOLATION_CUBIC: config->add_entry("interpolation","cubic","none,linear,cubic"); break;
	};
		
	config->add_entry( "filters",sw_mixer->use_filters()?"true":"false");
	config->add_entry( "volume_ramp",sw_mixer->use_volume_ramp()?"true":"false");
	config->add_entry( "oversampling",sw_mixer->use_oversampling()?"true":"false");
	config->add_entry( "declicker",sw_mixer->use_declicker()?"true":"false");
	config->add_entry( "clipping",sw_mixer->use_clipping()?"true":"false");
	config->add_entry( "effects",sw_mixer->use_effects()?"true":"false");
	config->add_entry( "global_volume",String::num( sw_mixer->get_global_volume() ));
		
}
MixerDialog::MixerDialog(Window *p_parent,ConfigApi *p_config) : Window(p_parent,Window::MODE_POPUP,Window::SIZE_CENTER) {

	config = p_config;
	WindowBox *vb = new WindowBox("Software Mixer");		
	set_root_frame( vb );
	
	MarginGroup *mg = vb->add( new MarginGroup("Interpolation:"));
	interpolation=mg->add(new ComboBox);
	interpolation->add_string( "None" );
	interpolation->add_string( "Linear" );
	interpolation->add_string( "Cubic" );
	interpolation->selected_signal.connect(this,&MixerDialog::update_params_i );

	
	HBoxContainer *gv_hb = vb->add(new MarginGroup("Total Volume"))->add(new HBoxContainer);
	globalvol=gv_hb->add(new HSlider,1);
	globalvol->get_range()->set_max( 512 );
	globalvol->get_range()->set( 192 );
	gv_hb->add(new ValueLabel)->set_range(globalvol->get_range());
	globalvol->get_range()->value_changed_signal.connect(this,&MixerDialog::update_params_p);
	
	filters=vb->add(new CheckButton("Filters"));
	filters->pressed_signal.connect(this,&MixerDialog::update_params );
	volume_ramp=vb->add(new CheckButton("Volume Ramp"));
	volume_ramp->pressed_signal.connect(this,&MixerDialog::update_params );
	
	declicker=vb->add(new CheckButton("Declicker"));
	declicker->pressed_signal.connect(this,&MixerDialog::update_params );
	
	clipping=vb->add(new CheckButton("Clipping"));
	clipping->pressed_signal.connect(this,&MixerDialog::update_params );
	
	effects=vb->add(new CheckButton("Effects (Chorus/Reverb)"));
	effects->pressed_signal.connect(this,&MixerDialog::update_params );
	
	oversampling=vb->add(new CheckButton("OverSampling"));
	oversampling->pressed_signal.connect(this,&MixerDialog::update_params );

	sw_mixer=0;
	updating=false;
	
	config->write_entry_signal.connect( this, &MixerDialog::save_slot );	
	config->read_entry_signal.connect( this, &MixerDialog::read_slot );
	config->read_finished_signal.connect( this, &MixerDialog::read_finished );
	
}


MixerDialog::~MixerDialog() {
}


