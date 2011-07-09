//
// C++ Implementation: theme_loader
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "theme_loader.h"
#include <stdio.h>

namespace GUI {

void ThemeLoader::cleanup_previous_theme() {


	for (int i=0;i<root->get_skin()->get_font_count();i++) {
	
		FontID font=root->get_skin()->get_font(i);
		if (font<1)
			continue;

		root->get_painter()->erase_font_and_bitmaps( font );
	
	}

	for (int i=0;i<root->get_skin()->get_bitmap_count();i++) {
	
		BitmapID bitmap=root->get_skin()->get_bitmap(i);
		if (bitmap<1)
			continue;

		root->get_painter()->remove_bitmap( bitmap );
	
	}

	for (int i=0;i<root->get_skin()->get_stylebox_count();i++) {
	
		const StyleBox & sb= root->get_skin()->get_stylebox(i);

		root->get_painter()->remove_bitmap( sb.bitmap );
	
	}

}

Color ThemeLoader::parse_color(String p_txt) {


	int slices=p_txt.get_slice_count(",");
	
	if (slices==1)
		return Color(p_txt.to_int());
	
	else if (slices!=3)
		return Color(0);
					
	Color ret;
	
	for (int i=0;i<3;i++) {
						
		int col=p_txt.get_slice( ",",i).to_int();
		
		
		switch(i) {
			
			case 0: ret.r=col; break;
			case 1: ret.g=col; break;
			case 2: ret.b=col; break;
			
		}
		
	}
	
	return ret;
	
}

void ThemeLoader::read_sb_margins(StyleBox *p_sb,String p_margins){

	int slices=p_margins.get_slice_count(",");
	if (slices!=4)
		return; //leave margins as they are
	
	
	
	for (int i=0;i<4;i++) {
		
		int margin=p_margins.get_slice( ",",i).to_int();
		
		p_sb->margins[i]=margin;	
	}
	
}

void ThemeLoader::read_sb_bitmaps(StyleBox *p_sb,String p_margins){


	int slices=p_margins.get_slice_count(",");
//	printf("++slices: for: %s, %i\n",p_margins.ascii().get_data(),slices);
	if (slices!=5)
		return; //leave margins as they are
					
	int m[4];
	
	for (int i=0;i<4;i++) {
						
		int margin=p_margins.get_slice( ",",i).to_int();
		
		m[i]=margin;
	}
	
	String bitmap=p_margins.get_slice( ",",4);
	
	BitmapID sb_bitmap=root->get_painter()->load_bitmap( theme_dir+"/"+bitmap );
	
//	printf("++bitmap loading: %i\n",sb_bitmap);
	if (sb_bitmap<0)
		return;
	p_sb->bitmap=sb_bitmap;
	
	for (int i=0;i<4;i++) {
						
		p_sb->bitmap_margins[i]=m[i];
	}
	
	
}

BitmapID ThemeLoader::get_bitmap_cache_for(String p_file){

	BitmapCacheList *bl=bmp_cache_list;
	
	while (bl) {
		
		if (bl->file==p_file) {
			
			return bl->cache;
		}
		
		bl=bl->next;
	}

	/* not cached, create */

	BitmapID id=root->get_painter()->load_bitmap( theme_dir+"/"+p_file );
	if (id==-1)
		return -1;
	
	bl= new BitmapCacheList;
	bl->file=p_file;
	bl->cache=id;
	bl->next=bmp_cache_list;
	bmp_cache_list=bl;
	
	return  id;
}

void ThemeLoader::read_font_entry(String p_section,String p_entry,String p_value) {


	if (p_section=="info") {
	
		if (p_entry=="height")
			read_font_height=p_value.to_int();
		else if (p_entry=="ascent")
			read_font_ascent=p_value.to_int();
	
	} else if (p_section=="characters") {
	
		if (read_font<0)
			return;
		
		int char_idx=p_entry.to_int();
		
		int slices=p_value.get_slice_count(",");
		if (slices!=5 && slices!=6)
			return;
			
		String bmp=p_value.get_slice(",",slices-1);
		
		BitmapID  bmpid = get_bitmap_cache_for( bmp );
		
		if (bmpid<0)
			return;
		
		Rect char_rect;
		
		char_rect.pos.x=p_value.get_slice(",",0).to_int();
		char_rect.pos.y=p_value.get_slice(",",1).to_int();
		char_rect.size.x=p_value.get_slice(",",2).to_int();
		char_rect.size.y=p_value.get_slice(",",3).to_int();
		int valign=(slices==5)?0:p_value.get_slice(",",4).to_int();
		
		if (char_rect.has_no_area())
			return;
		root->get_painter()->font_add_char( read_font, char_idx, bmpid, char_rect,valign );
	
	}

}

void ThemeLoader::read_font_section(String p_section) {

	if (p_section!="info")
		return;
	if (read_font!=-1)
		return;
	if (read_font_height<1)
		return;
	if (read_font_ascent<1)
		return;
	
	read_font=root->get_painter()->create_font( read_font_height, read_font_ascent );

}


FontID ThemeLoader::get_font_cache_for(String p_file) {

	FontCacheList *fl=font_cache_list;
	
	while (fl) {
		
		if (fl->file==p_file) {
			
			return fl->cache;
		}
		
		fl=fl->next;
	}

	/* not cached, create */


	read_font=-1;
	read_font_height=-1;
	read_font_ascent=-1;

	FileAccessWrapper *f = FileAccessWrapper::create();
	ConfigFile font(f);
	font.set_path( theme_dir +"/"+ p_file );
	font.read_entry_signal.connect(this,&ThemeLoader::read_font_entry);
	font.read_section_end_signal.connect(this,&ThemeLoader::read_font_section);

	if (font.load()) {
		delete f;
		return -1;
	
	}
	
	if (read_font==-1) {
	
		delete f;
		return -1;
	}
	
	
	fl= new FontCacheList;
	fl->file=p_file;
	fl->cache=read_font;
	fl->next=font_cache_list;
	font_cache_list=fl;
	
	delete f;

	return  read_font;

}

void ThemeLoader::section_begin(String p_section) {

	printf("reading section: %s\n",p_section.ascii().get_data());
	if (p_section=="stylebox") {
		
		reading=READ_STYLEBOX;
	} else if (p_section=="constant_list") {
		reading=READ_CONSTANT_LIST;
	} else if (p_section=="bitmap_list") {
		reading=READ_BITMAP_LIST;
	} else if (p_section=="font_list") {
		reading=READ_FONT_LIST;
	} else if (p_section=="color_list") {

		reading=READ_COLOR_LIST;		
	} else if (p_section=="stylebox_list") {
		
		reading=READ_STYLEBOX_LIST;
		
	} else {
		
		reading=READ_UNKNOWN;
	}
	
	
}

void ThemeLoader::read_entry(String p_section,String p_entry,String p_value) {
	
	switch (reading) {
		
		case READ_STYLEBOX: {
			
			if (p_entry=="name") {
				
				StyleBoxList * l = new 	StyleBoxList;
				l->name=p_value;
				l->next=sb_list;
				sb_list=l;
				sb_loading=l;
				break;
			} 
			
			if (!sb_loading)
				break; //can't accept variables if i dont have the name, you see!
			
			
			if (p_entry=="mode") {
				
				if (p_value=="flat") 
					
					sb_loading->sb.mode=StyleBox::MODE_FLAT;
				else if (p_value=="flat_bitmap")
					sb_loading->sb.mode=StyleBox::MODE_FLAT_BITMAP;
				else if (p_value=="bitmap")
					sb_loading->sb.mode=StyleBox::MODE_BITMAP;
				else
					sb_loading->sb.mode=StyleBox::MODE_NONE;

				//optherwise, mode is none
				
			} else if (p_entry=="color_upleft")
				sb_loading->sb.flat.border_upleft=parse_color(p_value);
			else if (p_entry=="color_downright")
				sb_loading->sb.flat.border_downright=parse_color(p_value);
			else if (p_entry=="color_center")
				sb_loading->sb.flat.center=parse_color(p_value);
			else if (p_entry=="flat_margin")
				sb_loading->sb.flat.margin=p_value.to_int();
			else if (p_entry=="margins")
				read_sb_margins(&sb_loading->sb,p_value);
			else if (p_entry=="bitmaps")
				read_sb_bitmaps(&sb_loading->sb,p_value);
			else if (p_entry=="draw_center")
				sb_loading->sb.draw_center=(p_value=="true");
//			printf("margin is: %i, mode %i\n",sb_loading->sb.flat.margin,sb_loading->sb.mode);
		} break;
		case READ_STYLEBOX_LIST: {
			
			int which=root->get_skin()->find_stylebox("SB_"+p_entry);
//			printf("SB entry for %s is %i\n",("SB_"+p_entry).ascii().get_data(),which);
			if (which<0)
				break;
			
			StyleBoxList *s=sb_list;
			
			while (s) {
				
				if (s->name==p_value) {
					root->get_skin()->set_stylebox( which, s->sb );
					break;
				}
				
				s=s->next;
			}
			
		} break;
		case READ_COLOR_LIST: {
			
			int which=root->get_skin()->find_color("COLOR_"+p_entry);
			if (which<0)
				break;
			
			root->get_skin()->set_color(which,parse_color( p_value ));
			
		} break;
		case READ_CONSTANT_LIST: {
			
			int which=root->get_skin()->find_constant("C_"+p_entry);
			if (which<0)
				break;
			
			root->get_skin()->set_constant(which,(int)p_value.to_double());

		} break;
		case READ_BITMAP_LIST: {
			
			int which=root->get_skin()->find_bitmap("BITMAP_"+p_entry);
			if (which<0)
				break;
			
			BitmapID cache = get_bitmap_cache_for(p_value);
			
			if (cache<0)
				break;
			root->get_skin()->set_bitmap(which,cache);

		} break;
		case READ_FONT_LIST: {
			
			int which=root->get_skin()->find_font("FONT_"+p_entry);
			if (which<0)
				break;
			
			FontID cache = get_font_cache_for(p_value);
			
			if (cache<0)
				break;
			root->get_skin()->set_font(which,cache);

		} break;
				
		
	}
	
}


void ThemeLoader::section_end(String p_Section) {
	
	if (sb_loading)
		sb_loading=0;
	
}


void ThemeLoader::open(String p_file) {
	
	sb_list=0;
	bmp_cache_list=0;
	font_cache_list=0;
	
//	printf("attempting to open: %s\n",(theme_path+"/"+p_file).ascii().get_data() );
	theme_dir=theme_path+"/"+p_file;
	config->set_path(theme_path+"/"+p_file+"/skin.cfg");
	
	cleanup_previous_theme();
	root->get_skin()->set_default();
	
	if (config->load()) {
		
//		printf("Load Failed\n");
	}
	
	while (sb_list) {
		
		StyleBoxList *aux = sb_list;
		sb_list=sb_list->next;
		delete aux;
	}
		
	while (bmp_cache_list) {
		
		BitmapCacheList *aux = bmp_cache_list;
		bmp_cache_list=bmp_cache_list->next;
		delete aux;
	}
	
	while (font_cache_list) {
		
		FontCacheList *aux = font_cache_list;
		font_cache_list=font_cache_list->next;
		delete aux;
	}

	sb_list=0;
	bmp_cache_list=0;
	font_cache_list=0;
	
}


void ThemeLoader::set_theme_path(String p_path) {
	
	theme_path=p_path;
}


ThemeLoader::ThemeLoader(ConfigFile *p_config,Window *p_root) {

	root=p_root;
	config=p_config;
	
	sb_list=0;
	bmp_cache_list=0;
	
	config->read_entry_signal.connect(this,&ThemeLoader::read_entry);
	config->read_section_begin_signal.connect(this,&ThemeLoader::section_begin);
	config->read_section_end_signal.connect(this,&ThemeLoader::section_end);
}


ThemeLoader::~ThemeLoader()
{
}

}
