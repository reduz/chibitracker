//
// C++ Implementation: signals
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "signals.h"

#ifndef GUI_EXTERNAL_SIGNAL

namespace GUI {

bool SignalBase::register_at_target( SignalTarget *p_target ) {
	
	return p_target->_register_signal(this);
}
void SignalBase::remove_from_target( SignalTarget *p_target ) {
	
	p_target->_clear_signal(this);
	
}


bool SignalTarget::_register_signal( SignalBase *p_signal ) {

	Connection *c=_conn_list;

	while (c) {

		if (c->signal==p_signal) {
			
			c->refcount++;
			return true;
		}

		c=c->next;
	}

	c = new Connection;

	c->next=_conn_list;
	c->signal=p_signal;
	_conn_list=c;
	
	return false;
}
void SignalTarget::_clear_signal( SignalBase *p_signal ) {


	Connection **pc=&_conn_list; //previous
	Connection *c=_conn_list;

	while (c) {

		if (c->signal==p_signal) {
			
			c->refcount--;
			if (c->refcount<=0) {
				
				*pc=c->next;
				delete c;
			}
			
			return; // just be connected only once anyway
		}
		pc=&c->next;
		c=c->next;
	}

}
		
SignalTarget::SignalTarget() {

	_conn_list=0; //no connections to here
}
SignalTarget::~SignalTarget() {

	while (_conn_list) {

		Connection *c=_conn_list;
		_conn_list=_conn_list->next;
		c->signal->remove_target( this );
		delete c;
	}		
}

}

#endif