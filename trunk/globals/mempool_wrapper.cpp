#include "mempool_wrapper.h"

MemPool_Wrapper * MemPool_Wrapper::singleton=0;

MemPool_Wrapper *MemPool_Wrapper::get_singleton() {

	return singleton;
}

MemPool_Wrapper::MemPool_Wrapper() {

	singleton=this;
}

