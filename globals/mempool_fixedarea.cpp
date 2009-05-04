//
// C++ Implementation: mempool_fixedarea
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mempool_fixedarea.h"
#include "error_macros.h"
#include "movemem.h"


#define COMPACT_CHUNK( m_chunk , m_to_pos ) 			\
do {								\
	void *_dst=&((unsigned char*)mem_area)[m_to_pos];	\
	void *_src=&((unsigned char*)mem_area)[(m_chunk).pos];	\
	memmove(_dst,_src,(m_chunk).len);			\
	(m_chunk).pos=m_to_pos;					\
} while (0);



bool MemPool_FixedArea::get_free_chunk_struct(ChunkMemPos* p_pos) {

	if (chunks_allocated==MAX_CHUNKS)
		return true;

	for (int i=0;i<MAX_CHUNKS;i++) {

		if (mem_chunks[i].len==0) {
			*p_pos=i;
			return false;
		}
		
	}

	ERR_PRINT("Out of memory Chunks!");

	return true; //

}

/**
 * Find a hole
 * @param p_pos The hole is behind the block pointed by this variable upon return. if pos==chunks_allocated, then allocate at end
 * @param p_for_size hole size
 * @return false if hole found, true if no hole found
 */
bool MemPool_FixedArea::find_hole(ChunkMapPos *p_pos, Uint32 p_for_size) {

	/* position where previous chunk ends. Defaults to zero (begin of pool) */
	
	Uint32 prev_chunk_end_pos=0;

	for (Uint32 i=0;i<chunks_allocated;i++) {


		MemChunk &chunk=mem_chunks[ chunk_map[ i ] ];

		/* determine hole size to previous chunk */

		int hole_size=chunk.pos-prev_chunk_end_pos;

		/* detemine if what we want fits in that hole */
		if (hole_size>=p_for_size) {
			*p_pos=i;
			return false;
		}

		/* prepare for next one */
		prev_chunk_end_pos=chunk.end();
	}

	/* No holes between chunks, check at the end..*/
	
	if ( (mem_area_size-prev_chunk_end_pos)>=p_for_size )  {
		*p_pos=chunks_allocated;
		return false;
	}
	
	return true;
	
}

void MemPool_FixedArea::compact() {
	
	Uint32 prev_chunk_end_pos=0;
	
	for (Uint32 i=0;i<chunks_allocated;i++) {


		MemChunk &chunk=mem_chunks[ chunk_map[ i ] ];

		/* determine hole size to previous chunk */

		int hole_size=chunk.pos-prev_chunk_end_pos;

		/* if we can compact, do it */
		if (hole_size>0 && !chunk.lock) {
			
			COMPACT_CHUNK(chunk,prev_chunk_end_pos);
			
		}
		
		/* prepare for next one */
		prev_chunk_end_pos=chunk.end();
	}
	
	
}

bool MemPool_FixedArea::find_chunk_map(ChunkMapPos *p_map_pos,MemChunk* p_chunk) {
	
	ChunkMapPos chunk_map_pos=MAX_CHUNKS;
	
	for (Uint32 i=0;i<chunks_allocated;i++) {
		
		if (&mem_chunks[ chunk_map[ i ] ]==p_chunk) {
			
			chunk_map_pos=i;
			break;
		}
	}
	
	if (chunk_map_pos==MAX_CHUNKS)
		return true;
	
	*p_map_pos=chunk_map_pos;
	return false;
	
}

MemPool_Handle MemPool_FixedArea::alloc_mem(Uint32 p_amount) {


	ERR_FAIL_COND_V(chunks_allocated==MAX_CHUNKS,MemPool_Handle());
	
	Uint32 size_to_alloc=p_amount;
	
	ChunkMapPos new_chunk_map_pos;
	
	if (find_hole(&new_chunk_map_pos, size_to_alloc)) {
		/* No hole could be found, try compacting mem */
		compact();
		/* Then search again */
		ERR_FAIL_COND_V(find_hole(&new_chunk_map_pos, size_to_alloc),MemPool_Handle()); //cant compact or out of memory
	}
	
	ChunkMemPos new_chunk_mem_pos;
	
	ERR_FAIL_COND_V( get_free_chunk_struct(&new_chunk_mem_pos) , MemPool_Handle() );
	
	for (Sint32 i=chunks_allocated;i>new_chunk_map_pos;i++) {
		
		chunk_map[i]=chunk_map[i-1];
	}
	
	chunk_map[new_chunk_map_pos]=new_chunk_mem_pos;
	
	chunks_allocated++;
	
	MemChunk &chunk=mem_chunks[ chunk_map[ new_chunk_map_pos ] ];
	
	chunk.len=size_to_alloc;
	chunk.pos=(new_chunk_map_pos==0)?0:mem_chunks[ chunk_map[ new_chunk_map_pos-1 ] ].end(); //alloc either at begining or end of previous
	chunk.lock=0;
	chunk.check=check_count++;
	
	return MemPool_Handle(&chunk,chunk.check);
}


bool MemPool_FixedArea::is_mem_valid(MemPool_Handle p_handle) {
	
	if (p_handle.is_null())
		return false;
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	
	/* If not in range of the pool, it's not valid pointer inside */
	if (chunk<&mem_chunks[0] || chunk>&mem_chunks[MAX_CHUNKS-1])
		return false;
	
	/* Mem has been freed */	
	if (chunk->len==0)
		return false;
	
	/* Check if the item "check" integer is the same */
	if (chunk->check!=get_check( p_handle ))
		return false;
	
	
	return true;
}

