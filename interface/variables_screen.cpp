//
// C++ Implementation: variables_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "widgets/separator.h"
#include "variables_screen.h"


enum ChorusPresets {

	CHORUS_NORMAL,
	CHORUS_ENSEMBLE_SMALL,
	CHORUS_ENSEMBLE_BIG,
	CHORUS_BALLAD,
	CHORUS_ECHOES,
	
	CHORUS_80s,
	CHORUS_ROTARY,
	CHORUS_DRUNK,
	CHORUS_COSMIC,
	
	CHORUS_PRESET_MAX,

};

static double chorus_preset[CHORUS_PRESET_MAX][4]={
	
	{0.5,6.0,6.0,3.0},
	{3.7,10.0,1.0,8.0},
	{3.7,10.0,3.0,8.0},
	{3.2,20.0,2.0,7.0},
	{0.5,20.0,5.0,8.0},
	{0.4,10.0,8.0,8.0},
	
	{11.8,12,1.0,4.0},
	{2.4,11.0,6.0,8.0},
	{11.0,12.0,7.0,10.0} 

};
	
	
		
	

void VariablesScreen::song_name_changed(String p_string) {

	if (updating)
		return;
	tracker->song->set_name( p_string.ascii().get_data() );
	song_name_changed_signal.call();
}

void VariablesScreen::parameter_changed() {
	
	if (updating)
		return;
	

	tracker->song->set_tempo((int) tempo->get_range()->get() );
	tracker->song->set_speed((int) speed->get_range()->get() );
	
	
	tracker->song->set_global_volume((int) global_volume->get_range()->get() );
	tracker->song->set_mixing_volume((int) mixing_volume->get_range()->get() );
	tracker->song->set_stereo_separation((int) stereo_separation->get_range()->get() );

	//tracker->song->set_instruments(  instruments->is_checked );
	
	tracker->song->set_old_effects(  old_effects->is_checked() );
	tracker->song->set_compatible_gxx(  compatible_gxx->is_checked() );
	tracker->song->set_stereo(  stereo->is_checked() );
	tracker->song->set_linear_slides(  linear_slides->is_checked() );
	
	tracker->song->set_chorus_delay_ms( (int)chorus_delay->get_range()->get() );
	tracker->song->set_chorus_speed_hz10( (int)(chorus_rate->get_range()->get()*10.0) );
	tracker->song->set_chorus_depth_ms10( (int)(chorus_depth->get_range()->get()*10.0) );
	tracker->song->set_chorus_separation_ms( (int)chorus_stereo->get_range()->get() );
	
	switch(reverb_mode->get_selected()) {
		
		case Song::REVERB_MODE_ROOM: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_ROOM );
		} break;
		case Song::REVERB_MODE_STUDIO_SMALL: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_STUDIO_SMALL );
			
		} break;
		case Song::REVERB_MODE_STUDIO_MEDIUM: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_STUDIO_MEDIUM );
			
		} break;
		case Song::REVERB_MODE_STUDIO_LARGE: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_STUDIO_LARGE );
			
		} break;
		case Song::REVERB_MODE_HALL: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_HALL );
			
		} break;
		case Song::REVERB_MODE_SPACE_ECHO: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_SPACE_ECHO );
			
		} break;

		case Song::REVERB_MODE_ECHO: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_ECHO );
			
		} break;
		case Song::REVERB_MODE_DELAY: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_DELAY );
			
		} break;
		case Song::REVERB_MODE_HALF_ECHO: {
			
			tracker->song->set_reverb_mode( Song::REVERB_MODE_HALF_ECHO );
			
		} break;
	
	}
	
}

void VariablesScreen::parameter_changed_p(double) {
	
	parameter_changed();
}

void VariablesScreen::parameter_changed_i(int) {
	
	parameter_changed();
}

void VariablesScreen::update_components() {

	updating=true;
	
	reverb_mode->select( tracker->song->get_reverb_mode() );
	song_name->set_text( tracker->song->get_name() );
	message->set_text( tracker->song->get_message() );
	tempo->get_range()->set( (double) tracker->song->get_tempo() );
	speed->get_range()->set( (double) tracker->song->get_speed() );
	
	global_volume->get_range()->set( (double) tracker->song->get_global_volume() );
	mixing_volume->get_range()->set( (double) tracker->song->get_mixing_volume() );
	stereo_separation->get_range()->set( (double) tracker->song->get_stereo_separation() );

	instruments->set_checked( tracker->song->has_instruments() );
	old_effects->set_checked( tracker->song->has_old_effects() );
	compatible_gxx->set_checked( tracker->song->has_compatible_gxx() );
	stereo->set_checked( tracker->song->is_stereo() );
	linear_slides->set_checked( tracker->song->has_linear_slides() );
	
	chorus_delay->get_range()->set( tracker->song->get_chorus_delay_ms() );
	chorus_rate->get_range()->set( (double)tracker->song->get_chorus_speed_hz10()/10.0 );
	chorus_depth->get_range()->set( tracker->song->get_chorus_depth_ms10()/10.0 );
	chorus_stereo->get_range()->set( tracker->song->get_chorus_separation_ms() );
	
	for (int i=0;i<CHORUS_PRESET_MAX;i++ ) {
		
		if (	chorus_rate->get_range()->get()==chorus_preset[i][0] &&
		        chorus_delay->get_range()->get()==chorus_preset[i][1] &&
			chorus_depth->get_range()->get()==chorus_preset[i][2] &&
			chorus_stereo->get_range()->get()==chorus_preset[i][3] ) {
			
			
			chorus_mode->select(i);
			break;
		}
	}
	
	updating=false;
}

