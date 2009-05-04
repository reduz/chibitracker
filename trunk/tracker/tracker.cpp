//
// C++ Implementation: tracker
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tracker.h"
#include "fileio/saver_it.h"
#include "fileio/saver_xm.h"
#include "fileio/saver_wav.h"
#include "fileio/loader_it.h"
#include "fileio/loader_xm.h"
#include "fileio/loader_s3m.h"
#include "fileio/loader_mod.h"
#include "fileio/loader_wav.h"

Tracker::Tracker(Mixer *p_mixer) {

	mixer=p_mixer;
	song = new Song;
	player = new Player(mixer,song);
	editor = new Editor(song,player);
	virtual_piano = new VirtualPiano(player,mixer);
	file_access = FileAccessWrapper::create();
	file_manager.register_loader( new Loader_IT(file_access) );
	file_manager.register_loader( new Loader_XM(file_access) );
	file_manager.register_loader( new Loader_WAV(file_access) );
	file_manager.register_loader( new Loader_S3M(file_access) );
	file_manager.register_loader( new Loader_MOD(file_access) );
	
	file_manager.register_saver( new Saver_IT(file_access) );
	file_manager.register_saver( new Saver_XM(file_access) );
	file_manager.register_saver( new Saver_WAV(file_access) );
}


Tracker::~Tracker()
{
	player->play_stop();
	delete song;
	delete editor;
	delete player;
	delete file_access;
	
}


