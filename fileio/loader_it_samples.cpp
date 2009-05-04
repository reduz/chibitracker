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
                          loader_it_samples.cpp  -  description
                             -------------------
    begin                : Wed Apr 11 2001
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
#include "loader_it.h"
#include "song/sample.h"
#include "error_macros.h"
struct AuxSampleData {


	Uint32 fileofs;
	Uint32 c5spd;
	Uint32 length;
	Uint32 loop_begin;
	Uint32 loop_end;
	bool loop_enabled;
	bool pingpong_enabled;
	bool is16bit;
	bool stereo;
	bool exists;
	bool compressed;

};


enum IT_Sample_Flags {

	IT_SAMPLE_EXISTS=1,
	IT_SAMPLE_16BITS=2,
	IT_SAMPLE_STEREO=4,
	IT_SAMPLE_COMPRESSED=8,
	IT_SAMPLE_LOOPED=16,
	IT_SAMPLE_SUSTAIN_LOOPED=32,
	IT_SAMPLE_LOOP_IS_PINGPONG=64,
	IT_SAMPLE_SUSTAIN_LOOP_IS_PINGPONG=128
};


Loader::Error Loader_IT::load_sample(Sample *p_sample) {


	AuxSampleData aux_sample_data;

	char aux_header[4];

	file->get_byte_array((Uint8*)aux_header,4);
	
	if (	aux_header[0]!='I' ||
		aux_header[1]!='M' ||
		aux_header[2]!='P' ||
		       aux_header[3]!='S') {
		
		//ERR_PRINT("IT Loader Sample: Failed Identifier");
		return FILE_UNRECOGNIZED;
	}
		

	// Ignore deprecated 8.3 filename
	for (int i=0;i<12;i++) file->get_byte();
	
	file->get_byte(); //ignore zerobyte
	
	p_sample->set_global_volume( file->get_byte() );
	
	/* SAMPLE FLAGS */
       	Uint8 flags=file->get_byte();
	aux_sample_data.loop_enabled=flags&IT_SAMPLE_LOOPED;
	aux_sample_data.pingpong_enabled=flags&IT_SAMPLE_LOOP_IS_PINGPONG;
	aux_sample_data.is16bit=flags&IT_SAMPLE_16BITS;
	aux_sample_data.exists=flags&IT_SAMPLE_EXISTS;
	aux_sample_data.stereo=flags&IT_SAMPLE_STEREO;
	aux_sample_data.compressed=flags&IT_SAMPLE_COMPRESSED;
	
	p_sample->set_default_volume(file->get_byte());
	/* SAMPLE NAME */
	char aux_name[26];
	file->get_byte_array((Uint8*)aux_name,26);
	p_sample->set_name(aux_name);
	
	// ??
	Uint8 convert_flag=file->get_byte();
	// PAN
	Uint8 pan=file->get_byte();
	p_sample->set_pan( pan&0x7F ); 
	p_sample->set_pan_enabled( pan & 0x80 );

	aux_sample_data.length=file->get_dword();

	
	aux_sample_data.loop_begin= file->get_dword();
	aux_sample_data.loop_end= file->get_dword();
	aux_sample_data.c5spd=file->get_dword();
	/*p_sample->data.set_sustain_loop_begin=*/file->get_dword();
	/*p_sample->data.sustain_loop_end=*/file->get_dword();
	aux_sample_data.fileofs=file->get_dword();
	p_sample->set_vibrato_speed( file->get_byte() );
	p_sample->set_vibrato_depth( file->get_byte() );
	p_sample->set_vibrato_rate( file->get_byte() );
	switch( file->get_byte() ) {
		/* Vibrato Wave: 0=sine, 1=rampdown, 2=square, 3=random */
		case 0: p_sample->set_vibrato_type( Sample::VIBRATO_SINE ); break;
		case 1: p_sample->set_vibrato_type( Sample::VIBRATO_SAW ); break;
		case 2: p_sample->set_vibrato_type( Sample::VIBRATO_SQUARE ); break;
		case 3: p_sample->set_vibrato_type( Sample::VIBRATO_RANDOM ); break;
		default: p_sample->set_vibrato_type( Sample::VIBRATO_SINE ); break;
	}
	
	//printf("Name %s - Flags: fileofs :%i - c5spd %i - len %i 16b %i - data?: %i\n",p_sample->get_name(),aux_sample_data.fileofs,aux_sample_data.c5spd, aux_sample_data.length, aux_sample_data.is16bit,aux_sample_data.exists);
	Sample_ID samp_id;
	
	if (aux_sample_data.exists) {
		samp_id=load_sample_data(aux_sample_data);
		SampleManager::get_singleton()->set_c5_freq(samp_id,aux_sample_data.c5spd);
		SampleManager::get_singleton()->set_loop_begin( samp_id,aux_sample_data.loop_begin );
		SampleManager::get_singleton()->set_loop_end( samp_id,aux_sample_data.loop_end );
		Sample_Loop_Type loop_type=aux_sample_data.loop_enabled?( aux_sample_data.pingpong_enabled? LOOP_BIDI: LOOP_FORWARD):LOOP_NONE;
		SampleManager::get_singleton()->set_loop_end( samp_id,aux_sample_data.loop_end );
		SampleManager::get_singleton()->set_loop_type( samp_id, loop_type);
		
	}
	
	//printf("Loaded id is null?: %i\n",samp_id.is_null());
	p_sample->set_sample_data(samp_id);
	if (!samp_id.is_null()) {
		
	//	printf("Loaded ID: stereo: %i len %i 16bit %i\n",SampleManager::get_singleton()->is_stereo(samp_id), SampleManager::get_singleton()->get_size( samp_id), SampleManager::get_singleton()->is_16bits( samp_id) );
	}
	
	ERR_FAIL_COND_V( file->eof_reached(),FILE_CORRUPTED );
	ERR_FAIL_COND_V( file->get_error(),FILE_CORRUPTED );
	
	return FILE_OK;

}

