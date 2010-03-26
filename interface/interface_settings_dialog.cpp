//
// C++ Implementation: interface_settings_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "interface_settings_dialog.h"
#include "bundles/margin_group.h"
#include "widgets/separator.h"
#include "tracker/key_bindings.h"
#include "containers/center_container.h"
#include "bundles/window_box.h"

void InterfaceSettingsDialog::fullscreen_mode_changed(int) {

	if (fullscreen->is_checked())
		fullscreen_toggle(true);
}

void InterfaceSettingsDialog::fullscreen_toggle(bool p_fsc) {

	if (!p_fsc) {
		
		
		get_painter()->set_fullscreen( false );
	} else {

		get_painter()->set_fullscreen( true, fullscreen_list->get_selected() );
	}
	
	
	get_root()->set_size( get_painter()->get_display_size() );
	get_root()->redraw_all();
	get_painter()->update_screen();
	
}

void InterfaceSettingsDialog::key_handler(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	if (!p_press)
		return;
	
	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return;
	
	
	switch( kbind ) {
	
		case KB_DISPLAY_TOGGLE_FULLSCREEN: {
			
			fullscreen->set_pressed( !fullscreen->is_pressed() );
			fullscreen_toggle( fullscreen->is_pressed() );
			
		} break;
		default: {}
	}
}


int InterfaceSettingsDialog::find_closest_fs_mode(Size p_size) {
	
	/* not best method, just somewhat works */
	int closest_idx=-1;
	int closest_d=0;
	
	for (int i=0;i<get_painter()->get_fullscreen_mode_count();i++) {
		
		Size fsmode=get_painter()->get_fullscreen_mode(i);
		
		int h=(p_size.height-fsmode.height);
		h*=h;
		
		int w=(p_size.width-fsmode.width);
		w*=w;		
		int d=h+w;
		
		if (d<closest_d || i==0) {
			
			closest_idx=i;
			closest_d=d;
		}
		
	}
	
	return closest_idx;
	
}

void InterfaceSettingsDialog::read_slot(String p_section,String p_entry,String p_value) {
	
	if (p_entry=="fullscreen_width")
		fsconfig.width=(int)p_value.to_double();
	else if (p_entry=="fullscreen_height")
		fsconfig.height=(int)p_value.to_double();
	else if (p_entry=="fullscreen")
		fullscreen->set_checked( p_value=="true" );
	else if (p_entry=="theme") {
		
		load_theme(p_value);
		for (int i=0;i<theme_list->get_size();i++) {
			
			if (theme_list->get_string(i)==p_value)
				theme_list->select(i);
		}
	} 
	
	
}
void InterfaceSettingsDialog::read_finished() {
	
	fullscreen_list->select( find_closest_fs_mode( fsconfig ) );
	if (fullscreen->is_checked() && !get_painter()->is_fullscreen())
		fullscreen_toggle(true);
}
void InterfaceSettingsDialog::save_slot() {
	
	Size fsmode=get_painter()->get_fullscreen_mode( fullscreen_list->get_selected() );

	config->set_section( "Interface" );
	config->add_entry( "fullscreen_width" , String::num( fsmode.width ) );
	config->add_entry( "fullscreen_height" , String::num( fsmode.height ) );
	config->add_entry( "fullscreen" , fullscreen->is_checked()?"true":"false" );
	if (last_theme_loaded!="")
		config->add_entry( "theme" , last_theme_loaded );
}


void InterfaceSettingsDialog::set_theme_path(String p_path) {
	
	
	FileSystem *fs = FileSystem::instance_func();
	if (fs->change_dir( p_path ))
		return;
	fs->list_dir_begin();
	
	while(true) {
		
		bool d;
		String f=fs->get_next(&d);
		if (f=="")
			break;
		if (!d)
			continue; 
		if (f=="." || f=="..")
			continue;
		//dont add a theme in multiple paths more than once
		if (theme_list->find_string_index( f )>=0)
			continue;
		theme_list->add_sorted_string( f );
	}
	
	fs->list_dir_end();
	
	theme_path=p_path;
	
	delete fs;
}


