//
// C++ Implementation: paths_dialog
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
#include "bundles/margin_group.h"
#include "paths_dialog.h"


void PathsDialog::paths_text_changed(String) {

	if (updating)
		return;
	paths->song_path=song_path->get_text();
	paths->sample_path=sample_path->get_text();
	paths->instrument_path=instrument_path->get_text();
	
	paths_changed_signal.call();
}

void PathsDialog::dir_selected(String p_dir) {

	switch(setting_for) {
		
		case SONG: {
			
			paths->song_path=p_dir;
			song_path->set_text( p_dir );
			
		} break;
		case SAMPLE: {
			
			paths->sample_path=p_dir;
			sample_path->set_text( p_dir );
			
		} break;
		case INSTRUMENT: {
			
			paths->instrument_path=p_dir;
			instrument_path->set_text( p_dir );
			
		} break;
		
	}
	
	paths_changed_signal.call();
}

void PathsDialog::browse_song_path() {

	setting_for=SONG;
	browser->set_dir( paths->song_path );
	browser->show( FileDialog::MODE_OPEN_DIR );

}
void PathsDialog::browse_sample_path() {

	setting_for=SAMPLE;
	browser->set_dir( paths->sample_path );
	browser->show( FileDialog::MODE_OPEN_DIR );
	

}
void PathsDialog::browse_instrument_path() {

	setting_for=INSTRUMENT;
	browser->set_dir( paths->instrument_path );
	browser->show( FileDialog::MODE_OPEN_DIR );
	
}

void PathsDialog::read_slot(String p_section,String p_entry,String p_value) {
	
	if (p_section!="Default Paths")
		return;
	
	if (p_entry=="song_path")
		paths->song_path=p_value;
	
	if (p_entry=="sample_path")
		paths->sample_path=p_value;
	
	if (p_entry=="instrument_path")
		paths->instrument_path=p_value;
	
	
}
void PathsDialog::read_finished() {
	
	paths_changed_signal.call();
	update_paths();
}
void PathsDialog::save_slot() {
	
	config->set_section( "Default Paths" );
	config->add_entry( "song_path" , paths->song_path );
	config->add_entry( "sample_path" , paths->sample_path );
	config->add_entry( "instrument_path" , paths->instrument_path );
	
}

void PathsDialog::update_paths() {
	
	updating=true;
	sample_path->set_text( paths->sample_path );
	instrument_path->set_text( paths->instrument_path );
	song_path->set_text( paths->song_path );
	
	updating=false;
}

PathsDialog::PathsDialog(Window *p_parent,DefaultPaths *p_paths,ConfigApi *p_config) : Window(p_parent,MODE_POPUP,SIZE_TOPLEVEL_CENTER) {

	
	paths=p_paths;
	

	config=p_config;
	WindowBox *vb = new WindowBox("Default Paths");
	set_root_frame( vb );
	
	MarginGroup *mg = vb->add( new MarginGroup("Song Path:"));

	HBoxContainer *hbc = mg->add(new HBoxContainer);
	
	song_path=hbc->add(new LineEdit,1);
	song_path->text_changed_signal.connect(this,&PathsDialog::paths_text_changed);
	hbc->add(new Button("Browse"))->pressed_signal.connect(this,&PathsDialog::browse_song_path);
	
	mg = vb->add( new MarginGroup("Sample Path:"));

	hbc = mg->add(new HBoxContainer);
	
	sample_path=hbc->add(new LineEdit,1);
	sample_path->text_changed_signal.connect(this,&PathsDialog::paths_text_changed);
	hbc->add(new Button("Browse"))->pressed_signal.connect(this,&PathsDialog::browse_sample_path);
	
	mg = vb->add( new MarginGroup("Instrument Path:"));

	hbc = mg->add(new HBoxContainer);
	
	instrument_path=hbc->add(new LineEdit,1);
	instrument_path->text_changed_signal.connect(this,&PathsDialog::paths_text_changed);
	
	hbc->add(new Button("Browse"))->pressed_signal.connect(this,&PathsDialog::browse_instrument_path);
	
	browser = new FileDialog(get_root());
	browser->dir_selected_signal.connect(this,&PathsDialog::dir_selected );
	
	config->write_entry_signal.connect( this, &PathsDialog::save_slot );
	config->read_entry_signal.connect( this, &PathsDialog::read_slot );
	config->read_finished_signal.connect( this, &PathsDialog::read_finished );
	
	updating=false;
	
	update_paths();
}


PathsDialog::~PathsDialog()
{
}


