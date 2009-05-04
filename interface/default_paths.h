//
// C++ Interface: default_paths
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DEFAULT_PATHS_H
#define DEFAULT_PATHS_H

#include "base/defs.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduz@gmail.com>
*/
class DefaultPaths{
public:
    
	String song_path;
	String sample_path;
	String instrument_path;
	
	
	DefaultPaths();
	~DefaultPaths();

};

#endif
