#ifndef MEMPOOL_MALLOC_H
#define MEMPOOL_MALLOC_H


#ifdef ANSIC_LIBS_ENABLED

#include "globals/mempool_wrapper.h"

class MemPool_Malloc : public MemPool_Wrapper {


	enum {
		
		MAX_ALLOCS=8192
	};
	
	
	struct AllocItem {
		
		void *ptr;
		Uint32 len;
		volatile Uint32 check;
		/*volatile Uint16 lock;*/
		
		AllocItem() { ptr=NULL; len=0; check=0;  }
	};
	
	AllocItem alloc_items[MAX_ALLOCS];

	Uint32 check_incr;
	
	Uint32 mem_usage;
public:

	MemPool_Handle alloc_mem(Uint32 p_amount);
	bool is_mem_valid(MemPool_Handle p_handle); /* check if memory is valid */
	void free_mem(MemPool_Handle p_handle);
	bool realloc_mem(MemPool_Handle p_handle,Uint32 p_new_amount); /* true on error */

	int get_mem_size(MemPool_Handle p_handle);
	bool lock_mem(MemPool_Handle p_handle); /* try to lock the mem */
	void *get_mem(MemPool_Handle p_handle);
	void unlock_mem(MemPool_Handle p_handle);

	Uint32 get_total_mem_used();
	
	MemPool_Malloc();
};



#endif
#endif
