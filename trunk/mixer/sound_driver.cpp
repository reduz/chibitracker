//
// C++ Implementation: sound_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_driver.h"


void AudioParams::set_mix_freq(int p_freq) {

	mix_freq=p_freq;
}
void AudioParams::set_bits(Bits p_bits) {

	bits=p_bits;
}
void AudioParams::set_buffer_size(int p_buffer_size) {

	buffer_size=p_buffer_size;
}

void AudioParams::set_stereo(bool p_stereo) {
	
	stereo=p_stereo;
}

void AudioParams::set_mixer(Mixer *p_mixer) {
	
	mixer=p_mixer;
}


int AudioParams::get_mix_freq() {

	return mix_freq;
}
AudioParams::Bits AudioParams::get_bits() {

	return bits;
}
int AudioParams::get_buffer_size() {

	return buffer_size;
}

bool AudioParams::is_stereo() {
	
	return stereo;
}

Mixer *AudioParams::get_mixer() {
	
	return mixer;
}


AudioParams::AudioParams() {
	
	/* Standard Values */
	bits=BITS_16;
	mix_freq=44100;
	buffer_size=2048;
	stereo=true;
	mixer=0;
	
}

SoundDriver::SoundDriver()
{
}


SoundDriver::~SoundDriver()
{
}


