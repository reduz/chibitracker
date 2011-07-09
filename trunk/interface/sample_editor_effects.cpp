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
//
//
// C++ Implementation: sample_editor_effects
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "sample_editor_effects.h"
#include "mixer/audio_lock.h"
#include <stdio.h>
#include <math.h>
int SampleEditorEffects::get_begin() {

	if (selection->is_active())
		return selection->get_begin();
	
	
	return 0;
	
}
int SampleEditorEffects::get_end() {

	if (selection->is_active())
		return selection->get_end();
	
	
	return sm->get_size( sample )-1;

}

int SampleEditorEffects::get_size() {
	
	return get_end()-get_begin();
}
void SampleEditorEffects::command(int p_command,int p_parameter) {

	if (sample.is_null())
		return;
	if (!sm->check( sample ))
		return;
	if (get_size()==0) //error?
		return;
	int ch=sm->is_stereo( sample )?2:1;
	AudioLock::begin();
	
	int resample_insert_samples=0;
	switch (p_command) {
		
		case CMD_COPY: {
			
			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;
			
			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;
			
			/* create clipboard */
			if (clipboard.is_null())
				clipboard=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), selection->get_size() );
			else
				sm->recreate( clipboard, sm->is_16bits( sample ), sm->is_stereo( sample ), selection->get_size() );

			/* copy to selection */
			for (int c=0;c<ch;c++) {

				for (int i=0;i<selection->get_size();i++) {

					sm->set_data( clipboard, i, sm->get_data( sample, selection->get_begin()+i, c ), c);
				}
			}
			
		} break;
		case CMD_CUT: {
			
			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;
			
			/* create clipboard */
			if (clipboard.is_null())
				clipboard=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), selection->get_size() );
			else
				sm->recreate( clipboard, sm->is_16bits( sample ), sm->is_stereo( sample ), selection->get_size() );

			/* copy to selection */
			for (int c=0;c<ch;c++) {

				for (int i=0;i<selection->get_size();i++) {

					sm->set_data( clipboard, i, sm->get_data( sample, selection->get_begin()+i, c ), c);
				}
			}

			/* delete selection */

			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size(sample)-selection->get_size() );
			if (aux.is_null())
				break;
			

			for (int c=0;c<ch;c++) {

				for (int i=0;i<selection->get_begin();i++) {

					sm->set_data( aux, i, sm->get_data( sample, i, c ), c);

				}
			}
			
			for (int c=0;c<ch;c++) {

				for (int i=0;i<(sm->get_size(sample)-selection->get_end());i++) {

					sm->set_data( aux, selection->get_begin()+i, sm->get_data( sample, i+selection->get_end(), c ), c);

				}
			}

			/* fix the loop (what a mess) */
			


			if (sm->get_loop_type( sample )!=LOOP_NONE ) {

				int new_begin=0;
				int new_end=0;
				bool keep_loop=true;

				if (sm->get_loop_begin( sample ) >= selection->get_begin() ) {


					if (sm->get_loop_begin( sample ) >= selection->get_end() ) {

						new_begin=sm->get_loop_begin( sample )-selection->get_size();
					} else {

						keep_loop=false;
					}

				} else {

					new_begin=sm->get_loop_begin( sample );
				}

				if (keep_loop) {

					if (sm->get_loop_end( sample ) >= selection->get_begin() ) {


						if (sm->get_loop_end( sample ) >= selection->get_end() ) {

							new_end=sm->get_loop_end( sample )-selection->get_size();
						} else {

							keep_loop=false;
						}

					} else {

						new_end=sm->get_loop_end( sample );
					}
				}

				if (keep_loop) {

					sm->set_loop_type( aux, sm->get_loop_type( sample ) );
					sm->set_loop_begin( aux, new_begin );
					sm->set_loop_end( aux, new_end );

				}
			}
			
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
		} break;
		case CMD_PASTE: {
			
			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;

			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;
			
			/* create clipboard */
			if (clipboard.is_null())
				break;


			/* copy to selection */
			for (int c=0;c<ch;c++) {

				for (int i=0;i<selection->get_size();i++) {

					sm->set_data( clipboard, i, sm->get_data( sample, selection->get_begin()+i, c ), c);
				}
			}

			/* delete selection */

			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size(sample)+sm->get_size(clipboard) );
			if (aux.is_null())
				break;


			/* Sample Begining */
			for (int c=0;c<ch;c++) {

				for (int i=0;i<selection->get_end();i++) {

					sm->set_data( aux, i, sm->get_data( sample, i, c ), c);

				}
			}
			
			/* Clipboard */
			for (int c=0;c<ch;c++) {

				int baseofs=selection->get_end();
				for (int i=0;i<sm->get_size(clipboard);i++) {

					sm->set_data( aux, i+baseofs, sm->get_data( clipboard, i, c ), c);

				}
			}
			
			/* Sample End */
			for (int c=0;c<ch;c++) {

				int baseofs=selection->get_end()+sm->get_size(clipboard);
				int size=sm->get_size(sample)-selection->get_end();
				
				for (int i=0;i<size;i++) {

					sm->set_data( aux, i+baseofs, sm->get_data( sample, i+selection->get_end(), c ), c);

				}
			}
			
			/* fixl oop */
			
			if (sm->get_loop_type( sample) != LOOP_NONE ) {

				if ( sm->get_loop_begin(sample) >get_end() )
					sm->set_loop_begin( aux, sm->get_loop_begin( sample )+sm->get_size(clipboard) );
				
				else
					sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
					
				if ( sm->get_loop_end(sample) >get_end() )
					sm->set_loop_end( aux, sm->get_loop_end( sample )+sm->get_size(clipboard) );
				
				else
					sm->set_loop_end( aux, sm->get_loop_end( sample ) );
				
				
				sm->set_loop_type( aux, sm->get_loop_type( sample) );
			}			
			
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			
		} break;
		case CMD_TRIM: {

			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;
			
			if (!selection->is_active())
				break;
			if (sample.is_null())
				break;

			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), selection->get_size() );
			if (aux.is_null())
				break;

			
			/* copy to aux */
			for (int c=0;c<ch;c++) {

				for (int i=0;i<selection->get_size();i++) {

					sm->set_data( aux, i, sm->get_data( sample, selection->get_begin()+i, c ), c);
				}
			}

			if (sm->get_loop_type( sample) != LOOP_NONE ) {

				
				if (sm->get_loop_begin(sample)>=get_begin() && sm->get_loop_begin(sample)<get_end() && sm->get_loop_end(sample)>=get_begin() && sm->get_loop_end(sample)<get_end()) {
					
					sm->set_loop_type( aux, sm->get_loop_type( sample) );
					sm->set_loop_begin( aux, sm->get_loop_begin( sample )-get_begin() );
					sm->set_loop_end( aux, sm->get_loop_end( sample )-get_begin() );
					
					
				}
			}
			
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
			
		} break;
		case CMD_TOGGLE_SIGN: {
			
			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size(sample) );
			if (aux.is_null())
				break;


			for (int c=0;c<ch;c++) {

				for (int i=0;i<sm->get_size( sample );i++) {

					sm->set_data( aux, i, (sm->get_data( sample, i, c )+32768), c);

				}
			}
			
			sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
			sm->set_loop_end( aux, sm->get_loop_end( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			
		} break;
		case CMD_TOGGLE_DEPTH: {
			
			Sample_ID aux=sm->create( !sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size(sample) );
			if (aux.is_null())
				break;


			for (int c=0;c<ch;c++) {

				for (int i=0;i<sm->get_size( sample );i++) {

					sm->set_data( aux, i, sm->get_data( sample, i, c ), c);

				}
			}
			
			sm->set_loop_type( aux, sm->get_loop_type( sample ) );
			sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
			sm->set_loop_end( aux, sm->get_loop_end( sample ) );
			
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
				
		} break;
		case CMD_ADD_SILENCE_AT_END: {
			
			if (p_parameter<1)
				break;
			Sample_ID aux=sm->create( !sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size(sample)+p_parameter );
			if (aux.is_null())
				break;

			

			for (int c=0;c<ch;c++) {

				for (int i=0;i<sm->get_size( sample );i++) {

					sm->set_data( aux, i, sm->get_data( sample, i, c ), c);

				}
				for (int i=0;i<p_parameter;i++) {

					sm->set_data( aux, sm->get_size( sample )+i, 0, c);

				}
			}
			
			sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
			sm->set_loop_end( aux, sm->get_loop_end( sample ) );
			
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
				
		} break;
		case CMD_TOGGLE_STEREO: {
			
			Sample_ID aux=sm->create( sm->is_16bits( sample ), !sm->is_stereo( sample ), sm->get_size(sample) );
			if (aux.is_null())
				break;


			if (sm->is_stereo( sample )) {


				for (int i=0;i<sm->get_size( sample );i++) {

					Sint32 v=sm->get_data( sample, i, 0 );
					v+=sm->get_data( sample, i, 1 );
					
					sm->set_data( aux, i, Sint16(v/2),0 );

				}
			} else {

				for (int i=0;i<sm->get_size( sample );i++) {

					Sint16 v=sm->get_data( sample, i, 0 );
					sm->set_data( aux, i, v,0 );
					sm->set_data( aux, i, v,1 );

				}


			}
			sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
			sm->set_loop_end( aux, sm->get_loop_end( sample ) );
			
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			
		} break;
		case CMD_AMPLIFY: {
			
			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()+1);i++) {
					
					Sint32 samp=sm->get_data(sample,i,c);

					samp=p_parameter*samp/100;
					if (samp>32767)
						samp=32767;
					if (samp<-32768)
						samp=-32768;
					sm->set_data( sample,i,(Sint16)samp,c );
				}
			}
			
		} break;
		case CMD_PUNCH_ENVELOPE: {

			int len = get_end()+1-get_begin();
			float amp = p_parameter/100.0;
			for (int c=0;c<ch;c++) {

				for (int i=get_begin();i<(get_end()+1);i++) {

					Sint32 samp=sm->get_data(sample,i,c);
					float pos = (i-get_begin())/float(len);

					float lamp = amp*(1.0-pos)+pos;
					samp=samp*lamp;
					if (samp>32767)
						samp=32767;
					if (samp<-32768)
						samp=-32768;
					sm->set_data( sample,i,(Sint16)samp,c );
				}
			}

		} break;
		case CMD_FADE_IN: {
			
			
			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()+1);i++) {
					
					int ofs=i-get_begin();
					Sint32 samp=sm->get_data(sample,i,c);
					samp=ofs*samp/get_size();
					sm->set_data( sample,i,(Sint16)samp,c);
				}
			}

			
		} break;
		case CMD_FADE_OUT: {
			
			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()+1);i++) {
					
					int ofs=i-get_begin();
					Sint32 samp=sm->get_data(sample,i,c);
					samp=(get_size()-ofs)*samp/get_size();
					sm->set_data( sample,i,(Sint16)samp,c);
				}
			}
			
		} break;
		case CMD_SWAP_STEREO_CHANS: {
			
			if (!sm->is_stereo( sample))
				break;
			
			
			for (int i=get_begin();i<(get_end()+1);i++) {
				
				Sint16 samp=sm->get_data(sample,i,CHAN_LEFT);
				sm->set_data(sample,i,sm->get_data(sample,i,CHAN_RIGHT),CHAN_LEFT);
				sm->set_data(sample,i,samp,CHAN_RIGHT);
			}
			
		} break;
		case CMD_LOOP_UNROLL: {
		
			if (sm->get_loop_type( sample )!=LOOP_BIDI)
				break;
				
			int looplen=sm->get_loop_end( sample )-sm->get_loop_begin( sample );
			looplen-=1; // don't repeat the restart
			
			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size( sample)+(looplen) );
			if (aux.is_null())
				break;

						
			for (int c=0;c<ch;c++) {
				
				/* first pass, copy until loop end */
				int size=sm->get_loop_end( sample );
				
				for (int i=0;i<size;i++) {
					
					sm->set_data( aux, i, sm->get_data(sample,i,c) , c);
				}
				
				
				for (int i=size;i<(size+looplen);i++) {
					
					int src=(size-1)-(i-size); 
					sm->set_data( aux, i-1, sm->get_data(sample,src,c) , c);
				}
			}

			sm->set_loop_type( aux, LOOP_FORWARD ); //unrolled
			sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
			sm->set_loop_end( aux, sm->get_loop_end( sample )+(looplen-2));
				
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
			
		
		} break;
		case CMD_CENTER: {
			
			Sint32 max=-32768;
			Sint32 min=32767;
			
			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()+1);i++) {
					
					Sint16 d = sm->get_data(sample,i,c);
					if (d>max)
						max=d;
					if (d<min)
						min=d;
				}
			}

			if (max==32767 || min==-32768)
				break; //pointless
			
			Sint32 range=max-min;
			Sint32 desired_min=(65535-range)/2-32768;

			Sint16 ofs=(Sint16)(desired_min-min);

			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()+1);i++) {
					
					Sint16 d = sm->get_data(sample,i,c);
					sm->set_data(sample,i,d+ofs,c);
				}
			}
			
			
			
		} break;

		case CMD_PRELOOP_CUT: {
			
			if (sm->get_loop_type( sample) == LOOP_NONE )
				break;

			int begin=sm->get_loop_begin( sample );
			int size=sm->get_size( sample );
			if (begin>= sm->get_loop_end( sample ) || begin>=size)
				break;
			if (begin<0)
				break;

			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), size-begin );
			if (aux.is_null())
				break;


			sm->set_loop_type( aux, sm->get_loop_type( sample ) );
			sm->set_loop_begin( aux, 0 );
			sm->set_loop_end( aux, sm->get_loop_end( sample )-begin );
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			
			for (int c=0;c<ch;c++) {
				
				for (int i=begin;i<size;i++) {
					
					Sint16 d = sm->get_data(sample,i,c);
					sm->set_data(aux,i-begin,d,c);
				}
			}

			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
		} break;
		case CMD_POSTLOOP_CUT: {
			
			if (sm->get_loop_type( sample) == LOOP_NONE )
				break;

			int end=sm->get_loop_end( sample );
			if (end<= sm->get_loop_begin( sample ) || end<=1)
				break;
			if ((end+1)>sm->get_size( sample))
				break;

			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), end+1 );
			if (aux.is_null())
				break;


			sm->set_loop_type( aux, sm->get_loop_type( sample ) );
			sm->set_loop_begin( aux, sm->get_loop_begin( sample ) );
			sm->set_loop_end( aux, sm->get_loop_end( sample ) );
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			
			for (int c=0;c<ch;c++) {
				
				for (int i=0;i<(end+1);i++) {
					
					Sint16 d = sm->get_data(sample,i,c);
					sm->set_data(aux,i,d,c);
				}
			}

			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
			
		} break;

		case CMD_REVERSE: {
			
			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()-(get_size()/2));i++) {
					
					int ofs=i-get_begin();
					int pos_inv=get_end()-ofs;
					Sint16 samp=sm->get_data(sample,i,c);
					sm->set_data( sample,i,sm->get_data(sample,pos_inv,c),c);
					sm->set_data( sample,pos_inv,samp,c);
				}
			}
			/*
			int lbegin=sm->get_loop_begin( sample );
			sm->set_loop_begin( sample, sm->get_loop_end( sample) );
			sm->set_loop_end( sample, lbegin ); */
			
		} break;
		case CMD_FIX_LOOPING: {
			
			if (sm->get_loop_type(sample)==LOOP_NONE)
				break;
				
			int blendcount=sm->get_loop_end(sample)-sm->get_loop_begin(sample);
			blendcount=p_parameter*blendcount/100;
			
			if (blendcount>sm->get_loop_begin(sample))
				blendcount=sm->get_loop_begin(sample);
			
			for (int c=0;c<ch;c++) {
				
				for (int i=0;i<blendcount;i++) {
					
					int read_ofs=sm->get_loop_begin(sample)-blendcount+i;
					int write_ofs=sm->get_loop_end(sample)-blendcount+i+1;
					
					float samp_read=sm->get_data(sample,read_ofs,c);
					float samp_write=sm->get_data(sample,write_ofs,c);
					float coeff=(float)i/(float)blendcount;
					
					float final=(1.0-coeff)*samp_write+(coeff)*samp_read;
					
					Sint16 samp_final=(Sint16)final;
					sm->set_data( sample,write_ofs,samp_final,c);
					
				}
			}
			
			
		} break;
		case CMD_RESAMPLE_FREQ: {
			
			int size=sm->get_size(sample);
			
			p_parameter=size*p_parameter/sm->get_c5_freq( sample ); // freq -> frames
			
		} //no brea, go ahead to resample
		case CMD_LOOP_TO_BOUNDARY: {
		
			if (p_command==CMD_LOOP_TO_BOUNDARY) {
			
				int size=sm->get_size(sample);
			
				if (sm->get_loop_type( sample )==LOOP_NONE )
					break; // do nothing
				
				int loop_beg = sm->get_loop_begin( sample );
				int loop_end = sm->get_loop_end( sample );

				int new_size=size;
				int new_loop_beg;
				int new_loop_end;
				
#define MIN_LOOP_TRESHOLD 128				
				if (loop_end-loop_beg < MIN_LOOP_TRESHOLD ) {
					/* temporary measure */
					new_size*=p_parameter;
					new_loop_beg=loop_beg*p_parameter;
					new_loop_end=loop_end*p_parameter;
					
				} else {
				
					while(true) {
					
						
						new_loop_beg = (Uint64)loop_beg * (Uint64)new_size / (Uint64)size;
						new_loop_end = (Uint64)loop_end * (Uint64)new_size / (Uint64)size;
						
						int loop_size = new_loop_end - new_loop_beg;
						
						if ( (loop_size%p_parameter) == 0 )
							break;
						
						new_size++;					
					}
				}
					
			
				resample_insert_samples = ((p_parameter - (new_loop_beg%p_parameter)) % p_parameter);

				if (size==new_size && resample_insert_samples==0)
					break; //nothing to fix
				
				p_parameter=(int)new_size; // skip to next effect
				
				
			}
		};
		
		case CMD_RESAMPLE: {
			
			if (p_parameter<2)
				return;
			
			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), p_parameter+resample_insert_samples );
			if (aux.is_null())
				break;

			int size=sm->get_size(sample);
			
						
			for (int c=0;c<ch;c++) {
				
				Sint16 first = sm->get_data( sample, 0, c);
				Sint16 last = sm->get_data( sample, size-1, c);
				
				for (int i=0;i<p_parameter;i++) {
					
					
					float pos=(float)i*(float)size/(float)p_parameter;
					int posi=(int)pos;
					float mu=pos-(float)posi;

					float y0=(pos-1)<0?first:sm->get_data( sample, posi-1, c);
					float y1=sm->get_data( sample, posi, c);
					float y2=(pos+1)>=size?last:sm->get_data( sample, posi+1, c);
					float y3=(pos+2)>=size?last:sm->get_data( sample, posi+2, c);
					
					float mu2 = mu*mu;
					float a0 = y3 - y2 - y0 + y1;
					float a1 = y0 - y1 - a0;
					float a2 = y2 - y0;
					float a3 = y1;
					
					float res=(a0*mu*mu2+a1*mu2+a2*mu+a3);
					if (res<-32768)
						res=-32768;
					if (res>32767)
						res=32767;
					
					sm->set_data(aux,i+resample_insert_samples,(Sint16)res,c);
				}
				
				for (int i=0;i<resample_insert_samples;i++) {
				
					sm->set_data(aux,i,0,c);
				}

			}

			sm->set_loop_type( aux, sm->get_loop_type( sample ) );
			sm->set_loop_begin( aux, (Uint64)sm->get_loop_begin( sample )*(Uint64)p_parameter/size + resample_insert_samples );
			sm->set_loop_end( aux, (Uint64)sm->get_loop_end( sample )*(Uint64)p_parameter/size + resample_insert_samples);
				
			sm->set_c5_freq( aux, (Uint64)sm->get_c5_freq( sample )*(Uint64)p_parameter/size );
			
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
						
		} break;
		case CMD_SEL_TO_LOOP: {
			
			if (!selection->is_active())
				break;
			
			if (sm->get_loop_type(sample)==LOOP_NONE) {
				
				sm->set_loop_type( sample, LOOP_FORWARD );
										
			}
			
			sm->set_loop_begin( sample, get_begin() );
			sm->set_loop_end( sample, get_end() );

			
		} break;
		case CMD_LEFT_CHAN_DELAY: {
			
			if (!sm->is_stereo( sample))
				break;
			Sample_ID aux=sm->create( sm->is_16bits( sample ), sm->is_stereo( sample ), sm->get_size(sample)+p_parameter );
			if (aux.is_null())
				break;
			

			/* Left */
			for (int i=0;i<sm->get_size(aux);i++) {

				Sint16 d=(i<p_parameter)?0:sm->get_data( sample, i-p_parameter, CHAN_LEFT );
				
				sm->set_data( aux, i,d,CHAN_LEFT);
			}
			
			/* Right */
			for (int i=0;i<sm->get_size(aux);i++) {

				Sint16 d=(i>=sm->get_size(sample))?0:sm->get_data( sample, i, CHAN_RIGHT );
				
				sm->set_data( aux, i,d,CHAN_RIGHT);
			}
			
			/* bye bye loop */
			sm->set_c5_freq( aux, sm->get_c5_freq( sample ) );
			sm->copy_to( aux, sample );
			sm->destroy( aux );
			selection->clear();
			
		} break;

		case CMD_MATCH_LOOP_END_BY_NOTE: {


			float base_freq=(8.1757989156*powf(2.0,p_parameter/12.0));
			if (base_freq==0)
				break;
			int len = sm->get_c5_freq(sample)/base_freq;
			if (len==0)
				break;
			int loop_beg = sm->get_loop_begin(sample);
			int loop_end = sm->get_loop_end(sample);
			loop_end-=(loop_end-loop_beg)%len;
			sm->set_loop_end(sample,loop_end);

		} break;
		
	}

	AudioLock::end();
	
}
bool SampleEditorEffects::command_has_parameter(int p_command) {

	switch(p_command) {
		
		case CMD_AMPLIFY:
		case CMD_PUNCH_ENVELOPE:
		case CMD_FIX_LOOPING:
		case CMD_RESAMPLE:
		case CMD_RESAMPLE_FREQ:
		case CMD_ADD_SILENCE_AT_END:
		case CMD_LOOP_TO_BOUNDARY:
		case CMD_LEFT_CHAN_DELAY:
		case CMD_MATCH_LOOP_END_BY_NOTE:
			return true;
			break;			
	}

	return false;
}
void SampleEditorEffects::command_set_parameter_range(int p_command,int& p_min, int &p_max, int &p_default) {

	switch(p_command) {
		
		case CMD_AMPLIFY: {
			
			int ch=sm->is_stereo( sample )?2:1;
			
			Sint32 max=0;
			
			for (int c=0;c<ch;c++) {
				
				for (int i=get_begin();i<(get_end()+1);i++) {
					
					Sint32 d = sm->get_data(sample,i,c);
					d=(d<0)?-d:d;
					
					if (d>max)
						max=d;
				}
			}
			
			
			Sint32 percent=32768*100/max;
			
			p_default=percent;			
			p_min=0;
			p_max=1000;
	
		} break;
		case CMD_FIX_LOOPING: {
			
			p_min=0;
			p_max=100;
			p_default=10;
			
		} break;
		case CMD_PUNCH_ENVELOPE: {

			p_min=0;
			p_max=800;
			p_default=300;

		} break;
		case CMD_RESAMPLE: {
			
			p_min=0;
			p_max=64*1024*1024; //64mb mxx size..
			p_default=sm->get_size(sample);
		} break;
		case CMD_RESAMPLE_FREQ: {
			
			p_min=1;
			p_max=256000; //64mb mxx size..
			p_default=sm->get_c5_freq( sample );
		} break;
		case CMD_LEFT_CHAN_DELAY: {
			
			p_min=0;
			p_default=0;
			p_max=sm->get_size(sample)*2;
			
		} break;

		case CMD_ADD_SILENCE_AT_END: {
			
			p_min=0;
			p_default=0;
			p_max=1024*1024*5;
			
		} break;
		case CMD_LOOP_TO_BOUNDARY: {
		
			p_min=2;
			p_default=8;
			p_max=32;
			
		} break;
		case CMD_MATCH_LOOP_END_BY_NOTE: {

			p_min=1;
			p_default=60;
			p_max=128;

		} break;
		default: {
			
			p_min=0;
			p_max=1;
			p_default=0;
		}
	}

}
void SampleEditorEffects::set_selection(Selection *p_selection)  {

	selection=p_selection;
}
void SampleEditorEffects::set_sample(Sample_ID p_sample)  {

	sample=p_sample;
	
}

SampleEditorEffects::SampleEditorEffects() {

	sm=SampleManager::get_singleton();
	selection=0;
}
