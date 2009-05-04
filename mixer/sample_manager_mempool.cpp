


#include "sample_manager_mempool.h"
#include "audio_lock.h"
#include "error_macros.h"
#include "stdio.h"

SampleManager_MemPool::MemPool_Sample *SampleManager_MemPool::get_sample(Sample_ID p_id) {

 	Sample_ID_MemPool * id_m=(Sample_ID_MemPool*)p_id._private;
	
	ERR_FAIL_COND_V(!id_m,NULL);

	ERR_FAIL_COND_V( id_m->index<0 || id_m->index>=MAX_MALLOC_SAMPLES, NULL );

	ERR_FAIL_COND_V( !mempool_samples[id_m->index].in_use , NULL);

	return &mempool_samples[id_m->index];

}
	
	

void SampleManager_MemPool::MemPool_Sample::reset() {


	in_use=false;
	
	loop.begin=0;
	loop.end=0;
	loop.type=LOOP_NONE;
	c5freq=44100; /* most common default */
	size=0;
	is_16bits=false;

	if (MemPool_Wrapper::get_singleton() && MemPool_Wrapper::get_singleton()->is_mem_valid(mem_handle)) /* has something, free it */
		MemPool_Wrapper::get_singleton()->free_mem(mem_handle);
}


SampleManager_MemPool::MemPool_Sample::MemPool_Sample() {


	reset();
	
}

SampleManager_MemPool::MemPool_Sample::~MemPool_Sample() {


	reset();
}


void SampleManager_MemPool::recreate(Sample_ID p_id,bool p_16bits,bool p_stereo,Sint32 p_len) {
	
	ERR_FAIL_COND(!check( p_id ));
	
	
	Sample_ID_MemPool * id_m=(Sample_ID_MemPool*)p_id._private;
	
	MemPool_Sample *samp=&mempool_samples[id_m->index];
	
	ERR_FAIL_COND(!id_m);
	
	int which=-1;
	for (int i=0;i<MAX_MALLOC_SAMPLES;i++)
		if (&mempool_samples[i]==samp)
			which=i;
	
	ERR_FAIL_COND(which==-1);
	
	AudioLock::begin();
	
	samp->reset(); //destroy and free mem
	
	samp->is_16bits=p_16bits;
	samp->stereo=p_stereo;
	samp->size=p_len;
	Uint32 mem_size=(p_len+1)*(p_16bits?2:1)*(p_stereo?2:1); //extra sample for interpolation
	
	samp->mem_handle = MemPool_Wrapper::get_singleton()->alloc_mem(mem_size);

	if (samp->mem_handle.is_null()) {
		
		mempool_samples[which].in_use=false;		
		AudioLock::end();
		ERR_FAIL_COND( samp->mem_handle.is_null());
	
		return;
	}
	

	samp->id.index=which; /* kind of redundant but.. well.. */

	mempool_samples[which].in_use=true;

	
	AudioLock::end();
	
	fix_loop_end( p_id );
	
	
}


/* quite complicated thing.. but works! */
Sample_ID SampleManager_MemPool::create(bool p_16bits,bool p_stereo,Sint32 p_len) {

	int empty_index=-1;

	/* search for empty ID */
	for (int i=0;i<MAX_MALLOC_SAMPLES;i++) {
		if (!mempool_samples[i].in_use) { /* empty! can use.. */
		empty_index=i;
		break;
		}
	}
	
	ERR_FAIL_COND_V( empty_index==-1, Sample_ID() );

	MemPool_Sample *new_samp=&mempool_samples[empty_index];

	/* allocate sample space */
	new_samp->is_16bits=p_16bits;
	new_samp->stereo=p_stereo;
	new_samp->size=p_len;
	Uint32 mem_size=(p_len+1)*(p_16bits?2:1)*(p_stereo?2:1); //one extra sample, for interpolation
	
	
	
	new_samp->mem_handle = MemPool_Wrapper::get_singleton()->alloc_mem(mem_size);

	ERR_FAIL_COND_V( new_samp->mem_handle.is_null() , Sample_ID() );
	
	new_samp->id.index=empty_index; /* kind of redundant but.. well.. */

	mempool_samples[empty_index].in_use=true;

	fix_loop_end( Sample_ID(&new_samp->id) );
	
	return Sample_ID(&new_samp->id);
	
}

void SampleManager_MemPool::destroy(Sample_ID p_id) {

	MemPool_Sample *smp;
	smp=get_sample(p_id);
	ERR_FAIL_COND(!smp);
	int which=-1;
	for (int i=0;i<MAX_MALLOC_SAMPLES;i++)
		if (&mempool_samples[i]==smp)
			which=i;

	ERR_FAIL_COND( which==-1 );
	
	AudioLock::begin();
		mempool_samples[which].reset();
	AudioLock::end();

}

