#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include "mixer/mixer_soft.h"
#include "mixer/sample_manager_mempool.h"
#include "drivers/mempool_malloc.h"
#include "mempool_fixedarea.h"
#include "song/song.h"
#include "player/player_data.h"
#include "fileio/loader_it.h"
#include "drivers/file_access.h"

//MemPool_Malloc mempool;
MemPool_FixedArea mempool(malloc(1024*1024),1024*1024);;
SampleManager_MemPool sample_manager;
Mixer_Soft mixer;
Song song;
Player player(&mixer,&song);


Sample_ID sample_id;
int volume=255;



 /**
 * The process callback for this JACK application.
 * It is called by JACK at the appropriate times.
  */
void process (void* arg, Uint8* p_stream, int p_len) {


	Sint16 *data=(Sint16*)p_stream;

	int todo=p_len/4;
	while (todo) {

		int mixed = mixer.process(todo);
		Sint32 * mixdown = mixer.get_mixdown_buffer_ptr();
		for (int i=0;i<mixed;i++) {

			*data++ = (*(mixdown++)>>16);
			*data++ = (*(mixdown++)>>16);
		}

		todo-=mixed;
	}

}


/**
 * This is the shutdown callback for this JACK application.
 * It is called by JACK if the server ever shuts down or
 * decides to disconnect the client.
 */

int main(int argc, char *argv[]) {

	
	printf("is=%i\n",sizeof(Song));
//	const char **ports;
		
	/* try to become a client of the JACK server */


	FileAccess file;
	Loader_IT loader(&file);
	
	
	if ( loader.load_song(argv[1],&song) )
		return 255;
	
	
	player.play_start_song();
	
	mixer.set_mix_frequency( 44100 );
	/* tell the JACK server that we are ready to roll */

	
	
	SDL_Init(SDL_INIT_AUDIO);

	/* Open the audio device */
	SDL_AudioSpec *desired, *obtained;
	SDL_AudioSpec *hardware_spec;

	/* Allocate a desired SDL_AudioSpec */
	desired = new SDL_AudioSpec;

	/* Allocate space for the obtained SDL_AudioSpec */
	obtained = new SDL_AudioSpec;

	/* 22050Hz - FM Radio quality */
	desired->freq=44100;

	/* 16-bit signed audio */
	desired->format=AUDIO_S16SYS;

	/* Mono */
	desired->channels=2;

	/* Large audio buffer reduces risk of dropouts but increases response time */
	desired->samples=8192;

	/* Our callback function */
	desired->callback=process;

	desired->userdata=NULL;

	/* Open the audio device */
	if ( SDL_OpenAudio(desired, obtained) < 0 ){
 		 fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		  exit(-1);
	}
	/* desired spec is no longer needed */
	delete desired;
	hardware_spec=obtained;


	/* Prepare callback for playing */



	/* Start playing */
	SDL_PauseAudio(0);


	while (true) {
		SDL_Delay(1000);
	};

	SDL_Quit();	
	exit (0);
}





