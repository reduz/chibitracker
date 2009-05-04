//
// C++ Implementation: paint_interface_sdl
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "paint_interface_sdl.h"
#include <SDL_gfxPrimitives.h>
#include "error_macros.h"

void PaintInterface_SDL::set_rect(const Rect& p_rect) {

	rect=p_rect;
}

void PaintInterface_SDL::draw_line(const Pos & p_from,const Pos & p_to,const Color& p_color) {
	
	Pos from=p_from+rect.pos;
	Pos to=p_to+rect.pos;
	lineRGBA(surface,from.x,from.y,to.x,to.y,p_color.r,p_color.g,p_color.b,255);
}

void PaintInterface_SDL::draw_rect(const Pos & p_from,const Size & p_size,const Color& p_color) {
	
	Pos from=p_from+rect.pos;
		
	rectangleRGBA(surface, from.x,from.y,from.x+p_size.width-1, from.y+p_size.height-1,p_color.r,p_color.g,p_color.b,255);
}

void PaintInterface_SDL::draw_fill_rect(const Pos & p_from,const Size & p_size,const Color& p_color) {
	
	Pos from=p_from+rect.pos;
	boxRGBA(surface, from.x,from.y,from.x+p_size.width-1, from.y+p_size.height-1,p_color.r,p_color.g,p_color.b,255);
	
	
}

void PaintInterface_SDL::draw_text(FontID p_font,const Pos & p_pos,const char *p_text) {
	ERR_FAIL_INDEX(p_font,fonts.size());
	Pos pos=p_pos+rect.pos;
	
	Font &f=fonts[p_font];
	Uint16 char_idx=0;
	while (p_text[char_idx]) {
		Uint8 ch=p_text[char_idx]; 
		ch-=f.ascii_begin;
		if (ch<f.characters) {
			SDL_Rect src_rect;
			src_rect.x=(Uint16)f.char_w*(Uint16)ch;
			src_rect.y=0;
			src_rect.w=f.char_w;
			src_rect.h=f.surface->h;
			
			SDL_Rect dst_rect;
			dst_rect.x=pos.x+f.char_w*char_idx;
			dst_rect.y=pos.y;
			dst_rect.w=dst_rect.h=0;
			
			SDL_BlitSurface(f.surface, &src_rect, surface, &dst_rect);
		}
		char_idx++;
	}
}

void PaintInterface_SDL::draw_bitmap(BitmapID p_bitmap,const Pos &p_pos) {
	
	ERR_FAIL_INDEX(p_bitmap,bitmaps.size());
	
	SDL_Rect dst_rect;
	dst_rect.x=p_pos.x+rect.pos.x;
	dst_rect.y=p_pos.y+rect.pos.y;
	dst_rect.w=dst_rect.h=0;
			
	SDL_BlitSurface(bitmaps[p_bitmap], NULL, surface, &dst_rect);

}

	
Uint16 PaintInterface_SDL::get_font_width(FontID p_font) {
	
	ERR_FAIL_INDEX_V(p_font,fonts.size(),0);
	
	return fonts[p_font].char_w;
}
Uint16 PaintInterface_SDL::get_font_height(FontID p_font) {
	
	ERR_FAIL_INDEX_V(p_font,fonts.size(),0);
	
	return fonts[p_font].surface->h;
	
}

Uint16 PaintInterface_SDL::get_bitmap_width(BitmapID p_bitmap) {
	
	ERR_FAIL_INDEX_V(p_bitmap,bitmaps.size(),0);
	
	return bitmaps[p_bitmap]->w;
}
Uint16 PaintInterface_SDL::get_bitmap_height(BitmapID p_bitmap) {
	
	ERR_FAIL_INDEX_V(p_bitmap,bitmaps.size(),0);
	
	return bitmaps[p_bitmap]->h;	
}

BitmapID PaintInterface_SDL::register_bitmap(SDL_Surface *p_bitmap) {
	
	bitmaps.push_back(p_bitmap);
	return bitmaps.size()-1;
}
FontID PaintInterface_SDL::register_font(SDL_Surface *p_surface,Uint8 p_ascii_begin,Uint8 p_chars) {
	
	Font f;
	f.surface=p_surface;
	f.char_w=p_surface->w/p_chars;
	f.ascii_begin=p_ascii_begin;
	f.characters=p_chars;
	fonts.push_back(f);
	
	return fonts.size()-1;
}

PaintInterface_SDL::PaintInterface_SDL(SDL_Surface *p_surface) {
	
	surface=p_surface;
	
}


PaintInterface_SDL::~PaintInterface_SDL()
{
}