bool SampleManager_MemPool::check(Sample_ID p_id) {

	Sample_ID_MemPool * id_m=(Sample_ID_MemPool*)p_id._private;
	
	if (!id_m)
		return false;
	

	if (id_m->index<0 || id_m->index>=MAX_MALLOC_SAMPLES)
		return false;

	if (!mempool_samples[id_m->index].in_use) 
		return false;
		
	return true;
}


void SampleManager_MemPool::fix_loop_end(Sample_ID p_id) {
	/* Set the last frame to the begining of the loop, so interpolation works well */
	int ch=is_stereo( p_id) ?2:1;
	if (get_loop_type( p_id)!=LOOP_NONE && get_loop_end( p_id)==get_size( p_id) ) {
		
		
		if (get_loop_type( p_id)==LOOP_FORWARD) {
		
			for (int i=0;i<ch;i++) {
				
				set_data( p_id, get_size( p_id), get_data( p_id,  get_loop_begin( p_id) + 1, i ), i );
			}
		} else {
			
			for (int i=0;i<ch;i++) {
				
				set_data( p_id, get_size( p_id), get_data( p_id,  get_loop_end( p_id) -1 , i ), i );
			}
			
			
		}

	} else {
		
		for (int i=0;i<ch;i++) {
			
			set_data( p_id, get_size( p_id), 0 , i );
		}
		
	}
}

void SampleManager_MemPool::set_loop_begin(Sample_ID p_id,Sint32 p_begin) {

	MemPool_Sample *smp;
	smp=get_sample(p_id);
	ERR_FAIL_COND(!smp);
	
	if (p_begin>smp->size)
		p_begin=smp->size;
	if (p_begin<0)
		p_begin=0;
	
	smp->loop.begin=p_begin;
	
	fix_loop_end( p_id );
}

void SampleManager_MemPool::set_loop_end(Sample_ID p_id,Sint32 p_end) {

	MemPool_Sample *smp;
	smp=get_sample(p_id);
	ERR_FAIL_COND(!smp);
	
	if (p_end>smp->size)
		p_end=smp->size;
	if (p_end<0)
		p_end=0;
	
	smp->loop.end=p_end;
	
	fix_loop_end( p_id );
}

void SampleManager_MemPool::set_loop_type(Sample_ID p_id,Sample_Loop_Type p_type) {

	MemPool_Sample *smp;
	smp=get_sample(p_id);
	ERR_FAIL_COND(!smp);
	
	smp->loop.type=p_type;
	fix_loop_end( p_id );

}

void SampleManager_MemPool::set_c5_freq(Sample_ID p_id,Sint32 p_freq) {

	MemPool_Sample *smp;
	smp=get_sample(p_id);
	ERR_FAIL_COND(!smp);

	if (p_freq<0)
		p_freq=0;	
	smp->c5freq=p_freq;

}


void SampleManager_MemPool::set_chunk(Sample_ID p_id,Sint32 p_index,void *p_data,int p_data_len) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return;
	
	if (p_index<0)
		return;

	if (p_index+p_data_len>smp->size) /* invalid size, adjust */
		p_data_len=smp->size-p_index;
	
	if (p_data_len<=0)
		return; /* just plain invalid size */

	if (MemPool_Wrapper::get_singleton()->lock_mem(smp->mem_handle))
		return;
	
	void *data=MemPool_Wrapper::get_singleton()->get_mem(smp->mem_handle);
	if (!data) {
		MemPool_Wrapper::get_singleton()->unlock_mem(smp->mem_handle);
		
		ERR_FAIL_COND(!data);
	}
	int samples_to_copy=p_data_len*(smp->stereo?2:1);
	if (smp->is_16bits) {
		Frame16 *src=(Frame16*)p_data;
		Frame16 *dst=&((Frame16*)data)[p_index];
		for (int i=0;i<samples_to_copy;i++)
			dst[i]=src[i];
	} else {

		Frame8 *src=(Frame8*)p_data;
		Frame8 *dst=&((Frame8*)data)[p_index];
		for (int i=0;i<samples_to_copy;i++)
			dst[i]=src[i];

	}

	MemPool_Wrapper::get_singleton()->unlock_mem(smp->mem_handle);
	
}


Sint32 SampleManager_MemPool::get_loop_begin(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return -1;

	return smp->loop.begin;


}

Sint32 SampleManager_MemPool::get_loop_end(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return -1;

	return smp->loop.end;


}

Sample_Loop_Type SampleManager_MemPool::get_loop_type(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return LOOP_NONE;

	return smp->loop.type;

}

Sint32 SampleManager_MemPool::get_c5_freq(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return 0;

	return smp->c5freq;
}

Sint32  SampleManager_MemPool::get_size(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return 0;

	return smp->size;
	

}

bool SampleManager_MemPool::is_16bits(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return false;

	return smp->is_16bits;
}

bool SampleManager_MemPool::is_stereo(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return false;

	return smp->stereo;
}