Sample_ID Loader_IT::load_sample_data(AuxSampleData& p_sample_data) {


	int aux_sample_properties = (p_sample_data.is16bit?IT_SAMPLE_16BITS:0)|(p_sample_data.compressed?IT_SAMPLE_COMPRESSED:0)|(p_sample_data.stereo?IT_SAMPLE_STEREO:0);

	file->seek(p_sample_data.fileofs);
	
	SampleManager *sm=SampleManager::get_singleton();

	Sample_ID id;
	
	switch (aux_sample_properties) {

		case (0):  // 8 bits, mono
		case (IT_SAMPLE_16BITS):  // 16 bits mono
		case (IT_SAMPLE_STEREO):  // 8 bits stereo
		case (IT_SAMPLE_16BITS|IT_SAMPLE_STEREO): { // 16 bits mono

			id=sm->create(p_sample_data.is16bit,p_sample_data.stereo,p_sample_data.length); 
			if (id.is_null())
				break;

			
			int chans=p_sample_data.stereo?2:1;
				
			for (int c=0;c<chans;c++) {
						
				for (int i=0;i<p_sample_data.length;i++) {
				
					
					
					if (p_sample_data.is16bit) {
						
						Uint16 s=file->get_word();
						Sint16 *v=(Sint16*)&s;
						sm->set_data(id,i,*v,c);
					} else {
						
						
						Sint8 *v;
						Uint8 s=file->get_byte();
						v=(Sint8*)&s;
						Sint16 v16=*v;
						v16<<=8;
						sm->set_data(id,i,v16,c);
						
					}
				
				}
			}

		} break;
		case (IT_SAMPLE_COMPRESSED): { // 8 bits compressed


			id=sm->create(false,false,p_sample_data.length); 
			if (id.is_null())
				break;
			sm->lock_data(id);
			
			if ( load_sample_8bits_IT_compressed((void*)sm->get_data( id),p_sample_data.length) ) {

				sm->unlock_data(id);
				sm->destroy(id);
				
				break;
			}

			sm->unlock_data(id);


		} break;
		case (IT_SAMPLE_16BITS|IT_SAMPLE_COMPRESSED): { // 16 bits compressed


			id=sm->create(true,false,p_sample_data.length); 
			if (id.is_null())
				break;
			sm->lock_data(id);
			
			if ( load_sample_16bits_IT_compressed((void*)sm->get_data(id),p_sample_data.length) ) {

				sm->unlock_data(id);
				sm->destroy(id);
				break;
			}

			sm->unlock_data(id);

		} break;
		default: {
			
			// I dont know how to handle stereo compressed, does that exist?
		} break;

	}


	return id;
}


Loader::Error Loader_IT::load_samples() {

	for (int i=0;i<header.smpnum;i++) {

		//seek to sample 
		file->seek(0xC0+header.ordnum+header.insnum*4+i*4);
		
		Uint32 final_location=file->get_dword();
		file->seek( final_location );
		

		Error err=load_sample(song->get_sample(i));
		ERR_FAIL_COND_V(err,err);

	}

	if (file->eof_reached() || file->get_error())
		return FILE_CORRUPTED;

	return FILE_OK;
}
/* * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE

 -The following sample decompression code is based on xmp's code.(http://xmp.helllabs.org) which is based in openCP code.

* NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE * NOTICE */

