#ifndef MEMPOOL_WRAPPER_H
#define MEMPOOL_WRAPPER_H

#include "config.h"

class MemPool_Handle {
friend class MemPool_Wrapper;

	Uint32 _check;
	void *_private;
public:
	
	MemPool_Handle(void *p_private=0,Uint32 p_check=0) { _private=p_private; _check=p_check; }
	bool is_null() { return _private==0; }
};


class MemPool_Wrapper {


	static MemPool_Wrapper * singleton;

protected:	
	
	inline void* get_private(MemPool_Handle &p_handle) { return p_handle._private; }
	inline Uint32 get_check(MemPool_Handle &p_handle) { return p_handle._check; }
public:

	static MemPool_Wrapper *get_singleton();

	virtual MemPool_Handle alloc_mem(Uint32 p_amount)=0; //return empty memhandle if failed
	virtual bool is_mem_valid(MemPool_Handle p_handle)=0; /* check if memory is valid, true if valid */
	virtual void free_mem(MemPool_Handle p_handle)=0;
	virtual bool realloc_mem(MemPool_Handle p_handle,Uint32 p_new_amount)=0; /* true on error */
	virtual int get_mem_size(MemPool_Handle p_handle)=0;
	
	virtual bool lock_mem(MemPool_Handle p_handle)=0; /* try to lock the mem */
	virtual void *get_mem(MemPool_Handle p_handle)=0;
	virtual void unlock_mem(MemPool_Handle p_handle)=0;

	virtual Uint32 get_total_mem_used()=0;	
	
	MemPool_Wrapper();
	virtual ~MemPool_Wrapper() {};

};


#endif





