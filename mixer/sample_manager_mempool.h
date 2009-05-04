


#ifndef SAMPLE_MANAGER_MEMPOOL_H
#define SAMPLE_MANAGER_MEMPOOL_H


#include "mixer/sample_manager.h"
#include "globals/mempool_wrapper.h"


class SampleManager_MemPool : public SampleManager {


	enum {

		MAX_MALLOC_SAMPLES=1024 /* not many, I'm afraid! */
	};

	static SampleManager * singleton;

	struct Sample_ID_MemPool {

		int index;
	};
	
	struct MemPool_Sample {

		struct Loop {
			Sint32 begin;
			Sint32 end;
			Sample_Loop_Type type;
		} loop;

		bool in_use;
		
		bool stereo;
		bool is_16bits;
		Sint32 c5freq;
		Sint32 size;

		MemPool_Handle mem_handle;

		Sample_ID_MemPool id;

		void reset();
		MemPool_Sample();
		~MemPool_Sample();
	};
	

	MemPool_Sample mempool_samples[MAX_MALLOC_SAMPLES];

	/* Sample params are small, they can be pre-allocated */
	
	MemPool_Sample *get_sample(Sample_ID p_id);
	
	void fix_loop_end(Sample_ID p_id); //fix loop end for linear interpolated samples, when looppoint is at the end
	
public:

	/* get the singleton instance */
	static SampleManager *get_singleton();


	Sample_ID create(bool p_16bits,bool p_stereo,Sint32 p_len);
	void recreate(Sample_ID p_id,bool p_16bits,bool p_stereo,Sint32 p_len);
		
	void destroy(Sample_ID p_id);
	bool check(Sample_ID p_id); // return false if invalid

	void set_loop_begin(Sample_ID p_id,Sint32 p_begin);
	void set_loop_end(Sample_ID p_id,Sint32 p_end);
	void set_loop_type(Sample_ID p_id,Sample_Loop_Type p_type);
	void set_c5_freq(Sample_ID p_id,Sint32 p_freq);
	void set_chunk(Sample_ID p_id,Sint32 p_index,void *p_data,int p_data_len);


	Sint32 get_loop_begin(Sample_ID p_id);
	Sint32 get_loop_end(Sample_ID p_id);
	Sample_Loop_Type get_loop_type(Sample_ID p_id);
	Sint32 get_c5_freq(Sample_ID p_id);
	Sint32 get_size(Sample_ID p_id);
	bool is_16bits(Sample_ID p_id);
	bool is_stereo(Sample_ID p_id);
	
	bool lock_data(Sample_ID p_id); /* use get_chunk when available!, return true on failure */
	void *get_data(Sample_ID p_id); /* works here */
	void unlock_data(Sample_ID p_id);
	
	Sint16 get_data(Sample_ID p_id, int p_sample, int p_channel=0); /// does not need locking
	void set_data(Sample_ID p_id, int p_sample, Sint16 p_data,int p_channel=0); /// does not need locking
	
	void get_chunk(Sample_ID p_id,Sint32 p_index,void *p_data,int p_data_len);


	SampleManager_MemPool();
	~SampleManager_MemPool();

};











#endif



