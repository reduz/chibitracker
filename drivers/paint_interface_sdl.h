//
// C++ Interface: paint_interface_sdl
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAINT_INTERFACE_SDL_H
#define PAINT_INTERFACE_SDL_H
#include <SDL.h>

#include <vector>
#include "gui/paint_interface.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PaintInterface_SDL : public PaintInterface {

	SDL_Surface *surface;
	Rect rect;
	
	std::vector<SDL_Surface*> bitmaps;
	struct Font {
		
		SDL_Surface * surface;
		Uint8 ascii_begin;
		Uint8 characters;
		Uint16 char_w;
	};
	std::vector<Font> fonts;
public:
	
	void set_rect(const Rect& p_rect);
	
	void draw_line(const Pos & p_from,const Pos & p_to,const Color& p_color);
	
	void draw_rect(const Pos & p_from,const Size & p_size,const Color& p_color);
	
	void draw_fill_rect(const Pos & p_from,const Size & p_size,const Color& p_color);
	
	void draw_text(FontID p_font,const Pos & p_pos,const char *p_text);
	
	void draw_bitmap(BitmapID p_bitmap,const Pos &p_pos );
	
	Uint16 get_font_width(FontID p_font);
	Uint16 get_font_height(FontID p_font);
	
	Uint16 get_bitmap_width(BitmapID p_bitmap);
	Uint16 get_bitmap_height(BitmapID p_bitmap);
	
	BitmapID register_bitmap(SDL_Surface *p_bitmap);
	FontID register_font(SDL_Surface *p_surface,Uint8 p_ascii_begin,Uint8 p_chars);
	
	PaintInterface_SDL(SDL_Surface *p_surface);
	
	~PaintInterface_SDL();

};

#endif