Uint32 Loader_IT::read_n_bits_from_IT_compressed_block (Uint8 p_bits_to_read) {

    Uint32 aux_return_value;
    Uint32 val;
    Uint8 *buffer=(Uint8*)source_position;

    if ( p_bits_to_read <= source_remaining_bits ) {

    	val=buffer[3];
	val<<=8;
    	val|=buffer[2];
	val<<=8;
    	val|=buffer[1];
	val<<=8;
    	val|=buffer[0];

	aux_return_value = val & ((1 << p_bits_to_read) - 1);
	val >>= p_bits_to_read;
	source_remaining_bits -= p_bits_to_read;

	buffer[3]=val>>24;
    	buffer[2]=(val>>16)&0xFF;
    	buffer[1]=(val>>8)&0xFF;
    	buffer[0]=(val)&0xFF;

    } else {
    	aux_return_value=buffer[3];
	aux_return_value<<=8;
    	aux_return_value|=buffer[2];
	aux_return_value<<=8;
    	aux_return_value|=buffer[1];
	aux_return_value<<=8;
    	aux_return_value|=buffer[0];

	Uint32 nbits = p_bits_to_read - source_remaining_bits;
//	aux_return_value = *source_position;
  	source_position++;
        buffer+=4;
    	val=buffer[3];
	val<<=8;
    	val|=buffer[2];
	val<<=8;
    	val|=buffer[1];
	val<<=8;
    	val|=buffer[0];
	aux_return_value |= ((val & ((1 << nbits) - 1)) << source_remaining_bits);
	val >>= nbits;
	source_remaining_bits = 32 - nbits;
	buffer[3]=val>>24;
    	buffer[2]=(val>>16)&0xFF;
    	buffer[1]=(val>>8)&0xFF;
    	buffer[0]=(val)&0xFF;

    }

    return aux_return_value;
}

bool Loader_IT::read_IT_compressed_block (bool p_16bits) {

	Uint16 size;

	size=file->get_word();

	if (file->eof_reached() || file->get_error()) return true;

	compressed_loader_memhandle = MemPool_Wrapper::get_singleton()->alloc_mem( 4* ((size >> 2) + 2) ); 
	if (compressed_loader_memhandle.is_null())
		return true;
	
	MemPool_Wrapper::get_singleton()->lock_mem(compressed_loader_memhandle);
	source_buffer=(Uint32*)MemPool_Wrapper::get_singleton()->get_mem(compressed_loader_memhandle);
	file->get_byte_array((Uint8*)source_buffer,size);
	
	if (file->eof_reached() || file->get_error()) {
		
		free_IT_compressed_block();
		return true;
	}
	
	source_position = source_buffer;
	source_remaining_bits = 32;

	return false;
}

void Loader_IT::free_IT_compressed_block () {


	if (!compressed_loader_memhandle.is_null()) {
	
		MemPool_Wrapper::get_singleton()->unlock_mem(compressed_loader_memhandle);
		MemPool_Wrapper::get_singleton()->free_mem(compressed_loader_memhandle);
		compressed_loader_memhandle=MemPool_Handle();
			
	}

}

bool Loader_IT::load_sample_8bits_IT_compressed(void *p_dest_buffer,int p_buffsize) {

	Sint8 *dest_buffer;		/* destination buffer which will be returned */
   	Uint16 block_length;		/* length of compressed data block in samples */
	Uint16 block_position;		/* position in block */
	Uint8 bit_width;			/* actual "bit width" */
	Uint16 aux_value;			/* value read from file to be processed */
	Sint8 d1, d2;		/* integrator buffers (d2 for it2.15) */
	Sint8 *dest_position;		/* position in output buffer */
	Sint8 v;			/* sample value */
	bool it215; // is this an it215 module?

	dest_buffer = (Sint8 *) p_dest_buffer;

	if (dest_buffer==NULL) 
		return true;

	for (int i=0;i<p_buffsize;i++)
		dest_buffer[i]=0;


	dest_position = dest_buffer;

	it215=(header.cmwt==0x215);

	/* now unpack data till the dest buffer is full */

	while (p_buffsize) {
	/* read a new block of compressed data and reset variables */
		if ( read_IT_compressed_block(false) ) {
			ERR_PRINT("Out of memory decompressing IT Sample");
			return true;
		}


		block_length = (p_buffsize < 0x8000) ? p_buffsize : 0x8000;

		block_position = 0;

		bit_width = 9;		/* start with width of 9 bits */

		d1 = d2 = 0;		/* reset integrator buffers */

	/* now uncompress the data block */
		while ( block_position < block_length ) {

			aux_value = read_n_bits_from_IT_compressed_block(bit_width);			/* read bits */

			if ( bit_width < 7 ) { /* method 1 (1-6 bits) */

				if ( aux_value == (1 << (bit_width - 1)) ) { /* check for "100..." */

					aux_value = read_n_bits_from_IT_compressed_block(3) + 1; /* yes -> read new width; */
		    			bit_width = (aux_value < bit_width) ? aux_value : aux_value + 1;
							/* and expand it */
		    			continue; /* ... next value */
				}

			} else if ( bit_width < 9 ) { /* method 2 (7-8 bits) */

				Uint8 border = (0xFF >> (9 - bit_width)) - 4;
							/* lower border for width chg */

				if ( aux_value > border && aux_value <= (border + 8) ) {

					aux_value -= border; /* convert width to 1-8 */
					bit_width = (aux_value < bit_width) ? aux_value : aux_value + 1;
							/* and expand it */
		    			continue; /* ... next value */
				}


			} else if ( bit_width == 9 ) { /* method 3 (9 bits) */

				if ( aux_value & 0x100 ) {			/* bit 8 set? */

					bit_width = (aux_value + 1) & 0xff;		/* new width... */
		    			continue;				/* ... and next value */
				}

			} else { /* illegal width, abort */

				
				free_IT_compressed_block();
				ERR_PRINT("Sample has illegal BitWidth ");
				return true;
			}

			/* now expand value to signed byte */
			if ( bit_width < 8 ) {

				Uint8 tmp_shift = 8 - bit_width;

				v=(aux_value << tmp_shift);
				v>>=tmp_shift;

			} else v = (Sint8) aux_value;

			/* integrate upon the sample values */
			d1 += v;
	    		d2 += d1;

			/* ... and store it into the buffer */
			*(dest_position++) = it215 ? d2 : d1;
			block_position++;

		}

		/* now subtract block lenght from total length and go on */
		free_IT_compressed_block();
		p_buffsize -= block_length;
	}


	return false;
}