void InterfaceSettingsDialog::set_system_theme_path(String p_path) {
	
	
	FileSystem *fs = FileSystem::instance_func();
	if (fs->change_dir( p_path ))
		return; //no luck
	fs->list_dir_begin();
	
	while(true) {
		
		bool d;
		String f=fs->get_next(&d);
		if (f=="")
			break;
		if (!d)
			continue; 
		if (f=="." || f=="..")
			continue;
		
		if (theme_list->find_string_index( f )>=0)
			continue;
		theme_list->add_sorted_string( f );
	}
	
	fs->list_dir_end();
	delete fs;
	system_theme_path=p_path;
}


void InterfaceSettingsDialog::load_theme(String p_theme) {
	
	FileSystem *fs = FileSystem::instance_func();
	
	String path_to_use;
	
	if (fs->file_exists( theme_path + "/" + p_theme +"/skin.cfg" )) {
		/* regular theme path has priority */
		path_to_use=theme_path;
	} else if (fs->file_exists( system_theme_path + "/" + p_theme +"/skin.cfg" )) {
		/* If not, check system theme path */
		path_to_use=system_theme_path;
		
	} else {
		
		delete fs;
		return; //theme not found
	}
	
	delete fs;
	theme_loader->set_theme_path( path_to_use );
	
	last_theme_loaded=p_theme;
	
	theme_loader->open( p_theme );
	
	get_root()->skin_changed();	
	get_root()->set_size( get_painter()->get_display_size() );
	get_root()->redraw_all();
	get_painter()->update_screen();
	
}

void InterfaceSettingsDialog::load_theme_slot() {
	
	String theme=theme_list->get_string( theme_list->get_selected() );
	
	load_theme(theme);
	
}

void InterfaceSettingsDialog::repaint_interval_changed(double p_to) {
	
	repaint_interval_changed_signal.call((int)p_to);
}

InterfaceSettingsDialog::InterfaceSettingsDialog(Window *p_parent,ConfigApi *p_config): Window(p_parent,MODE_POPUP,SIZE_CENTER) {
	
	file = FileAccessWrapper::create();
	config_theme = new ConfigFile( file );
	theme_loader = new ThemeLoader( config_theme, get_root() );

	config=p_config;
	WindowBox *vb = new WindowBox("UI Settings");
	set_root_frame( vb );
	
	MarginGroup *mg = vb->add( new MarginGroup("FullScreen Mode:"));
	
	fullscreen_list=mg->add( new ComboBox);
	
	for (int i=0;i<get_painter()->get_fullscreen_mode_count();i++) {
		
		Size mode=get_painter()->get_fullscreen_mode(i);
		fullscreen_list->add_string( String::num(mode.width)+"x"+String::num(mode.height) );
	}
	
	fullscreen_list->selected_signal.connect(this, &InterfaceSettingsDialog::fullscreen_mode_changed);
	
	fullscreen=mg->add(new CheckButton("Enabled"));
	fullscreen->set_checked( get_painter()->is_fullscreen() );
	fullscreen->toggled_signal.connect( this, &InterfaceSettingsDialog::fullscreen_toggle );
	
	vb->add( new HSeparator );
	
	mg = vb->add( new MarginGroup("Indicators Repaint Interval (ms):"));
	
	repaint_interval=mg->add(new SpinBox);
	repaint_interval->get_range()->set_min( 8 );
	repaint_interval->get_range()->set( 60 );
	repaint_interval->get_range()->set_max( 500 );
	repaint_interval->get_range()->value_changed_signal.connect(this,&InterfaceSettingsDialog::repaint_interval_changed);
	
	fullscreen->set_checked( get_painter()->is_fullscreen() );
	fullscreen->toggled_signal.connect( this, &InterfaceSettingsDialog::fullscreen_toggle );
	
	
	vb->add( new HSeparator );
	
	mg=vb->add( new MarginGroup("Theme"));
	
	theme_list=mg->add( new ComboBox);
	
	mg->add( new CenterContainer)->set( new Button("Load") )->pressed_signal.connect(this,&InterfaceSettingsDialog::load_theme_slot);
	
	get_root()->key_unhandled_signal.connect(this,&InterfaceSettingsDialog::key_handler);
	
	config->write_entry_signal.connect( this, &InterfaceSettingsDialog::save_slot );
	config->read_entry_signal.connect( this, &InterfaceSettingsDialog::read_slot );
	config->read_finished_signal.connect( this, &InterfaceSettingsDialog::read_finished );
	
}


InterfaceSettingsDialog::~InterfaceSettingsDialog()
{
	delete file;
	delete config_theme;
	delete theme_loader;
}


