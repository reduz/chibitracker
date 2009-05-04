#ifndef ORDER_H
#define ORDER_H


#include "globals/config.h"

enum OrderType {
 	ORDER_NONE=255,
	ORDER_BREAK=254
};

typedef Uint8 Order;

#endif

