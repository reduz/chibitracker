//
// C++ Interface: tracker
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACKER_H
#define TRACKER_H

#include "mixer/mixer.h"
#include "tracker/editor.h"
#include "tracker/virtual_piano.h"
#include "fileio/file_format_manager.h"
#include "player/player_data.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
struct Tracker{

	Song *song;
	Editor *editor;
	Mixer *mixer;
	
	FileFormatManager file_manager;
	FileAccessWrapper *file_access;
	Player *player;
	
	VirtualPiano *virtual_piano;

	Tracker(Mixer *p_mixer);
	~Tracker();

};

#endif
