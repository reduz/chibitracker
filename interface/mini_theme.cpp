//
// C++ Implementation: mini_theme
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mini_theme.h"
#include "ctskin.h"

#define SET_STYLEBOX p_skin->set_stylebox
#define SET_BITMAP p_skin->set_bitmap
#define SET_CONSTANT p_skin->set_constant
#define SET_COLOR p_skin->set_color

static const int font_builtin_width=477;
static const int font_builtin_height=6;
static const int font_builtin_char_width=5;

static const unsigned char font_builtin_data[]={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,1,1,1,0,1,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,0,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,1,0,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,0,1,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void MiniTheme::create(Skin *p_skin,Painter *p_painter) {
	

	/** FONT **/
	
	int chars=font_builtin_width/font_builtin_char_width;
	Painter *p=p_painter;
	
	BitmapID font_bmp = p->create_bitmap(Size(font_builtin_width,font_builtin_height),MODE_ALPHA_MASK);
	for (int y=0;y<font_builtin_height;y++) {
		for (int x=0;x<font_builtin_width;x++) {
		
			p->set_bitmap_pixel(font_bmp,Point(x,y),Color(font_builtin_data[y*font_builtin_width+x]?255:0));
			
		}
	}
	FontID font = p->create_font(font_builtin_height,font_builtin_height-1);
	FontID font_big = p->create_font(font_builtin_height,font_builtin_height-1);
	
	for (int i=0;i<chars;i++) {
				
		int chr=i+32;
		int idx=i;
		p->font_add_char(font,chr,font_bmp,Rect(Point(font_builtin_char_width*idx,0),Size(font_builtin_char_width,font_builtin_height)),0);
		
		if (chr>='a' && chr <='z')
			idx-='a'-'A';
		
		p->font_add_char(font_big,chr,font_bmp,Rect(Point(font_builtin_char_width*idx,0),Size(font_builtin_char_width,font_builtin_height)),0);
		
	}
	
	for (int i=0;i<p_skin->get_font_count();i++) {
		
		p_skin->set_font(i,font_big);
	}
		
	/* widgets */
	
	
	/* Base Container */
	
	SET_STYLEBOX( SB_ROOT_CONTAINER, StyleBox( 0, Color( 0xbd,0x9a,0x78 ),Color(250,250,250),Color(90,90,90) ) );
	
	StyleBox msf;
	for (int i=0;i<4;i++)
		msf.margins[i]=1;
	SET_STYLEBOX( SB_MAIN_STACK_FRAME, msf );
	SET_STYLEBOX( SB_TOP_STACK_FRAME, msf );
	
	/* Separator */
	
	SET_STYLEBOX( SB_SEPARATOR, StyleBox( 1, Color( 0,0,0 ), Color( 0x8c,0x65,0x4d ), Color( 0x8c,0x65,0x4d ) ) );
	SET_CONSTANT( C_SEPARATOR_EXTRA_MARGIN,1);
	
	/* Box Container */
	
	SET_CONSTANT(C_BOX_CONTAINER_DEFAULT_SEPARATION,0);
	
	/* Margin Group Container */

	SET_CONSTANT(C_MARGIN_GROUP_SPACER_SIZE,3);
	
	/* PopUp Background */
	
	/* sliders */
	SET_STYLEBOX( SB_SLIDER_NORMAL_H, StyleBox( 0, Color(0,0,0), Color(0,0,0) ,Color(0,0,0) ) );
	SET_STYLEBOX( SB_SLIDER_NORMAL_V, StyleBox( 0, Color(0,0,0), Color(0,0,0) ,Color(0,0,0) ) );
	
	SET_STYLEBOX( SB_SLIDER_FOCUS, StyleBox( 1,  Color(0,0,0 ), Color( 255,255,255 ), Color( 255,255,255 ) ) );
	SET_STYLEBOX( SB_SLIDER_GRABBER_H, StyleBox( 2, Color( 0xbd,0x9a,0x78 ), Color( 0,0,0 ), Color( 0,0,0) ) );
	SET_STYLEBOX( SB_SLIDER_GRABBER_V, StyleBox( 2, Color( 0xbd,0x9a,0x78 ), Color( 0,0,0 ), Color( 0,0,0) ) );	
	SET_STYLEBOX( SB_SLIDER_GRABBER_V, StyleBox( 2, Color( 150,150,150 ), Color( 250,250,250 ), Color( 50,50,50 ) ) );
	SET_CONSTANT( C_SLIDER_GRABBER_SIZE, 5 );
	SET_BITMAP( BITMAP_SLIDER_GRABBER_H, -1 );
	SET_BITMAP( BITMAP_SLIDER_GRABBER_V, -1 );
	
	/* button */
	
	SET_STYLEBOX( SB_BUTTON_NORMAL, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_BUTTON_PRESSED, StyleBox( 1, Color( 0xad,0x8a,0x68 ), Color( 0x7c,0x55,0x3d ),Color( 0xfa,0xfc,0xd6 ) ) );
	SET_STYLEBOX( SB_BUTTON_HOVER, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_BUTTON_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	
	SET_STYLEBOX( SB_BUTTON_CHECKED, StyleBox() ); //of no use here by default
	SET_STYLEBOX( SB_BUTTON_UNCHECKED, StyleBox() ); //of no use here by default
	
	SET_CONSTANT( C_BUTTON_SEPARATION, 0 );
	SET_CONSTANT( C_BUTTON_EXTRA_MARGIN, 1 );
	SET_CONSTANT( C_BUTTON_DISPLACEMENT, 0 );
	SET_CONSTANT( C_BUTTON_LABEL_ALIGN_CENTER, 1 );
	SET_CONSTANT( C_BUTTON_HAS_CHECKBOX, 0 );
	SET_CONSTANT( C_BUTTON_CHECKBOX_SIZE, 0 );
	
	SET_BITMAP( BITMAP_BUTTON_CHECKED , -1 );
	SET_BITMAP( BITMAP_BUTTON_UNCHECKED , -1 );
		
	SET_COLOR(COLOR_BUTTON_FONT,Color(0));
	
	/* CheckButton */
	
		
	SET_STYLEBOX( SB_CHECKBUTTON_NORMAL, StyleBox() ); //of no use here by default
	SET_STYLEBOX( SB_CHECKBUTTON_PRESSED, StyleBox() ); //of no use here by default
	SET_STYLEBOX( SB_CHECKBUTTON_HOVER, StyleBox( 0, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	
	SET_STYLEBOX( SB_CHECKBUTTON_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	
	SET_STYLEBOX( SB_CHECKBUTTON_CHECKED, StyleBox( 1, Color( 0xdd,0xaa,0x98 ), Color( 0,0,0 ), Color( 0,0,0 ) ) ); 
	SET_STYLEBOX( SB_CHECKBUTTON_UNCHECKED, StyleBox( 0, Color( 0,0,0 ), Color( 0,0,0 ), Color( 0,0,0 ) ) ); 
	
	SET_CONSTANT( C_CHECKBUTTON_SEPARATION, 2 );
	SET_CONSTANT( C_CHECKBUTTON_EXTRA_MARGIN, 2 );
	SET_CONSTANT( C_CHECKBUTTON_DISPLACEMENT, 0 );
	SET_CONSTANT( C_CHECKBUTTON_CHECKBOX_SIZE, 5 );
	
	SET_BITMAP( BITMAP_CHECKBUTTON_CHECKED , -1 );
	SET_BITMAP( BITMAP_CHECKBUTTON_UNCHECKED , -1 );
	
	
	SET_COLOR(COLOR_CHECKBUTTON_FONT,Color(0));
	
	/* MenuButton */
	

	SET_STYLEBOX( SB_MENUBUTTON_NORMAL, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_MENUBUTTON_PRESSED, StyleBox( 1, Color( 0xad,0x8a,0x68 ), Color( 0x7c,0x55,0x3d ),Color( 0xfa,0xfc,0xd6 ) ) );
	SET_STYLEBOX( SB_MENUBUTTON_HOVER, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_MENUBUTTON_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	
	SET_STYLEBOX( SB_MENUBUTTON_CHECKED, StyleBox( 1, Color( 0xdd,0xaa,0x98 ), Color( 0,0,0 ), Color( 0,0,0 ) ) ); 
	SET_STYLEBOX( SB_MENUBUTTON_UNCHECKED, StyleBox( 0, Color( 0,0,0 ), Color( 0,0,0 ), Color( 0,0,0 ) ) ); 
		
	SET_CONSTANT( C_MENUBUTTON_SEPARATION, 1 );
	SET_CONSTANT( C_MENUBUTTON_EXTRA_MARGIN, 1 );
	SET_CONSTANT( C_MENUBUTTON_DISPLACEMENT, 0 );
	SET_CONSTANT( C_MENUBUTTON_CHECKBOX_SIZE, 5 );
	
	SET_BITMAP( BITMAP_MENUBUTTON_CHECKED , -1 );
	SET_BITMAP( BITMAP_MENUBUTTON_UNCHECKED , -1 );
	
	
	SET_COLOR(COLOR_MENUBUTTON_FONT,Color(0));

	
	/* WindowButton */
	

	SET_CONSTANT(C_WINDOWBUTTON_DISPLACEMENT,1);
	SET_BITMAP(BITMAP_WINDOWBUTTON_CLOSE,-1);
	SET_BITMAP(BITMAP_WINDOWBUTTON_MAXIMIZE,-1);
	SET_BITMAP(BITMAP_WINDOWBUTTON_MINIMIZE,-1);
	
	SET_STYLEBOX( SB_WINDOWBUTTON_NORMAL, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_WINDOWBUTTON_PRESSED, StyleBox( 1, Color( 0xad,0x8a,0x68 ), Color( 0x7c,0x55,0x3d ),Color( 0xfa,0xfc,0xd6 ) ) );
	SET_STYLEBOX( SB_WINDOWBUTTON_HOVER, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_WINDOWBUTTON_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	
	SET_COLOR(COLOR_WINDOWBUTTON_FONT,0);
	/* Option button */


	SET_STYLEBOX( SB_OPTIONBUTTON_NORMAL, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_OPTIONBUTTON_PRESSED, StyleBox( 1, Color( 0xad,0x8a,0x68 ), Color( 0x7c,0x55,0x3d ),Color( 0xfa,0xfc,0xd6 ) ) );
	SET_STYLEBOX( SB_OPTIONBUTTON_HOVER, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_OPTIONBUTTON_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	
	SET_CONSTANT( C_OPTIONBUTTON_SEPARATION, 1 );
	SET_CONSTANT( C_OPTIONBUTTON_EXTRA_MARGIN, 1 );
	SET_CONSTANT( C_OPTIONBUTTON_DISPLACEMENT, 0 );



	SET_COLOR(COLOR_OPTIONBUTTON_FONT,Color(0));
	
		
	/* Menu Box */


	SET_STYLEBOX( SB_MENUBOX_NORMAL, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_MENUBOX_PRESSED, StyleBox( 1, Color( 0xad,0x8a,0x68 ), Color( 0x7c,0x55,0x3d ),Color( 0xfa,0xfc,0xd6 ) ) );
	SET_STYLEBOX( SB_MENUBOX_HOVER, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_MENUBOX_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	

	SET_CONSTANT( C_MENUBOX_SEPARATION, 1 );
	SET_CONSTANT( C_MENUBOX_EXTRA_MARGIN, 1 );
	SET_CONSTANT( C_MENUBOX_DISPLACEMENT, 0 );


	SET_COLOR(COLOR_MENUBOX_FONT,Color(0));
	
	/* Arrow Button */

	SET_STYLEBOX( SB_ARROWBUTTON_NORMAL, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_ARROWBUTTON_PRESSED, StyleBox( 1, Color( 0xad,0x8a,0x68 ), Color( 0x7c,0x55,0x3d ),Color( 0xfa,0xfc,0xd6 ) ) );
	SET_STYLEBOX( SB_ARROWBUTTON_HOVER, StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) ) );
	SET_STYLEBOX( SB_ARROWBUTTON_FOCUS, StyleBox( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) ) );
	
	
	SET_CONSTANT( C_ARROWBUTTON_DISPLACEMENT, 0 );
	
	SET_CONSTANT( C_ARROWBUTTON_ARROW_SIZE, 5);

	SET_COLOR( COLOR_ARROWBUTTON_ARROW_COLOR, Color(0,0,0) );
	
	/* UpDown */
	
	SET_CONSTANT( C_UPDOWN_ARROW_SIZE, 3 );
	SET_CONSTANT( C_UPDOWN_MARGIN, 1 );
	SET_BITMAP( BITMAP_UPDOWN, -1 );
	SET_COLOR( COLOR_UPDOWN, Color(0,0,0) );
		
	
	/* Line Edit */

	SET_COLOR( COLOR_LINE_EDIT_CURSOR, Color( 255,255,255 ) );
	SET_COLOR( COLOR_LINE_EDIT_SELECTION, Color( 0xfa,0xfc,0xd6 ) );
	SET_STYLEBOX( SB_LINE_EDIT_NORMAL, StyleBox( 1,  Color(0,0,0 ), Color( 0,0,0 ), Color( 0,0,0 ) ) );
	SET_STYLEBOX( SB_LINE_EDIT_FOCUS, StyleBox( 1,  Color(0,0,0 ), Color( 255,80,80 ), Color( 255,80,80 ) ) );

	SET_CONSTANT( C_LINE_EDIT_MARGIN, 1 );


	
	SET_COLOR(COLOR_LINE_EDIT_FONT,Color( 0xfa,0xfc,0xd6 ));
	SET_COLOR(COLOR_LINE_EDIT_FONT_SELECTED,Color(0));
	
	/* Text Edit */

	SET_COLOR( COLOR_TEXT_EDIT_CURSOR, Color( 0,0,0 ) );
	SET_COLOR( COLOR_TEXT_EDIT_SELECTION, Color( 0xfa,0xfc,0xd6 ) );
	SET_STYLEBOX( SB_TEXT_EDIT, StyleBox( 1,  Color(0,0,0 ), Color( 0,0,0 ), Color( 0,0,0 ) ) );
	SET_STYLEBOX( SB_TEXT_EDIT_FOCUS, StyleBox( 1,  Color(0,0,0 ), Color( 255,80,80 ), Color( 255,80,80 ) ) );

	SET_CONSTANT(C_TEXT_EDIT_LINE_SPACING,1);
	
	SET_COLOR(COLOR_TEXT_EDIT_FONT,Color( 0xfa,0xfc,0xd6 ));
	SET_COLOR(COLOR_TEXT_EDIT_FONT_SELECTED,Color(0));
	
	
	
	/* Tab Bar */
	
	SET_STYLEBOX( SB_TABBAR_LEFT, StyleBox() );
	SET_STYLEBOX( SB_TABBAR_RIGHT, StyleBox() );
	StyleBox tb_tab=StyleBox( 1, Color( 0xbd,0x9a,0x78 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) );
	for (int i=0;i<4;i++)
		tb_tab.margins[i]=2;

	SET_STYLEBOX( SB_TABBAR_TAB, tb_tab );
	
	tb_tab=StyleBox( 1, Color( 0xed,0xea,0xa8 ),Color( 0xfa,0xfc,0xd6 ), Color( 0x7c,0x55,0x3d ) );
	for (int i=0;i<4;i++)
		tb_tab.margins[i]=2;
	
	SET_STYLEBOX( SB_TABBAR_RAISED, tb_tab  );

	
	StyleBox tb_focus( 1, Color( 0,0,0 ), Color( 255,50,50 ), Color( 255,50,50 ) );
	tb_focus.draw_center=false;
	SET_STYLEBOX( SB_TABBAR_FOCUS, tb_focus );
		
	SET_CONSTANT(C_TABBAR_CENTERED,0);
	SET_CONSTANT(C_TABBAR_TAB_SEPARATOR,0);
	
	SET_COLOR(COLOR_TABBAR_FONT_RAISED,Color(0));
	SET_COLOR(COLOR_TABBAR_FONT,Color(0));

	/* Tab Box */
	
	StyleBox tab_box( 0, Color( 0xbd,0x9a,0x78 ), Color( 0xbd,0x9a,0x78 ), Color( 0xbd,0x9a,0x78 ) );
	SET_STYLEBOX(SB_TABBOX, tab_box );
	SET_CONSTANT( C_TABBOX_SEPARATION, 0 );
	
	/* Combo Box */
	
	SET_CONSTANT(C_COMBOBOX_POPUP_MAX_HEIGHT,100);
	SET_STYLEBOX(SB_COMBO_POPUP,StyleBox( 2, Color( 150,150,150 ), Color( 250,250,250 ), Color( 50,50,50 ) ));
	
	
}



