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
// C++ Interface: sample_editor_clipboard
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SAMPLE_EDITOR_CLIPBOARD_H_
#define SAMPLE_EDITOR_CLIPBOARD_H_


#include "mixer/sample_manager.h"

#include "widgets/button.h"
#include "widgets/label.h"
#include "bundles/margin_group.h"
#include "containers/box_container.h"
#include "tracker/selection.h"

using namespace GUI;

class SampleEditorClipboard : public VBoxContainer {

	SampleManager * sm;

	Selection *selection;
	Sample_ID sample;
	//std::vector<FrameData> clipboard;

        Button *copy;
        Button *cut;
        Button *paste;

	void copy_cbk();
	void cut_cbk();
	void paste_cbk();


public:	

	Signal<> destructive_operation_begin;
	Signal<> destructive_operation_end;
	Signal<> sample_modified;
	


	void set_selection(Selection *p_selection) { selection=p_selection; };
	void set_sample(Sample_ID p_s) { sample=p_s; }

	void clear_clipboard();

	SampleEditorClipboard();
	~SampleEditorClipboard();

};



#endif

