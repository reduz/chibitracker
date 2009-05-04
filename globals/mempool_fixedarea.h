//
// C++ Interface: mempool_fixedarea
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MEMPOOL_FIXEDAREA_H
#define MEMPOOL_FIXEDAREA_H

#include "mempool_wrapper.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class MemPool_FixedArea : public MemPool_Wrapper{

	enum {
		
		MAX_CHUNKS=1024
	};
	
	typedef Uint32 ChunkMemPos;
	typedef Uint32 ChunkMapPos;
	
	struct MemChunk {

		Uint16 lock;
		Uint32 pos; //position in memory
		Uint32 len; //lenght, 0 means unused block
		Uint16 check;
		inline void clear() { pos=0;lock=0;len=0;check=0;  }
		inline Uint32 end() { return pos+len; }
	};


	MemChunk mem_chunks[MAX_CHUNKS];
	ChunkMapPos chunk_map[MAX_CHUNKS]; //list that puts the MemChunks in order, so searching for memholes is easy
	Uint32 chunks_allocated;
	
	void *mem_area;
	Uint32 mem_area_size;
	
	//=-=-
	
	Uint32 check_count;

	/* Internal Functions */
	
	bool get_free_chunk_struct(ChunkMemPos* p_pos);
	bool find_hole(ChunkMapPos *p_pos, Uint32 p_for_size);
	bool find_chunk_map(ChunkMapPos *p_map_pos,MemChunk* p_chunk);
	void compact();

public:
	

	MemPool_Handle alloc_mem(Uint32 p_amount); //return empty memhandle if failed
	bool is_mem_valid(MemPool_Handle p_handle); /* check if memory is valid, true if valid */
	void free_mem(MemPool_Handle p_handle);
	bool realloc_mem(MemPool_Handle p_handle,Uint32 p_new_amount); /* true on error */
	int get_mem_size(MemPool_Handle p_handle);
	bool lock_mem(MemPool_Handle p_handle); /* try to lock the mem */
	void *get_mem(MemPool_Handle p_handle);
	void unlock_mem(MemPool_Handle p_handle);

	Uint32 get_total_mem_used();	
	
	
	MemPool_FixedArea(void *p_area,Uint32 p_area_size);
	~MemPool_FixedArea();

};

#endif
