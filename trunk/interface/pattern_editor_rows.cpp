//
// C++ Implementation: pattern_editor_rows
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pattern_editor_rows.h"
#include "base/painter.h"
#include "interface/ctskin.h"


void PatternEditorRows::check_play_cursor() {

	int current_playing_pattern=editor->get_player()->is_playing()?editor->get_player()->get_current_pattern():-1;

	int current_playing_row=editor->get_player()->is_playing()?editor->get_player()->get_current_row():-1;
	
	
	int current_pat=editor->get_current_pattern();
	
	if (last_pattern_played==current_pat || current_playing_pattern==current_pat) {
		
		if (current_playing_row!=last_row_played)
			update();
		
	}
	
	last_row_played=current_playing_row;
	last_pattern_played=current_playing_pattern;
}


Size PatternEditorRows::get_minimum_size_internal() {

	Painter *p=get_painter();

	Size min;
	min.width=p->get_font_char_width(font( FONT_PATTERN_EDITOR ),' ')*5;
	
	return min;

}
void PatternEditorRows::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	Painter *p=get_painter();
	
	int row_height=p->get_font_height( font( FONT_PATTERN_EDITOR ))+constant(C_PATTERN_EDITOR_ROW_SEPARATION);
	int visible_rows=p_size.height/row_height;
	
	int font_w=p->get_font_char_width(font( FONT_PATTERN_EDITOR ),' ');
	int current_pat=editor->get_current_pattern();

	for(int i=0;i<visible_rows;i++) {
		
		int r=i+editor->get_row_offset();
		if (r<0 || r>=editor->get_song()->get_pattern( editor->get_current_pattern() )->get_length())
			continue;
		char row[4]={0,0,0,0};
		row[0]='0'+r/100;
		row[1]='0'+(r/10)%10;
		row[2]='0'+r%10;
		
		Color col=color(COLOR_PATTERN_EDITOR_ROWS);
		
		if (editor->get_player()->is_playing()) {
			
			int current_playing_pattern=editor->get_player()->get_current_pattern();
			int current_playing_row=editor->get_player()->get_current_row();
			
			if (current_pat==current_playing_pattern && r==current_playing_row)
				col=color(COLOR_PATTERN_EDITOR_ROWS_CURRENT);
		}
		
		p->draw_text( font(FONT_PATTERN_EDITOR_ROWS), Point(font_w,i*row_height+p->get_font_ascent(font(FONT_PATTERN_EDITOR_ROWS))), row,col );
	}
}

PatternEditorRows::PatternEditorRows(Editor *p_editor)
{
	editor=p_editor;
	last_pattern_played=-1;
	last_row_played=-1;
	
}


PatternEditorRows::~PatternEditorRows()
{
}


