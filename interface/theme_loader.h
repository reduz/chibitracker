//
// C++ Interface: theme_loader
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef THEME_LOADER_H
#define THEME_LOADER_H

#include "interface/config_file.h"
#include "base/window.h"
/**
	@author Juan Linietsky <reduz@gmail.com>
*/

namespace GUI {

class ThemeLoader : public SignalTarget{

	Window *root;
	ConfigFile *config;
	
	enum CurrentlyReading {
		
		READ_UNKNOWN,
		READ_STYLEBOX,
		READ_CONSTANT_LIST,
		READ_BITMAP_LIST,
		READ_COLOR_LIST,
		READ_FONT_LIST,
		READ_STYLEBOX_LIST,

	};
	
	CurrentlyReading reading;
	
	void read_entry(String p_section,String p_entry,String p_value);
	void section_begin(String p_Section);
	void section_end(String p_Section);
	
	
	
	
	
	struct StyleBoxList {
		
		String name;
		StyleBox sb;
		StyleBoxList *next;
		StyleBoxList() { next=0; }
	};
	
	
	StyleBoxList *sb_list;
	StyleBoxList *sb_loading;
	
	struct BitmapCacheList {
		
		String file;
		BitmapID cache;
		BitmapCacheList *next;
		BitmapCacheList() { next=0; }
	};
	
	BitmapCacheList *bmp_cache_list;
	
	struct FontCacheList {
		
		String file;
		FontID cache;
		FontCacheList *next;
		FontCacheList() { next=0; }
	};
	
	FontCacheList *font_cache_list;
	
	Color parse_color(String p_txt);
	void read_sb_margins(StyleBox *p_sb,String p_margins);
	void read_sb_bitmaps(StyleBox *p_sb,String p_margins);
	BitmapID get_bitmap_cache_for(String p_file);
	FontID get_font_cache_for(String p_file);
	
	String theme_path;
	
	String theme_dir;
	
	/* Font loader */
	void read_font_entry(String p_section,String p_entry,String p_value);
	void read_font_section(String p_section);
	FontID read_font; //font being read
	int read_font_height;
	int read_font_ascent;
	
	void cleanup_previous_theme();
public:
	
	void set_theme_path(String p_path);
	
	void open(String p_file);
	
	ThemeLoader(ConfigFile *p_config,Window *p_root);
	~ThemeLoader();

};

};

#endif
