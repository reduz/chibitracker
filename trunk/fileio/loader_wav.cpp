//
// C++ Implementation: loader_wav
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "loader_wav.h"
#include "error_macros.h"
#include <stdio.h>
Loader::Error Loader_WAV::load_sample(const char *p_file,Sample *p_sample) {

	if (file->open( p_file, FileAccessWrapper::READ )!=FileAccessWrapper::OK)
		return Loader::FILE_CANNOT_OPEN;
	

	/* CHECK RIFF */
	char riff[5];
	riff[4]=0;
	file->get_byte_array((Uint8*)&riff,4); //RIFF

	if (riff[0]!='R' || riff[1]!='I' || riff[2]!='F' || riff[3]!='F') {

		file->close();
		return FILE_UNRECOGNIZED;
	}


	/* GET FILESIZE */
	Uint32 filesize=file->get_dword();



	/* CHECK WAVE */
	
	char wave[4];
	
	file->get_byte_array((Uint8*)&wave,4); //RIFF

	if (wave[0]!='W' || wave[1]!='A' || wave[2]!='V' || wave[3]!='E') {


		file->close();
		return FILE_UNRECOGNIZED;
	}
	
	SampleManager *sm = SampleManager::get_singleton(); //get sample manager ready

	bool format_found=false;
	int format_bits=0;
	int format_channels=0;
	int format_freq=0;

	Error err=FILE_CORRUPTED;
	
	while (!file->eof_reached()) {


		/* chunk */
		char chunkID[4];
		file->get_byte_array((Uint8*)&chunkID,4); //RIFF


		
		/* chunk size */
		Uint32 chunksize=file->get_dword();
		Uint32 file_pos=file->get_pos(); //save file pos, so we can skip to next chunk safely

		if (file->eof_reached()) {

			ERR_PRINT("EOF REACH");
			break;
		}
		
		if (chunkID[0]=='f' && chunkID[1]=='m' && chunkID[2]=='t' && chunkID[3]==' ' && !format_found) {
			/* IS FORMAT CHUNK */

			Uint16 compression_code=file->get_word();
			
			if (compression_code!=1) {
				ERR_PRINT("Format not supported for WAVE file (not PCM)");
				err=FILE_UNRECOGNIZED;
				break;
			}

			format_channels=file->get_word();
			if (format_channels!=1 && format_channels !=2) {

				ERR_PRINT("Format not supported for WAVE file (not stereo or mono)");
				err=FILE_UNRECOGNIZED;
				break;

			}

			format_freq=file->get_dword(); //sampling rate

			file->get_dword(); // average bits/second (unused)
			file->get_word(); // block align (unused)
			format_bits=file->get_word(); // bits per sample

			if (format_bits%8) {

				ERR_PRINT("Strange number of bits in sample (not 8,16,24,32)");
				err=FILE_UNRECOGNIZED;
				break;
			}

			/* Dont need anything else, continue */
			format_found=true;
		}


		if (chunkID[0]=='d' && chunkID[1]=='a' && chunkID[2]=='t' && chunkID[3]=='a') {
			/* IS FORMAT CHUNK */


			if (!format_found) {
				ERR_PRINT("'data' chunk before 'format' chunk found.");
				break;

			}

			int frames=chunksize;
			frames/=format_channels;
			frames/=(format_bits>>3);

			p_sample->reset();
			Sample_ID data = sm->create( (format_bits==8) ? false : true , (format_channels==2)?true:false, frames );
			sm->set_c5_freq( data, format_freq );

			if (data.is_null()) {

				err=FILE_OUT_OF_MEMORY;
				break;
			}

			if (!sm->lock_data( data ) && sm->get_data( data ) ) {
				
				void * data_ptr=sm->get_data( data );
			
				for (int i=0;i<frames;i++) {
	
	
					for (int c=0;c<format_channels;c++) {
	
	
						if (format_bits==8) {
							// 8 bit samples are UNSIGNED
							
							Uint8 s = file->get_byte();
							s-=128;
							Sint8 *sp=(Sint8*)&s;
							
							Sint8 *data_ptr8=&((Sint8*)data_ptr)[i*format_channels+c];
							
							*data_ptr8=*sp;
	
						} else {
							//16+ bits samples are SIGNED
							Uint16 s = file->get_word();
							Sint16 *sp=(Sint16*)&s;
							// if sample is > 16 bits, just read extra bytes
							
							for (int b=0;b<((format_bits>>3)-2);b++)
								file->get_byte(); //discard extra bytes
		
							Sint16 *data_ptr16=&((Sint16*)data_ptr)[i*format_channels+c];

							*data_ptr16=*sp;
						}
					}
					
				}
				
				sm->unlock_data( data );
			}

			if (file->eof_reached()) {

				err=FILE_CORRUPTED;
				sm->destroy( data );
				break;
			} else {

				err=FILE_OK;
				p_sample->set_sample_data( data );
				break;

			}
		}

		file->seek( file_pos+chunksize );
	}

	if (!err) {
		/* Set file name */

		int from=0;
		int i=0;
		while(p_file[i]) {
						
			if (p_file[i]=='/' || p_file[i]=='\\') {

				from=i+1;
			}
			i++;
		}

		p_sample->set_name(&p_file[from]);

	}
	
	file->close();
	return err;

}


Loader_WAV::Loader_WAV(FileAccessWrapper *p_file) {

	file=p_file;
}


Loader_WAV::~Loader_WAV()
{
}


