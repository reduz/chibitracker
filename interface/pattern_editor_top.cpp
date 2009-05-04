//
// C++ Implementation: pattern_editor_top
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pattern_editor_top.h"
#include "base/painter.h"
#include "interface/ctskin.h"
#include "widgets/value_label.h"
#include <stdio.h>
int PatternEditorTop::get_column_width() {

	int char_width=get_painter()->get_font_char_width( font( FONT_PATTERN_EDITOR ), ' ' );

	return char_width*13+constant(C_PATTERN_EDITOR_COLUMN_SEPARATION);
}

Size PatternEditorTop::get_minimum_size_internal() {

	Painter *p=get_painter();

	Size min;
	min.height=p->get_font_height(font( FONT_PATTERN_EDITOR ));
	min.height+=constant(C_PATTERN_EDITOR_ROW_SEPARATION);
	
	
	return min;

}

void PatternEditorTop::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {


	if (!p_press)
		return;

	if (mouse_over_which==-1)
		return;

	popup_track=-1;
	int track=mouse_over_which+editor->get_column_offset();
	if (track<0 || track>=Pattern::WIDTH)
		return;
	
	Size win_size=Size( get_column_width()*2, 0);
	Point win_ofs=Point(-get_column_width()/2,0);
	win_ofs+=get_global_pos()+Point(get_column_width()*mouse_over_which,get_painter()->get_font_height(font( FONT_PATTERN_EDITOR )) );
	
	if (win_ofs.x<0)
		win_ofs.x=0;
	if ((win_ofs.x+win_size.width)>get_window()->get_size().width)
		win_ofs.x=get_window()->get_size().width-win_size.width;
		
	
	popup->set_pos( win_ofs );
	popup->set_size( win_size );

	mute_button->set_checked( editor->get_song()->is_channel_mute( track ) );
	surround_button->set_checked( editor->get_song()->is_channel_surround( track ) );
	default_vol_slider->get_range()->set((double)editor->get_song()->get_channel_volume( track ) );
	default_pan_slider->get_range()->set((double)editor->get_song()->get_channel_pan( track )-32 );
	default_chorus_slider->get_range()->set((double)editor->get_song()->get_channel_chorus( track ) );
	default_reverb_slider->get_range()->set((double)editor->get_song()->get_channel_reverb( track ) );
	popup_track=track;
	popup->show();
}

void PatternEditorTop::mouse_leave() {


	int over_which=-1;

	if (over_which==mouse_over_which)
		return;

	mouse_over_which=over_which;
	update();
	
}
void PatternEditorTop::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {

	int over_which=p_pos.x/get_column_width();

	if (over_which>=editor->get_visible_columns())
		over_which=-1;
	
	if (over_which==mouse_over_which)
		return;

	mouse_over_which=over_which;
	update();
}

void PatternEditorTop::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	
	Painter *p=get_painter();


	int visible_c=editor->get_visible_columns();
	int offset_c=editor->get_column_offset();
	

	int char_w=get_painter()->get_font_char_width( font( FONT_PATTERN_EDITOR ), ' ' );
	

	for (int i=0;i<visible_c;i++) {

		char top[]={'T','r','a','c','k',' ','0','0',0};


		BitmapID popup_bitmap=bitmap((mouse_over_which==i)?BITMAP_PATTERN_EDITOR_TRACK_POPUP_PRESSED:BITMAP_PATTERN_EDITOR_TRACK_POPUP);
		BitmapID popup_bitmap_pressed=bitmap(BITMAP_PATTERN_EDITOR_TRACK_POPUP_PRESSED);
		int ofs=get_column_width()*i;;

		bool mute=editor->get_song()->is_channel_mute( editor->get_column_offset()+ i);
		Color text_color=color((mouse_over_which==i)?COLOR_PATTERN_EDITOR_TOP_SELECTED:(mute?COLOR_PATTERN_EDITOR_TOP_MUTE:COLOR_PATTERN_EDITOR_TOP));
		if (popup_bitmap>=0) {


			p->draw_bitmap( popup_bitmap, Point(ofs,0));
			ofs+=p->get_bitmap_size( popup_bitmap ).width;
		} else {

			p->draw_arrow( Point(ofs,0), Size(char_w,p_size.height),DOWN,text_color);
			ofs+=char_w;
		}

		ofs+=char_w;;

		int colidx=i+offset_c+1;
		top[6]+=colidx/10;
		top[7]+=colidx%10;
		

		p->draw_text( font( FONT_PATTERN_EDITOR_TOP ), Point( ofs, p->get_font_ascent( font( FONT_PATTERN_EDITOR_TOP ) ) ), top, text_color );
	
	}


}