void MemPool_FixedArea::free_mem(MemPool_Handle p_handle) {
	
	ERR_FAIL_COND( !is_mem_valid(p_handle) );
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	
	ERR_FAIL_COND(chunk->lock); //it's locked
	
	ChunkMapPos chunk_map_pos;
	
	ERR_FAIL_COND( find_chunk_map(&chunk_map_pos,chunk) );

	for (int i=chunk_map_pos;i<(chunks_allocated-1);i++) {
		
		chunk_map[ i ] = chunk_map[ i+1 ];
	}
	
	chunks_allocated--;
	chunk->clear();

	
}

int MemPool_FixedArea::get_mem_size(MemPool_Handle p_handle) {
	
	ERR_FAIL_COND_V( !is_mem_valid(p_handle),-1 );
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	return chunk->len;
	
}
bool MemPool_FixedArea::realloc_mem(MemPool_Handle p_handle,Uint32 p_new_amount) {
	
	ERR_FAIL_COND_V( !is_mem_valid(p_handle),true );
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	
	ERR_FAIL_COND_V(chunk->lock,true); //it's locked
	
	ChunkMapPos chunk_map_pos;
	
	ERR_FAIL_COND_V( find_chunk_map(&chunk_map_pos,chunk), true );
	
	Uint32 new_len_request=p_new_amount;
	
	{ /* Try strategies that can resize without touching other blocks first */
		
		Uint32 next_chunk_begin_pos=(chunk_map_pos==(chunks_allocated-1))?mem_area_size:mem_chunks[ chunk_map[ chunk_map_pos+1 ] ].pos;
		
		/* FIRST let's try to see if we can resize without moving any data */
		if ( (chunk->pos+new_len_request)<=next_chunk_begin_pos ) {
			
			chunk->len=new_len_request;
			return false; //nothing much was needed to do!
		}
		
		Uint32 prev_chunk_end_pos=(chunk_map_pos==0)?0:mem_chunks[ chunk_map[ chunk_map_pos-1 ] ].end(); //alloc either at begining or
		
		/* SECOND, If we can resize withouth moving the other blocks, try it */
		
		if ( (next_chunk_begin_pos-prev_chunk_end_pos)>=new_len_request ) {
			
			COMPACT_CHUNK(*chunk,prev_chunk_end_pos);
			return false;
			
		}
		
		
	}	
	
	/* If nothing worked, try to move it to the end */
	Uint32 last_block_end_pos=mem_chunks[ chunk_map[ chunks_allocated-1 ] ].end();
	/* Can move to the end? */
	if ( (mem_area_size-last_block_end_pos)<new_len_request ) {
		/* No, can't move, try compacting */
		compact();
		//need to compute this again
		last_block_end_pos=mem_chunks[ chunk_map[ chunks_allocated-1 ] ].end();
		ERR_FAIL_COND_V( (mem_area_size-last_block_end_pos)<new_len_request, true );
		
	}
	
	/* Move it */
	COMPACT_CHUNK(*chunk,last_block_end_pos);
	/* update map */
	ChunkMemPos chunk_mem_pos=chunk_map[ chunk_map_pos ];
	 
	for (Uint32 i=chunk_map_pos;i<(chunks_allocated-1);i++) {
		
		chunk_map[ i ] = chunk_map [ i+1 ];
	}
	
	chunk_map[chunks_allocated-1]=chunk_mem_pos;
		
	chunk->len=new_len_request;		
	
	return false;
}


bool MemPool_FixedArea::lock_mem(MemPool_Handle p_handle) {
	
	ERR_FAIL_COND_V( !is_mem_valid( p_handle ), true );
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	
	chunk->lock++;
	
	return false;
	
}
void *MemPool_FixedArea::get_mem(MemPool_Handle p_handle) {
	
	
	ERR_FAIL_COND_V( !is_mem_valid( p_handle ), NULL );
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	
	ERR_FAIL_COND_V( chunk->lock==0, NULL );
	
	return &((unsigned char*)mem_area)[ chunk->pos ];
	
}
void MemPool_FixedArea::unlock_mem(MemPool_Handle p_handle) {
	
	ERR_FAIL_COND( !is_mem_valid( p_handle ) );
	
	MemChunk *chunk=(MemChunk*)get_private( p_handle );
	
	ERR_FAIL_COND( chunk->lock==0 );
	
	chunk->lock--;
}

Uint32 MemPool_FixedArea::get_total_mem_used() {
	
	Uint32 total=0;
	for (Uint32 i=0;i<MAX_CHUNKS;i++) {
		
		total+=mem_chunks[i].len; //if 0, it's disabled anyway
	}
		
	return total;
}

MemPool_FixedArea::MemPool_FixedArea(void *p_area,Uint32 p_area_size) {
	
	mem_area=p_area;
	mem_area_size=p_area_size;
	
	for (Uint32 i=0;i<MAX_CHUNKS;i++) {
		
		chunk_map[i]=0;
		mem_chunks[i].clear();
	}
	
	chunks_allocated=0;
	check_count=1;	
}


MemPool_FixedArea::~MemPool_FixedArea()
{
}


