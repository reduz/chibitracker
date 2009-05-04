//
// C++ Interface: pattern_editor_top
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PATTERN_EDITOR_TOP_H
#define PATTERN_EDITOR_TOP_H


#include "base/widget.h"
#include "tracker/editor.h"

#include "base/window.h"
#include "bundles/margin_group.h"
#include "widgets/check_button.h"
#include "widgets/slider.h"

using namespace GUI;


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PatternEditorTop : public Widget {


	
	int popup_track;
	int mouse_over_which;
	int get_column_width();
	Editor *editor;
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	virtual void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	virtual void mouse_leave();
	
	virtual Size get_minimum_size_internal();


	void set_in_window();
	Window *popup;
	
	CheckButton *mute_button;
	CheckButton *surround_button;
	Slider *default_vol_slider;
	Slider *default_pan_slider;

	Slider *default_chorus_slider;
	Slider *default_reverb_slider;
	
	void track_reverb_changed(double p_to_what);
	void track_chorus_changed(double p_to_what);
	void track_volume_changed(double p_to_what);
	void track_pan_changed(double p_to_what);
	void track_mute_toggled(bool p_mute);
	void track_surround_toggled(bool p_mute);
public:

	Signal<> channel_mute_toggled;
	PatternEditorTop(Editor *p_editor);
	~PatternEditorTop();

};

#endif