bool Loader_IT::load_sample_16bits_IT_compressed(void *p_dest_buffer,int p_buffsize) {

	Sint16 *dest_buffer;		/* destination buffer which will be returned */
   	Uint16 block_length;		/* length of compressed data block in samples */
	Uint16 block_position;		/* position in block */
	Uint8 bit_width;			/* actual "bit width" */
	Uint32 aux_value;			/* value read from file to be processed */
	Sint16 d1, d2;		/* integrator buffers (d2 for it2.15) */
	Sint16 *dest_position;		/* position in output buffer */
	Sint16 v;			/* sample value */

	bool it215; // is this an it215 module?

	dest_buffer = (Sint16 *) p_dest_buffer;

	if (dest_buffer==NULL) 
		return true;

	for (int i=0;i<p_buffsize;i++)
		dest_buffer[i]=0;

	dest_position = dest_buffer;

	it215=(header.cmwt==0x215);


	while (p_buffsize) {
	/* read a new block of compressed data and reset variables */
		if ( read_IT_compressed_block(true) ) {

			return true;
		}


		block_length = (p_buffsize < 0x4000) ? p_buffsize : 0x4000;

		block_position = 0;

		bit_width = 17;		/* start with width of 9 bits */

		d1 = d2 = 0;		/* reset integrator buffers */

		while ( block_position < block_length ) {

			aux_value = read_n_bits_from_IT_compressed_block(bit_width);			/* read bits */

			if ( bit_width < 7 ) { /* method 1 (1-6 bits) */

				if ( (signed)aux_value == (1 << (bit_width - 1)) ) { /* check for "100..." */

					aux_value = read_n_bits_from_IT_compressed_block(4) + 1; /* yes -> read new width; */
		    			bit_width = (aux_value < bit_width) ? aux_value : aux_value + 1;
							/* and expand it */
		    			continue; /* ... next value */
				}

			} else if ( bit_width < 17 ) {

				Uint16 border = (0xFFFF >> (17 - bit_width)) - 8;

				if ( (int)aux_value > (int)border && (int)aux_value <= ((int)border + 16) ) {

					aux_value -= border; /* convert width to 1-8 */
					bit_width = (aux_value < bit_width) ? aux_value : aux_value + 1;
							/* and expand it */
		    			continue; /* ... next value */
				}


			} else if ( bit_width == 17 ) {

				if ( aux_value & 0x10000 ) {			/* bit 8 set? */

					bit_width = (aux_value + 1) & 0xff;		/* new width... */
		    			continue;				/* ... and next value */
				}

			} else { /* illegal width, abort */

			 	ERR_PRINT("Sample has illegal BitWidth ");

				free_IT_compressed_block();
			
				return true;
			}

			/* now expand value to signed byte */
			if ( bit_width < 16 ) {

				Uint8 tmp_shift = 16 - bit_width;

				v=(aux_value << tmp_shift);
				v>>=tmp_shift;

			} else v = (Sint16) aux_value;

			/* integrate upon the sample values */
			d1 += v;
	    		d2 += d1;

			/* ... and store it into the buffer */
			*(dest_position++) = it215 ? d2 : d1;
			block_position++;

		}

		/* now subtract block lenght from total length and go on */
		free_IT_compressed_block();
		p_buffsize -= block_length;
	}


	return false;

}