void VariablesScreen::toggle_accept() {
	
	//??
	toggle_ins_mode->hide();
	tracker->player->play_stop();
	tracker->song->set_instruments( instruments->is_checked() );
	if (instruments->is_checked()) {
		tracker->song->make_instruments_from_samples();
	}
		
	instrument_mode_changed.call();
}

void VariablesScreen::toggle_cancel() {
	
	updating=true;
	instruments->set_checked( !instruments->is_checked() );
	updating=false;
}

void VariablesScreen::toggle_changed() {
	
	if (updating)
		return;
	
	toggle_label->set_text( instruments->is_checked()?"Go to Instrument Mode?":"Go to Sample Mode?");
	toggle_ins_mode->show();
	
}

void VariablesScreen::set_in_window() {
	
	toggle_ins_mode = new Window(get_window(),Window::MODE_POPUP,Window::SIZE_CENTER);
	
	toggle_ins_mode->popup_cancel_signal.connect(this,&VariablesScreen::toggle_cancel);
	
	VBoxContainer *win_vb = new VBoxContainer;
	toggle_ins_mode->set_root_frame( win_vb );
	
	
	toggle_label= win_vb->add(new Label,0);
	
	win_vb->add(new CenterContainer,0)->set( new Button("Ok"))->pressed_signal.connect(this,&VariablesScreen::toggle_accept);
	
	chorus_window = new Window(get_window(),Window::MODE_POPUP,Window::SIZE_CENTER);
	win_vb = new VBoxContainer;
	chorus_window->set_root_frame( win_vb );
	
	win_vb->add(new MarginGroup("Chorus Rate (hz):"))->add(chorus_rate);
	win_vb->add(new MarginGroup("Chorus Delay (ms):"))->add(chorus_delay);
	win_vb->add(new MarginGroup("Chorus Depth (ms):"))->add(chorus_depth);
	win_vb->add(new MarginGroup("Chorus Stereo (ms):"))->add(chorus_stereo);;

	
	
}

void VariablesScreen::chorus_preset_select(int p_which) {
	
	if (updating)
		return;
	if (p_which<0 || p_which>=CHORUS_PRESET_MAX)
		return;
	
	chorus_rate->get_range()->set( chorus_preset[p_which][0] );
	chorus_delay->get_range()->set( chorus_preset[p_which][1] );
	chorus_depth->get_range()->set( chorus_preset[p_which][2] );
	chorus_stereo->get_range()->set( chorus_preset[p_which][3] );
	
}

void VariablesScreen::message_changed() {

	tracker->song->set_message( message->get_text().ascii().get_data() );
}

void VariablesScreen::chorus_set() {
	
	chorus_window->show();
}