bool SampleManager_MemPool::lock_data(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return true;

	return MemPool_Wrapper::get_singleton()->lock_mem(smp->mem_handle);
}

void SampleManager_MemPool::unlock_data(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return ;

	MemPool_Wrapper::get_singleton()->unlock_mem(smp->mem_handle);
}

void *SampleManager_MemPool::get_data(Sample_ID p_id) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return NULL;

	return MemPool_Wrapper::get_singleton()->get_mem(smp->mem_handle);
}

Sint16 SampleManager_MemPool::get_data(Sample_ID p_id, int p_sample, int p_channel) {
	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return 0;
	
	ERR_FAIL_INDEX_V(p_sample,smp->size,0);
	ERR_FAIL_COND_V(p_channel<0,0);
	
	if (smp->stereo) {
		
		ERR_FAIL_COND_V(p_channel>1,0);
	} else {
		
		ERR_FAIL_COND_V(p_channel>0,0);
	}
	

	if (lock_data(p_id)) {
		return 0;
	}
	
	Sint16 res;
	
	if (smp->is_16bits) {
		
		Sint16 *ptr=(Sint16*)get_data( p_id );
		
		if (ptr==0) {
			unlock_data(p_id);
			ERR_FAIL_COND_V(ptr==0,0);
		}
		
		if (smp->stereo)
			p_sample*=2;
		p_sample+=p_channel;
		
		res=ptr[p_sample];
		
		
	} else {
		
		Sint8 *ptr=(Sint8*)get_data( p_id );
		
		if (ptr==0) {
			unlock_data(p_id);
			ERR_FAIL_COND_V(ptr==0,0);
		}
		
		if (smp->stereo)
			p_sample*=2;
		p_sample+=p_channel;
		
		res=ptr[p_sample];
		
		res<<=8;
	}
	unlock_data( p_id );
	

	return res;
}
void SampleManager_MemPool::set_data(Sample_ID p_id, int p_sample, Sint16 p_data,int p_channel) {
	
	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return;
	
	ERR_FAIL_INDEX(p_sample,(smp->size+1));
	ERR_FAIL_COND(p_channel<0);
	
	if (smp->stereo) {
		
		ERR_FAIL_COND(p_channel>1);
	} else {
		
		ERR_FAIL_COND(p_channel>0);
	}
	
	AudioLock::begin();
	if (lock_data(p_id)) {
		
		AudioLock::end();
		return ;
	}
	
	
	if (smp->is_16bits) {
		
		Sint16 *ptr=(Sint16*)get_data( p_id );
		
		if (ptr==0) {
			unlock_data(p_id);
			AudioLock::end();
			ERR_FAIL_COND(ptr==0);
		}
		
		if (smp->stereo)
			p_sample*=2;
		p_sample+=p_channel;
		
		ptr[p_sample]=p_data;
		
		
	} else {
		
		Sint8 *ptr=(Sint8*)get_data( p_id );
		
		if (ptr==0) {
			unlock_data(p_id);
			AudioLock::end();
			ERR_FAIL_COND(ptr==0);
		}
		
		if (smp->stereo)
			p_sample*=2;
		p_sample+=p_channel;
		
		ptr[p_sample]=p_data>>8;

	}
	
	unlock_data( p_id );
	AudioLock::end();
	
	if (p_sample==(smp->size-1) ||  (smp->loop.type!=LOOP_NONE && p_sample==smp->loop.begin && smp->loop.begin<(smp->size-1)))
		fix_loop_end( p_id );
}

void SampleManager_MemPool::get_chunk(Sample_ID p_id,Sint32 p_index,void *p_data,int p_data_len) {

	MemPool_Sample *smp;
	if (!(smp=get_sample(p_id)))
		return;

	if (p_index<0)
		return;
	if (p_index+p_data_len>smp->size) /* invalid size, adjust */
		p_data_len=smp->size-p_index;
	
	if (p_data_len<=0)
		return; /* just plain invalid size */

	if (MemPool_Wrapper::get_singleton()->lock_mem(smp->mem_handle))
		return;
	
	void *data=MemPool_Wrapper::get_singleton()->get_mem(smp->mem_handle);
	
	if (smp->is_16bits) {
		Frame16 *dst=(Frame16*)p_data;
		Frame16 *src=&((Frame16*)data)[p_index];
		for (int i=0;i<p_data_len;i++)
		dst[i]=src[i];
	} else {

		Frame8 *dst=(Frame8*)p_data;
		Frame8 *src=&((Frame8*)data)[p_index];
		for (int i=0;i<p_data_len;i++)
		dst[i]=src[i];
	}

	MemPool_Wrapper::get_singleton()->unlock_mem(smp->mem_handle);
	

}


SampleManager_MemPool::SampleManager_MemPool() {


}


SampleManager_MemPool::~SampleManager_MemPool() {


}

