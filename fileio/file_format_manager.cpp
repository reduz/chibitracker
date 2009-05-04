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



 /***************************************************************************
                          file_format_manager.cpp  -  description
                             -------------------
    begin                : Thu Apr 12 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "file_format_manager.h"
#include "mixer/audio_lock.h"
#include <stdio.h>

void FileFormatManager::register_loader(Loader *p_loader) {

	if (loader_count>=MAX_LOADERS)
		return;
 	loaders[loader_count++]=p_loader;
}

Loader::Error FileFormatManager::load_song(const char *p_file,Song *p_song) {


	Loader::Error tmperr=Loader::FILE_UNRECOGNIZED;
	
	AudioLock::begin();
	
	for (int i=0;i<loader_count;i++) {

		if (!loaders[i]->can_load_song())
			continue;

		tmperr=loaders[i]->load_song(p_file,p_song);
		
		if (tmperr!=Loader::FILE_UNRECOGNIZED)
			break;

	}
	
	AudioLock::end();
	return tmperr;
}

Loader::Error FileFormatManager::load_sample(const char *p_file,Sample *p_sample) {

	Loader::Error tmperr=Loader::FILE_UNRECOGNIZED;
	
	AudioLock::begin();
	
	for (int i=0;i<loader_count;i++) {

		if (!loaders[i]->can_load_sample())
			continue;

		tmperr=loaders[i]->load_sample(p_file,p_sample);

		if (tmperr!=Loader::FILE_UNRECOGNIZED)
			break;

	}
	AudioLock::end();
	
	return tmperr;
}
Loader::Error FileFormatManager::load_instrument(const char *p_file,Song *p_song, int p_instrument_idx) {

	Loader::Error tmperr=Loader::FILE_UNRECOGNIZED;
	
	AudioLock::begin();
	
	for (int i=0;i<loader_count;i++) {

		if (!loaders[i]->can_load_instrument())
			continue;

		tmperr=loaders[i]->load_instrument(p_file,p_song,p_instrument_idx);

		if (tmperr!=Loader::FILE_UNRECOGNIZED)
			break;

	}
	
	AudioLock::end();
	
	return tmperr;

}


void FileFormatManager::register_saver(Saver *p_saver) {
	
	if (saver_count>=MAX_SAVERS)
		return;
	savers[saver_count++]=p_saver;
	
}
Saver::Error FileFormatManager::save_song(const char *p_file,Song *p_song){
	
	Saver::Error tmperr=Saver::UNKNOWN_EXTENSION;
	
	AudioLock::begin();
	
	for (int i=0;i<saver_count;i++) {

		
		tmperr=savers[i]->save_song(p_file,p_song);

		if (tmperr!=Saver::UNKNOWN_EXTENSION)
			break;

	}
	
	AudioLock::end();
	
	return tmperr;
	
}
Saver::Error FileFormatManager::save_sample(const char *p_file, Sample *p_sample){
	
	Saver::Error tmperr=Saver::UNKNOWN_EXTENSION;
	
	AudioLock::begin();
	
	for (int i=0;i<saver_count;i++) {

//		printf("trying saver %i\n",i);
		tmperr=savers[i]->save_sample(p_file,p_sample);

		if (tmperr!=Saver::UNKNOWN_EXTENSION)
			break;

	}
	
	AudioLock::end();
	
	return tmperr;
	
}
Saver::Error FileFormatManager::save_instrument(const char *p_file, Song *p_song, int p_instrument_idx){
	
	Saver::Error tmperr=Saver::UNKNOWN_EXTENSION;
	
	AudioLock::begin();
	
	for (int i=0;i<saver_count;i++) {

		tmperr=savers[i]->save_instrument(p_file,p_song,p_instrument_idx);

		if (tmperr!=Saver::UNKNOWN_EXTENSION)
			break;

	}
	
	AudioLock::end();
	
	return tmperr;

}
FileFormatManager::FileFormatManager(){

	loader_count=0;
	saver_count=0;
}
FileFormatManager::~FileFormatManager(){
	
	for (int i=0;i<loader_count;i++)
		delete loaders[i];
	for (int i=0;i<saver_count;i++)
		delete savers[i];
}
