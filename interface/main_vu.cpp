//
// C++ Implementation: main_vu
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "base/painter.h"
#include "main_vu.h"
#include "ctskin.h"

void MainVU::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	
	Color bg=color(COLOR_VU_BG);
	int last=0;
	for (int i=0;i<(p_size.width>>1);i++) {
		
		int pos=i*2;
		
		int valatpos=pos*VU_MAX/p_size.width;
		
		Color min=color(COLOR_VU_MIN);
		Color max=color(COLOR_VU_MAX);
		
		Color col;
		col.r=(int)min.r+((int)max.r-(int)min.r)*valatpos/VU_MAX;
		col.g=(int)min.g+((int)max.g-(int)min.g)*valatpos/VU_MAX;
		col.b=(int)min.b+((int)max.b-(int)min.b)*valatpos/VU_MAX;
		
		if (valatpos<=val_l) {
			
			get_painter()->draw_fill_rect( Point( i<<1, 0 ), Size(1,p_size.height >> 1), col );
		} else {
			
			get_painter()->draw_fill_rect( Point( i<<1, 0 ), Size(1,(p_size.height >> 1)), bg );
			
		}
		if (valatpos<=val_r) {
			
			get_painter()->draw_fill_rect( Point( i<<1, p_size.height >> 1), Size(1,p_size.height >> 1), col );
		} else {
			
			get_painter()->draw_fill_rect( Point( i<<1, p_size.height >> 1), Size(1,p_size.height >> 1), bg );
			
		}
		
		last=(i<<1)+1;
		if (valatpos>val_l && valatpos>val_r) {
			break;
		}
		
		get_painter()->draw_fill_rect( Point( (i<<1) +1, 0), Size(1,p_size.height ), bg );
		
		
	}
	
	if (last<p_size.width) {
		
		get_painter()->draw_fill_rect( Point( last, 0), Size(p_size.width-last,p_size.height ), bg );
		
	}
	get_painter()->draw_fill_rect( Point( 0, p_size.height>>1), Size(p_size.width,1 ), bg );
	
}

void MainVU::set(int p_val_l,int p_val_r) {
	
	if (p_val_l>val_l)
		val_l=p_val_l;
	
	if (p_val_r>val_r)
		val_r=p_val_r;
	
	update();
}

void MainVU::compute(int p_time_elapsed_ms) {
	
	val_l-=p_time_elapsed_ms*VU_MAX/1000;
	if (val_l<0)
		val_l=0;
	val_r-=p_time_elapsed_ms*VU_MAX/1000;
	if (val_r<0)
		val_r=0;
}

MainVU::MainVU(){

	val_l=0;;
	val_r=0;

}


MainVU::~MainVU(){
	
	
}