void PatternEditorTop::track_reverb_changed(double p_to_what) {
	
	if (popup_track<0)
		return;
	editor->get_song()->set_channel_reverb(popup_track, (int)p_to_what );
		
}
void PatternEditorTop::track_chorus_changed(double p_to_what) {
	
	if (popup_track<0)
		return;
	editor->get_song()->set_channel_chorus(popup_track, (int)p_to_what );
	
}

void PatternEditorTop::track_volume_changed(double p_to_what) {

	if (popup_track<0)
		return;
	editor->get_song()->set_channel_volume(popup_track, (int)p_to_what );
	
}
void PatternEditorTop::track_pan_changed(double p_to_what) {

	if (popup_track<0)
		return;
	
	editor->get_song()->set_channel_pan(popup_track, (int)p_to_what+32 );

}
void PatternEditorTop::track_mute_toggled(bool p_mute) {

	if (popup_track<0)
		return;
	
	editor->get_song()->set_channel_mute(popup_track, p_mute );

	channel_mute_toggled.call();
}

void PatternEditorTop::track_surround_toggled(bool p_surround) {
	
	if (popup_track<0)
		return;
	
	editor->get_song()->set_channel_surround(popup_track, p_surround );

}

void PatternEditorTop::set_in_window() {

	popup = new Window(get_window(),Window::MODE_POPUP);
	VBoxContainer *vbc = new VBoxContainer;
	popup->set_root_frame( vbc );

	MarginGroup *mg;

	mg=vbc->add( new MarginGroup("Initial Volume"), 0 );
	HBoxContainer *vol_hb = mg->add( new HBoxContainer, 0 );
	mute_button=mg->add( new CheckButton("Mute"),0 );
	

	default_vol_slider=vol_hb->add( new HSlider, 1 );
	vol_hb->add( new ValueLabel, 0)->set_range( default_vol_slider->get_range() );
	
	mg=vbc->add( new MarginGroup("Initial Pan"), 0 );
	HBoxContainer *pan_hb = mg->add( new HBoxContainer, 0 );
	surround_button=mg->add( new CheckButton("Surround"),0 );
	
	default_pan_slider=pan_hb->add( new HSlider, 1 );
	pan_hb->add( new ValueLabel, 0)->set_range( default_pan_slider->get_range() );

	//vbc->set_stylebox_override( stylebox( SB_POPUP_BG ) );

	mute_button->toggled_signal.connect( this, &PatternEditorTop::track_mute_toggled );
	surround_button->toggled_signal.connect( this, &PatternEditorTop::track_surround_toggled );
	default_pan_slider->get_range()->set_min(-32);
	default_pan_slider->get_range()->set_max(32);
	default_pan_slider->get_range()->value_changed_signal.connect( this, &PatternEditorTop::track_pan_changed );
	default_vol_slider->get_range()->value_changed_signal.connect( this, 	&PatternEditorTop::track_volume_changed );
	default_vol_slider->get_range()->set_max(64);

	mg=vbc->add( new MarginGroup("Initial Chorus"), 0 );
	HBoxContainer *chorus_hb = mg->add( new HBoxContainer, 0 );
	default_chorus_slider=chorus_hb->add( new HSlider, 1 );
	chorus_hb->add( new ValueLabel, 0)->set_range( default_chorus_slider->get_range() );
	default_chorus_slider->get_range()->value_changed_signal.connect( this, 	&PatternEditorTop::track_chorus_changed );
	default_chorus_slider->get_range()->set_max( Song::CHANNEL_MAX_CHORUS );
	
	mg=vbc->add( new MarginGroup("Initial Reverb"), 0 );
	HBoxContainer *reverb_hb = mg->add( new HBoxContainer, 0 );
	default_reverb_slider=reverb_hb->add( new HSlider, 1 );
	reverb_hb->add( new ValueLabel, 0)->set_range( default_reverb_slider->get_range() );
	default_reverb_slider->get_range()->set_max( Song::CHANNEL_MAX_REVERB );
	default_reverb_slider->get_range()->value_changed_signal.connect( this, 	&PatternEditorTop::track_reverb_changed );
	
	

}
PatternEditorTop::PatternEditorTop(Editor *p_editor) {

	editor=p_editor;
	mouse_over_which=-1;
	popup=0;
}


PatternEditorTop::~PatternEditorTop()
{
	if (popup)
		delete popup;
}


