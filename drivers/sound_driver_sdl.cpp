//
// C++ Implementation: sound_drivers_dl
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifdef SDL_ENABLED

#include "sound_driver_sdl.h"
#include "error_macros.h"
#include "mixer/mixer.h"
#include <stdlib.h>




void SoundDriverSDL::process(Uint8* p_stream, int p_bytes) {

	static bool processing=false;
	
	if (processing)
		return;
	
	processing=true;
	
	if (!params->get_mixer()) {
		//mixer may not be availabe at all time
		
		for (int i=0;i<p_bytes;i++) //clear audio buffer
			p_stream[i]=0;
		
		processing=false;
		return; //go back
	}
		
	params->get_mixer()->set_mix_frequency( internal_params.freq );

	int todo=p_bytes;
	todo/=internal_params.channels; //discount stereo
	
	
	
	if (internal_params.format==AUDIO_U8) {
		
		processing=false;
		return;
	}
	
	Sint32 aux_max_l=0;
	Sint32 aux_max_r=0;
	
	if (internal_params.format==AUDIO_S8) {


		Sint8 *data=(Sint8*)p_stream;
		
		while (todo) {
	
			int mixed = params->get_mixer()->process(todo);
			Sint32 * mixdown = params->get_mixer()->get_mixdown_buffer_ptr();
			
			if (params->is_stereo()) {
			
				for (int i=0;i<mixed;i++) {
		
					if (abs(mixdown[0])>aux_max_l)
						aux_max_l=abs(mixdown[0]);
					if (abs(mixdown[1])>aux_max_r)
						aux_max_r=abs(mixdown[1]);
						
					*data++ = (*(mixdown++)>>16);
					*data++ = (*(mixdown++)>>16);
				}
			} else {
				
				for (int i=0;i<mixed;i++) {
		
					if (abs(mixdown[0])>aux_max_l)
						aux_max_l=abs(mixdown[0]);
					if (abs(mixdown[1])>aux_max_r)
						aux_max_r=abs(mixdown[1]);
					
					*data++ = (*(mixdown++)>>17) + (*(mixdown++)>>17);;
				}
				
			}
	
			todo-=mixed;
		}
		
	} else {
		
		todo/=2;		
		Sint16 *data=(Sint16*)p_stream;
		
		while (todo) {
	
			int mixed = params->get_mixer()->process(todo);
			Sint32 * mixdown = params->get_mixer()->get_mixdown_buffer_ptr();
			
			if (params->is_stereo()) {
			
				for (int i=0;i<mixed;i++) {
		
					if (abs(mixdown[0])>aux_max_l)
						aux_max_l=abs(mixdown[0]);
					if (abs(mixdown[1])>aux_max_r)
						aux_max_r=abs(mixdown[1]);
					
					*data++ = (*(mixdown++)>>8);
					*data++ = (*(mixdown++)>>8);
				}
			} else {
				for (int i=0;i<mixed;i++) {
				
					if (abs(mixdown[0])>aux_max_l)
						aux_max_l=abs(mixdown[0]);
					if (abs(mixdown[1])>aux_max_r)
						aux_max_r=abs(mixdown[1]);
					
					*data++ = (*(mixdown++)>>9) + (*(mixdown++)>>9);
				}	
			}
	
			todo-=mixed;
		}	
	}

	if (aux_max_l>max_l)
		max_l=aux_max_l;
	if (aux_max_r>max_r)
		max_r=aux_max_r;
	
	processing=false;
}


static int lockcount=0;

void SoundDriverSDL::lock() {
	
	if (!active)
		return;

	
	
	ERR_FAIL_COND(lockcount<0);
	if (lockcount==0)
		SDL_LockAudio();	
	lockcount++;
}
void SoundDriverSDL::unlock() {
	if (!active)
		return;
	
	ERR_FAIL_COND(lockcount==0)
	
	lockcount--;
	if (lockcount==0)
		SDL_UnlockAudio();	
	
}

Uint16 SoundDriverSDL::get_max_level_l() {
	
	Uint16 val = max_l >> 13;
	max_l=0;
	
	return val;
}
 Uint16 SoundDriverSDL::get_max_level_r() {
	
	Uint16 val = max_r >> 13;
	max_r=0;
	
	return val;
	
}

void SoundDriverSDL::process_cbk(void* arg, Uint8* p_stream, int p_bytes) {

	
	SoundDriverSDL *_this=(SoundDriverSDL*)arg;
	_this->process(p_stream,p_bytes);

}


const char * SoundDriverSDL::get_name() {

	return "SDL Audio";
}


bool SoundDriverSDL::is_active() {

	return active;
}
bool SoundDriverSDL::init() {
	
	
	
	ERR_FAIL_COND_V(active,true);
	ERR_FAIL_COND_V(!params,true);
	
	ERR_FAIL_COND_V(SDL_InitSubSystem( SDL_INIT_AUDIO )<0,true);
	
	/* Open the audio device */
	SDL_AudioSpec *desired;


	/* Allocate a desired SDL_AudioSpec */
	desired = new SDL_AudioSpec;

	/* Allocate space for the internal_params SDL_AudioSpec */

	desired->freq=params->get_mix_freq();

	/* 16-bit signed audio */
	switch (params->get_bits()) {
		case AudioParams::BITS_8: desired->format=AUDIO_S8; break;
		case AudioParams::BITS_16: desired->format=AUDIO_S16SYS; break;
		case AudioParams::BITS_32: desired->format=AUDIO_S16SYS; break;
	}
	/* Mono */
	desired->channels=params->is_stereo()?2:1;

	/* Large audio buffer reduces risk of dropouts but increases response time */
	desired->samples=params->get_buffer_size();

	/* Our callback function */
	desired->callback=process_cbk;

	desired->userdata=this;

	/* Open the audio device */
	if ( SDL_OpenAudio(desired, &internal_params) < 0 ) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		delete desired;
		return true;
	}
	delete desired;

	SDL_PauseAudio(0);

	active=true;
//	printf("all ok!\n");
	params->set_buffer_size( internal_params.samples );
	params->set_mix_freq( internal_params.freq );
	params->set_stereo( internal_params.channels==2 );
	params->set_bits( (internal_params.format==AUDIO_S8)?AudioParams::BITS_8:AudioParams::BITS_16 );
	
	max_l=0;
	max_r=0;
	
	return false;
}

void SoundDriverSDL::finish(){
	
//	printf("Driver done!\n");
	ERR_FAIL_COND(!active);
	
	SDL_CloseAudio();
	SDL_QuitSubSystem( SDL_INIT_AUDIO );
	active=false;
}

void SoundDriverSDL::set_audio_params(AudioParams *p_params){
	
	params=p_params;
}


SoundDriverSDL::SoundDriverSDL() {
	
	active=false;
	params=0;
	max_l=0;
	max_r=0;
	
}


SoundDriverSDL::~SoundDriverSDL() {
	
	if (active)
		finish();
}


#endif