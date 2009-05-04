//
// C++ Implementation: pattern_transpose
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "widgets/line_edit.h"
#include "containers/center_container.h"
#include "widgets/separator.h"
#include "pattern_transpose.h"


void PatternTranspose::show() {

	apply_b->get_focus();
	Window::show();
}

void PatternTranspose::raise() {

	int first=trans[0]->get_selected();
	for (int i=0;i<12;i++) {
		
		if (i!=11) {
			
			trans[i]->select( trans[i+1]->get_selected() );
		} else {
			
			trans[i]->select( first );
		}
	}
}
void PatternTranspose::lower() {
	
	int last=trans[11]->get_selected();
	for (int i=11;i>=0;i--) {
		
		if (i!=0) {
			
			trans[i]->select( trans[i-1]->get_selected() );
		} else {
			
			trans[i]->select( last );
		}
	}
	
}
void PatternTranspose::reset() {
	
	for (int i=0;i<12;i++) {
		
		
		trans[i]->select( i );
	}
	
}
void PatternTranspose::apply() {
	
	Uint8 notes[12];
	
	for (int i=0;i<12;i++) {
		
		
		notes[i]=trans[i]->get_selected();
	}
	
	editor->transpose( notes );
	hide();
	update_pattern.call();
	
	
}


PatternTranspose::PatternTranspose(Window *p_parent,Editor *p_editor) : Window(p_parent,Window::MODE_POPUP,Window::SIZE_CENTER) {

	editor=p_editor;
	
	VBoxContainer *vb = new VBoxContainer;
	set_root_frame( vb );
	
	MarginGroup *mg=vb->add(new MarginGroup("Note Relationship"));
	
	for (int i=0;i<12;i++) {
		
		static const char* notes[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
		
		HBoxContainer *hb=mg->add(new HBoxContainer);
		
		LineEdit *le=hb->add(new LineEdit,1);
		le->set_text(notes[i]);
		le->set_editable( false );
		
		hb->add(new HSeparator,2);
		
		trans[i]=hb->add(new ComboBox,2);
		
		for (int j=0;j<12;j++) {
			
			trans[i]->add_string(notes[j]);
		}
		
		trans[i]->select(i);
	}
	
	mg=vb->add(new MarginGroup("Actions"));
	
	HBoxContainer *hb = mg->add(new CenterContainer)->set(new HBoxContainer);
		
	hb->add(new Button("Clear"))->pressed_signal.connect(this,&PatternTranspose::reset);
	hb->add(new VSeparator);
	hb->add(new Button("Lower"))->pressed_signal.connect(this,&PatternTranspose::lower);
	hb->add(new Button("Raise"))->pressed_signal.connect(this,&PatternTranspose::raise);
	hb->add(new VSeparator);
	apply_b=hb->add(new 	Button("Apply"));
	apply_b->pressed_signal.connect(this,&PatternTranspose::apply);

	
	
}


PatternTranspose::~PatternTranspose() {

}


