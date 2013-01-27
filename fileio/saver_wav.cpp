//
// C++ Implementation: saver_wav
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "saver_wav.h"
#include "error_macros.h"
#include <stdio.h>
static char UCASE(char p_c) {
	
	if (p_c>='a' && p_c<='z')
		p_c-='a'-'A';
	
	return p_c;
}

Saver::Error Saver_WAV::save_song(const char *p_filename,Song *p_song) {

	return UNKNOWN_EXTENSION;
}
Saver::Error Saver_WAV::save_sample(const char *p_filename,Sample *p_sample) {

	if (p_sample->get_sample_data().is_null()) {
		ERR_PRINT("NULL SAMPLE!!");
		return SAVE_ERROR; //dont M0ck me
	}
	
	int file_len=0;
	while (p_filename[file_len]!=0)
		file_len++;
		
	
	if ( (file_len<4) || UCASE(p_filename[file_len-1])!='V' || UCASE(p_filename[file_len-2])!='A' || UCASE(p_filename[file_len-3])!='W' ||  (p_filename[file_len-4])!='.' ) return UNKNOWN_EXTENSION;

	if (file->open(p_filename,FileAccessWrapper::WRITE)) {

		ERR_PRINT("NO WRITE!");
		return SAVE_ERROR;
	}

	file->store_byte_array((const Uint8*)"RIFF",4);

	int total_size=4 /* WAVE */ + 8 /* fmt+size */ + 16 /* format */ + 8 /* data+size */;

	SampleManager *sm = SampleManager::get_singleton();

	Sample_ID sample=p_sample->get_sample_data();
	int sample_data_size=sm->get_size( sample );
	sample_data_size*=sm->is_16bits( sample ) ? 2 :1;
	sample_data_size*=sm->is_stereo( sample ) ? 2 :1;

	total_size+=sample_data_size;

	bool pad_a_byte=false; //RIFF crap must be word aligned
	if ( (sample_data_size % 2)==1) {
		pad_a_byte=true;
		total_size+=1;
	}

	file->store_dword( total_size );

	file->store_byte_array((const Uint8*)"WAVE",4);

	/* FORMAT CHUNK */
	
	file->store_byte_array((const Uint8*)"fmt ",4);

	file->store_dword( 16 ); //standard format, no extra fields

	
	file->store_word( 1 ); // compression code, standard PCM
			

	file->store_word( sm->is_stereo( sample ) ? 2 : 1 );
	file->store_dword( sm->get_c5_freq( sample ) );
	

	/* useless stuff the format asks for */

	int bits_per_sample=sm->is_16bits(sample)?16:8;
	int blockalign = bits_per_sample / 8 * ( sm->is_stereo( sample ) ? 2 : 1 );
	int bytes_per_sec = sm->get_c5_freq( sample )*blockalign;
	
	file->store_dword( bytes_per_sec );
	file->store_word(blockalign); // block align (unused)
	file->store_word( bits_per_sample );

	if (sm->get_loop_type(sample)!=LOOP_NONE) {

		file->store_byte_array((const Uint8*)"smpl",4);
		file->store_dword((6+9)*4); //manufacturer

		file->store_dword(0); //manufacturer
		file->store_dword(0); //product
		file->store_dword(1); //period
		file->store_dword(48); //midi note
		file->store_dword(0); //midi pitch
		file->store_dword(0); //smpte format
		file->store_dword(0); //smpte offset
		file->store_dword(1); //sampler loops
		file->store_dword(24); //extra bytes

		file->store_dword(0); //cue point
		file->store_dword(sm->get_loop_type(sample)==LOOP_FORWARD?0:1); //loop
		file->store_dword(sm->get_loop_begin(sample));
		file->store_dword(sm->get_loop_end(sample));
		file->store_dword(0); //fraction
		file->store_dword(0); //play count
		
	}


	/* DATA CHUNK */

	file->store_byte_array((const Uint8*)"data",4);
		
	file->store_dword( sample_data_size );

	for(int i=0;i<sm->get_size( sample );i++) {

		int ch=sm->is_stereo( sample ) ? 2 : 1;
				
		for (int c=0;c<ch;c++) {

			Sint16 s=sm->get_data( sample, i, c );

			if (sm->is_16bits( sample )) {

				Uint16 *smp=(Uint16*)&s;
				file->store_word( *smp );

			} else {

				Sint8 sample8=s>>8;
				sample8+=128;
				Uint8 *smp=(Uint8*)&sample8;
				file->store_byte( *smp );

			}

		}

	}

	if (pad_a_byte)
		file->store_byte( 0 ); //pad zero byte, so RIFF can be word aligned

	file->close();

	return SAVE_OK;
	

	
}
Saver::Error Saver_WAV::save_instrument(const char *p_filename,Song *p_song,int p_instrument_index) {

	return UNKNOWN_EXTENSION;

}


Saver_WAV::Saver_WAV(FileAccessWrapper *p_file) {

	file=p_file;
}


Saver_WAV::~Saver_WAV() {
}


