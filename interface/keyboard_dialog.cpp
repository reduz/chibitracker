//
// C++ Implementation: keyboard_dialog
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
#include "containers/center_container.h"
#include "containers/box_container.h"
#include "keyboard_dialog.h"
#include "tracker/key_bindings.h"
#include "base/keyboard.h"
#include "widgets/separator.h"

#include <stdio.h>

void KeyboardDialog::keybind_selected( int p_which) {

	update_kb_text();

}

void KeyboardDialog::update_kb_text() {
	
	int idx=list->get_selected();
	if (idx<0)
		return;
	
	unsigned int bind=KeyBind::get_keybind_code( idx );
	if (bind==NO_BIND)
		bind_string->set_text("No KeyBind");
	else
		bind_string->set_text( Keyboard::get_code_name( bind ) );
	
}
void KeyboardDialog::update_keybind_list() {

	
	bind_string->clear();
	list->clear();
	for(int i=0;i<KeyBind::get_keybind_count();i++) {
		
		list->add_string( KeyBind::get_keybind_description(i));
	}
	
}

void KeyboardDialog::kb_change_ok() {
	
	int idx=list->get_selected();
	if (idx<0)
		return;
	
	kb_change_window->hide();
	
	KeyBindList bind_exists=KeyBind::get_bind(kb_change_last);
	
	if (bind_exists!=KB_MAX && bind_exists!=idx) {
		
		mb->show( Keyboard::get_code_name( kb_change_last ) + String(" Already assigned to: ")+KeyBind::get_keybind_description(bind_exists) );
		return;
	}
	
	KeyBind::set_keybind_code(idx,kb_change_last);
	update_kb_text();

}
void KeyboardDialog::key_handler(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {
	
	if (!p_press)
		return ;
	kb_change_last=p_scan_code|p_modifier_mask;
	kb_change_label->set_text( Keyboard::get_code_name( kb_change_last ) );
	
	return ;
}


void KeyboardDialog::change_cbk() {

	int idx=list->get_selected();
	if (idx<0)
		return;
	
	kb_change_last=KeyBind::get_keybind_code( idx );
	if (kb_change_last==NO_BIND)
		kb_change_label->set_text("No KeyBind");
	else
		kb_change_label->set_text(Keyboard::get_code_name( kb_change_last ));
	
	kb_change_window->show();
}
void KeyboardDialog::clear_cbk() {
	
	int idx=list->get_selected();
	if (idx<0)
		return;
	
	KeyBind::set_keybind_code(idx,NO_BIND);

	update_kb_text();
	
}

void KeyboardDialog::read_slot(String p_section,String p_entry,String p_value) {
	
	if (p_section!="KeyBindings")
		return;

	if (p_entry=="keybind_count") {
		
		allow_kb_load=( (int)p_value.to_double()==KB_MAX );
		
	} else if (p_entry=="keyrepeat_rate") {
		
		spin_rate->get_range()->set( p_value.to_double() );
		
	} else if (p_entry=="keyrepeat_delay") {
		
		spin_delay->get_range()->set( p_value.to_double() );
		
	} else {
	
		if (!allow_kb_load)
			return;
		
		int entrynum=(int)(p_entry.to_double());
		
		if (entrynum<=0 || entrynum>KB_MAX)
			return;
		
		entrynum--;
		
		int code=(int)(p_value.to_double());
		
		KeyBind::set_keybind_code( entrynum, code );
	}	
	
}
void KeyboardDialog::read_finished() {
	
	
	update_kb_text();
}


void KeyboardDialog::save_slot() {
	

	config->set_section( "KeyBindings" );
	config->add_entry( "keybind_count",String::num( KB_MAX ),"Please use UI to edit");
	config->add_entry( "keyrepeat_rate",String::num( spin_rate->get_range()->get() ),"ms");
	config->add_entry( "keyrepeat_delay",String::num( spin_delay->get_range()->get() ),"ms");
	
	for(int i=0;i<KeyBind::get_keybind_count();i++) {
		
		int code=KeyBind::get_keybind_code(i);
		config->add_entry( String::num(i+1) , String::num(code),String( KeyBind::get_keybind_description(i))+" - code is: "+ String(Keyboard::get_code_name( code ) ));
		list->add_string( KeyBind::get_keybind_description(i));
	}
	
}

void KeyboardDialog::keyrepeat_changed(double) {
	
	get_painter()->set_key_repeat( (int)spin_delay->get_range()->get(), (int)spin_rate->get_range()->get() );
}


KeyboardDialog::KeyboardDialog(Window *p_parent,ConfigApi *p_config) : Window(p_parent,MODE_POPUP,SIZE_TOPLEVEL_CENTER) {
	
	config=p_config;
	WindowBox *vb = new WindowBox("Keyboard Configuration");	
	set_root_frame( vb );
	
	MarginGroup *mg = vb->add( new MarginGroup("Key Binding List:"),1);
	HBoxContainer *hbc = mg->add( new HBoxContainer,1 );
	list=hbc->add( new List,1 );
	list->selected_signal.connect(this,&KeyboardDialog::keybind_selected);
	hbc->add( new VScrollBar )->set_range( list->get_range() );
	mg = vb->add( new MarginGroup("Selected Bind:"));
	bind_string=mg->add( new LineEdit );
	hbc=mg->add( new CenterContainer)->set(new HBoxContainer );
	hbc->add(new Button("Change"))->pressed_signal.connect(this,&KeyboardDialog::change_cbk );
	hbc->add(new Button("Clear"))->pressed_signal.connect(this,&KeyboardDialog::clear_cbk );
	
	update_keybind_list();
	
	kb_change_window = new Window(this,MODE_POPUP,SIZE_CENTER);
	mg = new MarginGroup("Press New KeyBind:        ");
	kb_change_window->set_root_frame( mg );
	kb_change_label=mg->add( new Label());
	kb_change_label->set_align( Label::ALIGN_CENTER );
	Button *b  = mg->add( new CenterContainer)->set(new Button("Apply") );
	b->set_focus_mode( FOCUS_NONE );
	b->pressed_signal.connect(this,&KeyboardDialog::kb_change_ok );
	kb_change_window->key_unhandled_signal.connect(this,&KeyboardDialog::key_handler);
	
	vb->add( new HSeparator );
	
	HBoxContainer *key_hb =vb->add( new HBoxContainer );
	
	spin_delay=key_hb->add( new MarginGroup("Key Repeat Delay (ms):"),1)->add(new SpinBox );
	spin_delay->get_range()->config( 1,1000, get_painter()->get_key_repeat_delay(), 1 );
	spin_delay->get_range()->value_changed_signal.connect(this,&KeyboardDialog::keyrepeat_changed);
	spin_rate=key_hb->add( new MarginGroup("Key Repeat Rate (ms):"),1)->add(new SpinBox );
	spin_rate->get_range()->config( 1,1000, get_painter()->get_key_repeat_rate(), 1 );
	spin_rate->get_range()->value_changed_signal.connect(this,&KeyboardDialog::keyrepeat_changed);
	vb->add( new MarginGroup("Test:"))->add(new LineEdit);
	mb= new MessageBox(this);
	
	config->write_entry_signal.connect( this, &KeyboardDialog::save_slot );	
	config->read_entry_signal.connect( this, &KeyboardDialog::read_slot );
	config->read_finished_signal.connect( this, &KeyboardDialog::read_finished );
	
	allow_kb_load=false;
}


KeyboardDialog::~KeyboardDialog()
{
}


