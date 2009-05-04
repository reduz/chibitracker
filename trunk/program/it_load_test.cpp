//
// C++ Interface: it_load_test
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "song/song.h"
#include "drivers/mempool_malloc.h"
#include "mempool_fixedarea.h"
#include "mixer/sample_manager_mempool.h"
#include "fileio/loader_it.h"
#include "drivers/file_access.h"
#include <stdio.h>
#include <stdlib.h>

void print_all_song(Song *p_song) {

	printf("Name: %s\n",p_song->get_name() );
	printf("Hilight - Minor: %i , Major %i\n",p_song->get_row_highlight_minor(),p_song->get_row_highlight_major());
	printf("Global Volume: %i  - Mixing Volume %i\n",p_song->get_global_volume(),p_song->get_mixing_volume());
	printf("Stereo Separation: %i\n",p_song->get_stereo_separation());
	printf("Flags: Stereo %i - Instruments %i - Linear Slides: %i - Comp GXX: %i - Old Effects: %i\n",p_song->is_stereo(),p_song->has_instruments(),p_song->has_linear_slides(),p_song->has_compatible_gxx(),p_song->has_old_effects());
	printf("Speed: %i - Tempo %i\n",p_song->get_speed(),p_song->get_tempo());


	printf("-=-=-=-\n");
	printf("Total Mem Used: %i\n",MemPool_Wrapper::get_singleton()->get_total_mem_used());

}


int main(int argc,char *argv[]) {

	if (argc==1) {
		printf("usage %s: IT file\n",argv[0]);
		return 255;
	}

	FileAccess::set_as_default();
	//MemPool_Malloc mempool;
	MemPool_FixedArea mempool(malloc(600*1024),500*1024);
	SampleManager_MemPool sample_manager;

	Song song;

	FileAccess file;
	Loader_IT loader(&file);
	
	/*
	
	switch (file.open(argv[1],FileAccess::READ)) {
		
		case FileAccess::ERROR_ALREADY_IN_USE: printf("ERROR: Already in Use\n"); return 255; break;
		case FileAccess::ERROR_INVALID_PARAMETERS: printf("ERROR: Invalid Parameters\n"); return 255; break;
		case FileAccess::ERROR_OPENING_FILE: printf("ERROR: Opening file\n"); return 255; break;
		case FileAccess::ERROR_READING_FILE: printf("ERROR: Reading file\n"); return 255; break;
		case FileAccess::ERROR_WRITING_FILE: printf("ERROR: Writing file\n"); return 255; break;
		default: printf("File Open OK\n"); break;
		
	}
	*/
	switch( loader.load_song(argv[1],&song) ) {
		
		case Loader::FILE_OK: printf("FILE OK\n"); print_all_song(&song); break;
		case Loader::FILE_UNRECOGNIZED: printf("FILE UNRECOGNIZED\n"); break;
		case Loader::FILE_CORRUPTED: printf("FILE CORRUPTED\n"); break;
		case Loader::FILE_CANNOT_OPEN: printf("FILE CAN'T OPEN\n"); 		
		case Loader::FILE_OUT_OF_MEMORY: printf("FILE OUT OF MEMORY\n"); break;
		
	}

	
	return 0;

}
