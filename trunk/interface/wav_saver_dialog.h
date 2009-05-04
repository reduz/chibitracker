//
// C++ Interface: wav_saver_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WAV_SAVER_DIALOG_H
#define WAV_SAVER_DIALOG_H

#include "bundles/file_dialog.h"
#include "widgets/label.h"

using namespace GUI;
#include "tracker/tracker.h"
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class WavSaverDialog : public FileDialog {
	
	Tracker *tracker;
	ComboBox *save_hz;
	void save_callback(String p_file);
	
	Window *save_progress;
	
public:
	
	void show();
	
	WavSaverDialog(Window *p_parent,Tracker *p_tracker);
	~WavSaverDialog();

};

#endif
