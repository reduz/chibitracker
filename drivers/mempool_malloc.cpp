#include "mempool_malloc.h"

#ifdef ANSIC_LIBS_ENABLED

#include <stdlib.h>

#include <stdio.h>

#include "error_macros.h"

MemPool_Handle MemPool_Malloc::alloc_mem(Uint32 p_amount) {

	int free_idx=-1;
	for (int i=0;i<MAX_ALLOCS;i++) {
		
		if (alloc_items[i].ptr==NULL) {
			
			free_idx=i;
			break;
		}
	}
		
	ERR_FAIL_COND_V( free_idx==-1 , MemPool_Handle() );
	
	
	alloc_items[free_idx].ptr=calloc(p_amount,1);
	if (alloc_items[free_idx].ptr==NULL)
		return MemPool_Handle(); //out of memory
	
	alloc_items[free_idx].len=p_amount;
	alloc_items[free_idx].check=check_incr++;
	mem_usage+=p_amount;
	
	return MemPool_Handle(&alloc_items[free_idx],alloc_items[free_idx].check);

}
bool MemPool_Malloc::is_mem_valid(MemPool_Handle p_handle) {

	if (p_handle.is_null()) {
		
		//printf("ERR: handle is null\n");
		return false;
	}
	
	AllocItem *alloc_item=(AllocItem*)get_private( p_handle );
	
	
	
	/* If not in range of the pool, it's not valid pointer inside */
	if (alloc_item<&alloc_items[0] || alloc_item>&alloc_items[MAX_ALLOCS-1]) {
		
		//printf("ERR: handle out of range\n");

		return false;
	}
	
	/* Mem has been freed */	
	if (alloc_item->ptr==NULL) {
		
		//printf("ERR: mem has been freed\n");

		return false;
	}
	
	/* Check if the item "check" integer is the same */
	if (alloc_item->check!=get_check( p_handle )) {
		
		//printf("ERR: check is invalid\n");

		return false;
	}
	
	return true;

}
void MemPool_Malloc::free_mem(MemPool_Handle p_handle) {

	ERR_FAIL_COND( !is_mem_valid(p_handle) );
	
	AllocItem *alloc_item=(AllocItem*)get_private( p_handle );
	
	
	mem_usage-=alloc_item->len;
	
	free(alloc_item->ptr);
	alloc_item->ptr=NULL;
	alloc_item->check=0;
	alloc_item->len=0;
	

}

int MemPool_Malloc::get_mem_size(MemPool_Handle p_handle) {
	
	ERR_FAIL_COND_V( !is_mem_valid(p_handle), -1 );
	AllocItem *alloc_item=(AllocItem*)get_private( p_handle );
	return alloc_item->len;	
}
bool MemPool_Malloc::realloc_mem(MemPool_Handle p_handle,Uint32 p_new_amount) {

	ERR_FAIL_COND_V( !is_mem_valid(p_handle), true );
	
	AllocItem *alloc_item=(AllocItem*)get_private( p_handle );
	
	/* Cant be locked when realloc-ing */
	
	if (alloc_item->len==p_new_amount)
		return false;
	
	
	void *new_mem=realloc(alloc_item->ptr,p_new_amount);
	
	mem_usage-=alloc_item->len;
	if (new_mem) {
		alloc_item->ptr=new_mem;
		alloc_item->len=p_new_amount;
		mem_usage+=alloc_item->len;
		
		return false;
	} else
		return true;
}

bool MemPool_Malloc::lock_mem(MemPool_Handle p_handle) {

	ERR_FAIL_COND_V( !is_mem_valid(p_handle), true );
	
	return false;
}

void *MemPool_Malloc::get_mem(MemPool_Handle p_handle) {

	ERR_FAIL_COND_V( !is_mem_valid(p_handle), NULL );
	
	AllocItem *alloc_item=(AllocItem*)get_private( p_handle );
	
	return alloc_item->ptr;
}

void MemPool_Malloc::unlock_mem(MemPool_Handle p_handle) {

	ERR_FAIL_COND( !is_mem_valid(p_handle) );
	
	return; 
	
}

Uint32 MemPool_Malloc::get_total_mem_used() {
	
	return mem_usage;
	/*
	Uint32 total=0;
	for (int i=0;i<MAX_ALLOCS;i++) {
		
		if (alloc_items[i].ptr)
			total+=alloc_items[i].len;
	}
    
	return total;	*/
	
}

MemPool_Malloc::MemPool_Malloc() {
	
	check_incr=1;
	mem_usage=0;
}

#endif