VariablesScreen::VariablesScreen(Tracker *p_tracker,bool p_mini) {
	
	tracker=p_tracker;
	
	
	
	if (p_mini) 
		mini_tabbox = add( new TabBox,2 );
	else
		var_vb=add(new VBoxContainer,2);
	
	VBoxContainer *main_vb=add(new VBoxContainer,5);
	
	VBoxContainer *init_vb=p_mini?mini_tabbox->add("Initial",new VBoxContainer):var_vb->add(new VBoxContainer,0);
	
	song_name=main_vb->add(new MarginGroup("Song Name"),0)->add(new LineEdit,0);
	song_name->set_max_length( Song::MAX_SONG_NAME-1 );
	song_name->text_changed_signal.connect(this,&VariablesScreen::song_name_changed);
	HBoxContainer *msg_hb =main_vb->add(new MarginGroup("Message"),1)->add(new HBoxContainer,1);
	
	message=msg_hb->add(new TextEdit,1);
	message->text_changed_signal.connect(this,&VariablesScreen::message_changed);
	VScrollBar *vsb=msg_hb->add(new VScrollBar,0);
	vsb->set_range(message->get_range() );
	vsb->set_auto_hide( true );
	vsb->hide();
	MarginGroup *mg=init_vb->add( new MarginGroup("Speed/Tempo"), 0);
	
	GridContainer *gc=mg->add(new GridContainer(2),0);
	
	gc->add(new Label("Tempo"),false,false);
	
	tempo=gc->add(new SpinBox,true,false);
	tempo->get_range()->set_min(31);
	tempo->get_range()->set_max(255);
	tempo->get_range()->value_changed_signal.connect( this, &VariablesScreen::parameter_changed_p );
		
	gc->add(new Label("Speed"),false,false);
	
	speed=gc->add(new SpinBox,true,false);
	
	speed->get_range()->set_min(1);
	speed->get_range()->set_max(255);
	speed->get_range()->value_changed_signal.connect( this, &VariablesScreen::parameter_changed_p );
	
	mg=init_vb->add( new MarginGroup("Volume"), 0);
	
	gc=mg->add(new GridContainer(2),0);
	
	gc->add(new Label("Global"),false,false);
	global_volume=gc->add(new SpinBox,true,false);
	global_volume->get_range()->set_max(128);
	global_volume->get_range()->value_changed_signal.connect( this, &VariablesScreen::parameter_changed_p );
	
	gc->add(new Label("Mixing"),false,false);
	mixing_volume=gc->add(new SpinBox,true,false);
	mixing_volume->get_range()->set_max(128);
	mixing_volume->get_range()->value_changed_signal.connect( this, &VariablesScreen::parameter_changed_p );
	
	gc->add(new Label("Stereo"),false,false);
	stereo_separation=gc->add(new SpinBox,true,false);
	stereo_separation->get_range()->set_max(128);
	stereo_separation->get_range()->value_changed_signal.connect( this, &VariablesScreen::parameter_changed_p );
	
	VBoxContainer *sound_vb=p_mini?mini_tabbox->add("FX",new VBoxContainer):var_vb->add(new VBoxContainer,0);
	
	mg=sound_vb->add( new MarginGroup("Compatibility"), 0);
	
	gc=mg->add(new GridContainer(1),0);
	
	instruments=gc->add(new CheckButton("Instrument Mode"),false,false);
	instruments->pressed_signal.connect(this,&VariablesScreen::toggle_changed);
	gc->add(new HSeparator,false,false);
	
	old_effects=gc->add(new CheckButton("Old Effects"),false,false);
	old_effects->pressed_signal.connect( this, &VariablesScreen::parameter_changed );
	
	compatible_gxx=gc->add(new CheckButton("Compatible GXX"),false,false);
	compatible_gxx->pressed_signal.connect( this, &VariablesScreen::parameter_changed );
	
	stereo=gc->add(new CheckButton("Stereo"),false,false);
	stereo->pressed_signal.connect( this, &VariablesScreen::parameter_changed );
	
	linear_slides=gc->add(new CheckButton("Linear Slides"),false,false);
	linear_slides->pressed_signal.connect( this, &VariablesScreen::parameter_changed );


	reverb_mode=sound_vb->add(new MarginGroup("Reverb Preset"),0)->add(new ComboBox,0);
	reverb_mode->add_string("Room");
	reverb_mode->add_string("Studio Small");
	reverb_mode->add_string("Studio Medium");
	reverb_mode->add_string("Studio Large");
	reverb_mode->add_string("Hall");
	reverb_mode->add_string("Space Echo");
	reverb_mode->add_string("Echo");
	reverb_mode->add_string("Delay");
	reverb_mode->add_string("Half Echo");
	reverb_mode->selected_signal.connect(this,&VariablesScreen::parameter_changed_i );
		
	HBoxContainer *chorus_hb =sound_vb->add(new MarginGroup("Chorus Preset"),0)->add(new HBoxContainer);
	chorus_mode=chorus_hb->add(new ComboBox,1);
	chorus_hb->add(new Button("set"))->pressed_signal.connect(this,&VariablesScreen::chorus_set);
	
	chorus_rate=new SpinBox;
	chorus_rate->get_range()->config(0.1,30, 1,0.1);
	chorus_rate->get_range()->value_changed_signal.connect(this,&VariablesScreen::parameter_changed_p );
	chorus_delay=new SpinBox;
	chorus_delay->get_range()->config(1,20,6,1);
	chorus_delay->get_range()->value_changed_signal.connect(this,&VariablesScreen::parameter_changed_p );
	chorus_depth=new SpinBox;
	chorus_depth->get_range()->config(0.1,20,3,0.1);
	chorus_depth->get_range()->value_changed_signal.connect(this,&VariablesScreen::parameter_changed_p );
	chorus_stereo=new SpinBox;
	chorus_stereo->get_range()->config(0,10,3,1);
	chorus_stereo->get_range()->value_changed_signal.connect(this,&VariablesScreen::parameter_changed_p );
	
	
	chorus_mode->add_string("Normal");
	chorus_mode->add_string("Ensemble");
	chorus_mode->add_string("Great Ensemble");
	chorus_mode->add_string("Ballad");
	chorus_mode->add_string("Echoes");
	chorus_mode->add_string("Eighties");
	chorus_mode->add_string("Rotary");
	chorus_mode->add_string("Drunk");
	chorus_mode->add_string("Cosmic");
	
	chorus_mode->selected_signal.connect( this, &VariablesScreen::chorus_preset_select );
	
	toggle_ins_mode=0;
	toggle_label=0;
	
	update_components();
}


VariablesScreen::~VariablesScreen()
{
}



