//
// C++ Interface: playback_button
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLAYBACK_BUTTON_H
#define PLAYBACK_BUTTON_H

#include "widgets/base_button.h"


using namespace GUI;

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class PlaybackButton : public BaseButton {
public:	
	
	
	enum Type {
		
		TYPE_REWIND,
		TYPE_PLAY,
		TYPE_STOP,
		TYPE_FORWARD,
		TYPE_PLAY_PATTERN,
		TYPE_PLAY_CURSOR
	};
	
private:	
	Type type;
	bool no_minsize;

	Size get_minimum_size_internal();
	void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);

	virtual String get_type();
	
public:

	PlaybackButton(Type p_type,bool p_no_minsize=false);
	~PlaybackButton();

};



#endif
