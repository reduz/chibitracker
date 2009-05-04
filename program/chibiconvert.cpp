//
// C++ Implementation: chibitracker_palm
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "tracker/tracker.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "interface/sample_editor_effects.h"
#include "drivers/mempool_malloc.h"
#include "drivers/file_access.h"
#include "mixer/sample_manager_mempool.h"
#include "mixer/mixer_soft.h"

static void show_usage() {
	
	printf("Usage: chibifix_hw [flags] file.it|file.xm\n");
	printf("Valid Flags:\n");
	printf("\t-a <f>\tAlign Loop Points to N frames (2 to 64)\n");
	printf("\t-u\tUnroll Ping-Pong Loops.\n");
	printf("\t-h <hz>\tLimit maximum C5-HZ per sample.\n");
	printf("\t-b\tBass Boost Samples.\n");

}

int main(int argc, char *argv[]) {

	if (argc<3) {
	
		show_usage();	
		return 255;
	}
		

	MemPool_Malloc mempool;
	FileAccess::set_as_default();
	SampleManager_MemPool sample_manager;


	Mixer_Soft mixer_software;
	Tracker tracker(&mixer_software);
		

	Loader::Error  err = tracker.file_manager.load_song(argv[argc-1],tracker.song);
		
	switch( err ) {
		
		case Loader::FILE_UNRECOGNIZED: printf("Error: FILE UNRECOGNIZED\n"); return 255; break;
		case Loader::FILE_CANNOT_OPEN: printf("Error: FILE CAN'T OPEN\n"); return 255; break;
		case Loader::FILE_CORRUPTED: printf("Error: FILE CORRUPTED\n"); return 255; break;
		case Loader::FILE_OUT_OF_MEMORY: printf("Error: FILE OUT OF MEMORY\n"); return 255; break;
		
	}
	
	
	SampleEditorEffects fx;
	Selection s;
	fx.set_selection(&s);
	
	
	for (int s=0;s<Song::MAX_SAMPLES;s++) {
	
		Sample_ID smp=tracker.song->get_sample(s)->get_sample_data();
		if (smp.is_null())
			continue;
	
		fx.set_sample(smp);
		
		bool unroll=false;
		int align=-1;
		int hz=-1;

		
		for (int i=1;i<(argc-1);i++) {
		
			const char *c = argv[i];
			
			if (strcmp(c,"-u")==0) {
			
				unroll=true;				
			
			} 
			
			if (strcmp(c,"-a")==0) {
			
				if (i>=(argc-2)) {
				
					show_usage();
					return(255);
				}
					
				align=atoi( argv[i+1] );
				if (align<2 || align >64) {
				
					show_usage();
					return(255);
				
				}
				i+=1;
			}
			
			if (strcmp(c,"-h")==0) {
			
				if (i>=(argc-2)) {
				
					show_usage();
					return(255);
				}
					
				hz=atoi( argv[i+1] );
				if (hz<4 || hz >192000) {
				
					show_usage();
					return(255);
				
				}
				i+=1;
			}
			

		}
		
		if (hz!=-1 && SampleManager::get_singleton()->get_c5_freq(smp)>hz) 
			fx.command( SampleEditorEffects::CMD_RESAMPLE_FREQ, hz );	
		
		if (unroll) //must be done always first 
			fx.command( SampleEditorEffects::CMD_LOOP_UNROLL, 0 );	
			
		if (align!=-1)
			fx.command( SampleEditorEffects::CMD_LOOP_TO_BOUNDARY, align );
				
		
	}
		
	Saver::Error  serr = tracker.file_manager.save_song(argv[argc-1],tracker.song);
	
	return 0;	
}

