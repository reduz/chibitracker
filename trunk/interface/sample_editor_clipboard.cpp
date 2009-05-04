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
// C++ Implementation: sample_editor_clipboard
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "sample_editor_clipboard.h"


void SampleEditorClipboard::copy_cbk() {
/*
	if (!selection->is_active())
		return;
	if (!sdata || !sdata->get_length())
		return;

        clipboard.resize(selection->get_end() - selection->get_begin());
	for (int i=0;i<(int)clipboard.size();i++) {

		clipboard[i]=sdata->get_frame( selection->get_begin() + i );
	}

*/
}
void SampleEditorClipboard::cut_cbk() {

	/*
	if (!selection->is_active())
		return;
	if (!sdata || !sdata->get_length())
		return;

	copy_cbk();

	std::vector<FrameData> aux_data;

	aux_data.resize(sdata->get_length() - ( selection->get_end() - selection->get_begin() ) );

	if ( (sdata->get_loop_begin()>=selection->get_begin()) && (sdata->get_loop_begin()<selection->get_end()) ) {

		sdata->set_loop_begin(selection->get_begin());
	} else if (sdata->get_loop_begin()>=selection->get_end()) {

		sdata->set_loop_begin( sdata->get_loop_begin() - ( selection->get_end() - selection->get_begin() ) );

	}

	if ( (sdata->get_loop_end()>=selection->get_begin()) && (sdata->get_loop_end()<selection->get_end()) ) {

		sdata->set_loop_end(selection->get_begin());
	} else if (sdata->get_loop_end()>=selection->get_end()) {

		sdata->set_loop_end( sdata->get_loop_end() - ( selection->get_end() - selection->get_begin() ) );

	}

	for (int i=0;i<selection->get_begin();i++)
		aux_data[i]=sdata->get_frame(i);
	for (int i=selection->get_end();i<sdata->get_length();i++)
		aux_data[ selection->get_begin() + (i-selection->get_end() )]=sdata->get_frame(i);

	
	


	destructive_operation_begin();

	sdata->import( aux_data );
	
	selection->clear();
	destructive_operation_end();
	
	sample_modified();
	*/
}

void SampleEditorClipboard::paste_cbk() {
	/*
	if (!selection->is_active())
		return;
	if (!sdata || !sdata->get_length())
		return;

	if (!clipboard.size())
		return;

	std::vector<FrameData> aux_data;

	aux_data.resize(sdata->get_length() + clipboard.size() );

	for (int i=0;i<selection->get_end();i++)
		aux_data[i]=sdata->get_frame(i);

	for (int i=0;i<(int)clipboard.size();i++)
		aux_data[selection->get_end()+i]=clipboard[i];

	for (int i=0;i<( sdata->get_length() - selection->get_end() );i++)
		aux_data[selection->get_end()+i+clipboard.size()]=sdata->get_frame(selection->get_end()+i);


	destructive_operation_begin();


	if  (sdata->get_loop_end()>=selection->get_end()) {

		sdata->set_loop_end( sdata->get_loop_end() + clipboard.size() );

	}

	if  (sdata->get_loop_begin()>=selection->get_end()) {

		sdata->set_loop_begin( sdata->get_loop_begin() + clipboard.size() );

	}
	
	sdata->import(aux_data);

	destructive_operation_end();
	sample_modified();
	*/
}

void SampleEditorClipboard::clear_clipboard() {

//	clipboard.clear();
}


SampleEditorClipboard::SampleEditorClipboard() {

	
	MarginGroup *mg=add(new MarginGroup("Edit:"),1);

	
	copy = mg->add(new Button("Copy"));
	cut = mg->add(new Button("Cut"));
	paste = mg->add(new Button("Paste"));

	selection=NULL;

	copy->pressed_signal.connect( this, &SampleEditorClipboard::copy_cbk );
	cut->pressed_signal.connect( this, &SampleEditorClipboard::cut_cbk );
	paste->pressed_signal.connect( this, &SampleEditorClipboard::paste_cbk );
	
	sm=SampleManager::get_singleton();

}

SampleEditorClipboard::~SampleEditorClipboard() {


}

