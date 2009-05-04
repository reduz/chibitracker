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
#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H

#include "config.h"
#include "sample_defs.h"


/**
@author Juan Linietsky
*/


/* abstract base Sample_ID class */

struct Sample_ID {

	void *_private;
	
	bool operator==(const Sample_ID&p_other) const { return _private==p_other._private; }
	bool operator!=(const Sample_ID&p_other) const { return _private!=p_other._private; }
	bool is_null() const { return _private==0; }
	Sample_ID(void *p_private=0) { _private=p_private; };
};


class SampleManager {
	
	static SampleManager * singleton;

public:

	/* get the singleton instance */
	static SampleManager *get_singleton();

	virtual void copy_to(Sample_ID p_from,Sample_ID &p_to); ///< if p_to is null, it gets created

	virtual Sample_ID create(bool p_16bits,bool p_stereo,Sint32 p_len)=0;
	virtual void recreate(Sample_ID p_id,bool p_16bits,bool p_stereo,Sint32 p_len)=0;
	virtual void destroy(Sample_ID p_id)=0;
	virtual bool check(Sample_ID p_id)=0; // return false if invalid

	virtual void set_c5_freq(Sample_ID p_id,Sint32 p_freq)=0;
	virtual void set_loop_begin(Sample_ID p_id,Sint32 p_begin)=0;
	virtual void set_loop_end(Sample_ID p_id,Sint32 p_end)=0;
	virtual void set_loop_type(Sample_ID p_id,Sample_Loop_Type p_type)=0;
	virtual void set_chunk(Sample_ID p_id,Sint32 p_index,void *p_data,int p_data_len)=0;


	virtual Sint32 get_loop_begin(Sample_ID p_id)=0;
	virtual Sint32 get_loop_end(Sample_ID p_id)=0;
	virtual Sample_Loop_Type get_loop_type(Sample_ID p_id)=0;
	virtual Sint32 get_c5_freq(Sample_ID p_id)=0;
	virtual Sint32 get_size(Sample_ID p_id)=0;
	virtual bool is_16bits(Sample_ID p_id)=0;
	virtual bool is_stereo(Sample_ID p_id)=0;
	virtual bool lock_data(Sample_ID p_id)=0;
	virtual void *get_data(Sample_ID p_id)=0; /* WARNING: Not all sample managers 
may be able to implement this, it depends on the mixer in use! */
	virtual Sint16 get_data(Sample_ID p_id, int p_sample, int p_channel=0)=0; /// Does not need locking
	virtual void set_data(Sample_ID p_id, int p_sample, Sint16 p_data,int p_channel=0)=0; /// Does not need locking
	virtual void unlock_data(Sample_ID p_id)=0;

	virtual void get_chunk(Sample_ID p_id,Sint32 p_index,void *p_data,int p_data_len)=0;
	
	SampleManager();
	virtual ~SampleManager(){}

};

#endif
