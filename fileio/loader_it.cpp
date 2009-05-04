//
// C++ Implementation: loader_it
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "loader_it.h"
#include "error_macros.h"

bool Loader_IT::can_load_song() { return true; }
bool Loader_IT::can_load_sample() { return true; }
bool Loader_IT::can_load_instrument() { return true; }

Loader::Error Loader_IT::load_song(const char *p_file,Song *p_song) {
	
	
	song=p_song;

	if (file->open( p_file, FileAccessWrapper::READ )!=FileAccessWrapper::OK)
		return Loader::FILE_CANNOT_OPEN;
	
	
	Error err;
	
	if ((err=load_header())) {
		file->close();
		return err;
	}
	
	if ((err=load_orders())) {
		file->close();
		return err;
	}
	
	if ((err=load_patterns())) {
		file->close();
		return err;
	}
	
	if ((err=load_samples())) {
		file->close();
		return err;
	}
	
	if ((err=load_effects())) {
		file->close();
		return err;
	}

	if ((err=load_instruments())) {
		file->close();
		return err;
	}
	
	if ((err=load_message())) {
		file->close();
		return err;
	}

	file->close();
	return FILE_OK;
	
}




Loader::Error Loader_IT::load_sample(const char *p_file,Sample *p_sample) {
	
	if (file->open( p_file, FileAccessWrapper::READ )!=FileAccessWrapper::OK)
		return Loader::FILE_CANNOT_OPEN;
	
	p_sample->reset();
	Loader::Error res=load_sample(p_sample);
		
	file->close();
	
	return res;
}
Loader::Error Loader_IT::load_instrument(const char *p_file,Song *p_song,int p_instr_idx) {
	
	ERR_FAIL_INDEX_V(p_instr_idx,Song::MAX_INSTRUMENTS,Loader::FILE_CANNOT_OPEN);
	
	if (file->open( p_file, FileAccessWrapper::READ )!=FileAccessWrapper::OK)
		return Loader::FILE_CANNOT_OPEN;
		
	
	p_song->get_instrument( p_instr_idx )->reset();
	
	
	int samples=0;
	Loader::Error res=load_instrument( p_song->get_instrument( p_instr_idx ), &samples );
		
	if (res) {
		file->close();
		return res;
	}
	
	
	char exchange[Song::MAX_SAMPLES];
	for (int i=0;i<Song::MAX_SAMPLES;i++)
		exchange[i]=0;
			
	for (int i=0;i<samples;i++) {
		
		file->seek( 554+i*80 ); //i think this should work?! seems to.. but i'm not sure	
		
		/* find free sample */
		
		int free_idx=-1;
		for (int s=0;s<Song::MAX_SAMPLES;s++) {
			
			if (p_song->get_sample( s )->get_sample_data().is_null()) {
				free_idx=s;
				break;
			}
		}
		if (free_idx==-1)
			break; //can't seem to be able to load more samples
		
		exchange[i]=free_idx;
		res=load_sample( p_song->get_sample( free_idx ) );
		
		if (res) {
			
			file->close();
			return res;
		}
	}
	
	for (int i=0;i<Note::NOTES;i++) {
		
		int smp=song->get_instrument(p_instr_idx)->get_sample_number(i);
		
		if (smp>=Song::MAX_SAMPLES)
			continue;
		
		if (smp<0)
			continue;
		
		smp=exchange[smp];
		
		song->get_instrument(p_instr_idx)->set_sample_number(i,smp);
		
	}
	
	file->close();

	return res;
	
}

Loader_IT::Loader_IT(FileAccessWrapper *p_file) {

	file=p_file;

}
