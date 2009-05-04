//
// C++ Interface: sample_manager
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "sample_manager.h"
#include "globals/error_macros.h"


SampleManager * SampleManager::singleton=NULL;


void SampleManager::copy_to(Sample_ID p_from,Sample_ID &p_to) {
	
	ERR_FAIL_COND(!check( p_from ));

	
	if (p_to.is_null()) {
		
		p_to=create( is_16bits( p_from), is_stereo( p_from), get_size(p_from));
	} else {
		
		recreate( p_to, is_16bits( p_from), is_stereo( p_from), get_size(p_from));
		
	}
	
	int len=get_size( p_from );
	int ch=is_stereo( p_from ) ? 2 : 1;
	
	for (int c=0;c<ch;c++) {
		
		for (int i=0;i<len;i++) {
			
			Sint16 s=get_data( p_from, i, c );			
			set_data( p_to, i, s, c );
		}
	}
	
	set_loop_type( p_to, get_loop_type( p_from ) );
	set_loop_begin( p_to, get_loop_begin( p_from ) );
	set_loop_end( p_to, get_loop_end( p_from ) );
	set_c5_freq( p_to, get_c5_freq( p_from ) );
	
		
	
}

SampleManager::SampleManager() {

	singleton=this;
}

SampleManager *SampleManager::get_singleton() {

	return singleton;
}
