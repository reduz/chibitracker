/***************************************************************************
    This file is part of the CheeseTronic Music Tools
    url                  : http://reduz.com.ar/cheesetronic
    copyright            : (C) 2003 by Juan Linietsky
    email                : reduzio@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "base/painter.h"
#include "ctskin.h"
#include <math.h>
#include "envelope_point_editor.h"


static const float MIN_ZOOM=0.2;
static const float ZOOM_ADJUST_MARGIN_MIN=30.0; //percent of screen
static const float ZOOM_ADJUST_MARGIN_MAX=5.0; //percent of screen


int EnvelopePointEditor::get_font_h_spacing() {
	
	return get_painter()->get_font_height( font(FONT_ENVELOPE) )*2+6; //?
}


void EnvelopePointEditor::resize(const Size& p_new_size) {
	
	size=p_new_size;	
}
void EnvelopePointEditor::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	if (!envelope)
		return;
	
	int cur_height=size.height-get_font_h_spacing();
	float env_normalize=-envelope->get_min();
	float env_total=envelope->get_max()-envelope->get_min();
			
	if (!p_press) { //release
		
		
		if (grab.moving_point && p_button==BUTTON_LEFT) {

			grab.moving_point=false;
			update();
		}
		
	} else {
		
		
		
		float gx=p_pos.x,gy=p_pos.y;

		// check if we grabbed anything.. distance in pixels
		int closest_node=-1;
		float closest_node_dist=1000;
		for (int i=0;i<envelope->get_node_count();i++) {


			float node_x,node_y;

			node_x=(float)envelope->get_node(i).tick_offset;
			node_y=(float)envelope->get_node(i).value;

			node_x/=zoom;
			node_y+=env_normalize;
			node_y=node_y*(float)cur_height/env_total;
			node_y=cur_height-node_y;

			float distance=sqrt(pow(node_x-gx,2)+pow(node_y-gy,2));

			if ( (distance<closest_node_dist) && (distance<(float)MIN_DRAG_DISTANCE)) {

				closest_node_dist=distance;
				closest_node=i;
			}

		}

		switch (p_button) {

			case BUTTON_LEFT: {

				// check the fish
				if (closest_node!=-1) {

					//convert to local coords

					grab.moving_point=true;
					grab.node=closest_node;
					grab.x=envelope->get_node(grab.node).tick_offset;
					grab.y=envelope->get_node(grab.node).value;

					move_mouse_to_selected_node();
				} else { //couldnt find close node, adding the point!


					gx*=zoom; // to env coordinates
					gy=(cur_height-gy)*env_total/(float)cur_height;
					gy-=env_normalize;



					int which=envelope->add_position((int)gx,(int)gy);


					if (which>=0) {
						grab.moving_point=true;

						grab.node=which;
						grab.x=(int)gx;
						grab.y=(int)gy;
						move_mouse_to_selected_node();
						update();
						envelope_changed.call();;

					}
				}

			} break;
			case BUTTON_RIGHT: {

				if (closest_node!=-1) {


					envelope->del_position(closest_node);

					update();
					envelope_changed.call();
					adjust_zoom_to_window();
					
					if (!envelope->is_enabled()) {
						
						envelope->set_enabled( true );
						envelope_changed.call();
					}
				}
			} break;
			default: {}
		}
		
		
		
	}
}

void EnvelopePointEditor::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {
	
	if (!envelope)
		return;

	if (!grab.moving_point)
		return;
	

	int cur_height=size.height-get_font_h_spacing();
	float env_normalize=-envelope->get_min();
	float env_total=envelope->get_max()-envelope->get_min();
	
	float gx=p_pos.x,gy=p_pos.y;
	gx*=zoom;

	gy=float(cur_height-gy)*env_total/(float)cur_height;
	gy-=env_normalize;

	if ( ((int)gx!=envelope->get_node(grab.node).tick_offset) || ((int)gy!=envelope->get_node(grab.node).value) ) {

			//avoid pointless overdraw
		
		envelope->set_position(grab.node,(int)gx,(int)gy);
		
		if (adjust_zoom_to_window())
			move_mouse_to_selected_node();
		update();
		
		if (!envelope->is_enabled()) {
						
			envelope->set_enabled( true );
			envelope_changed.call();
		}
		
	}
	
}

bool EnvelopePointEditor::adjust_zoom_to_window() {

	float cur_width=size.width;
	float env_len=envelope->get_node(envelope->get_node_count()-1).tick_offset;

	float adjust_margin_min=ZOOM_ADJUST_MARGIN_MIN*cur_width/100.0;
	float adjust_margin_max=ZOOM_ADJUST_MARGIN_MAX*cur_width/100.0;

	bool changed=false;

	float old_zoom=zoom;

        if ( (env_len/zoom) < (cur_width-adjust_margin_min) ) {

        	zoom=env_len/(cur_width-adjust_margin_max);
	}

        if ( (env_len/zoom) > (cur_width-adjust_margin_max) ) {

        	zoom=env_len/(cur_width-adjust_margin_min);
	}


	if (zoom<MIN_ZOOM) {
		zoom=MIN_ZOOM;
	}

	if (zoom!=old_zoom)
		changed=true;

	return changed;

}

void EnvelopePointEditor::move_mouse_to_selected_node() {

//	int cur_width=size.width;
	int cur_height=size.height-get_font_h_spacing();
//	int real_height=size.height;

	if (cur_height<=0)
		return;

        float env_normalize=-envelope->get_min();
	float env_total=envelope->get_max()-envelope->get_min();
	if (env_total==0)
		return; //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;


	float node_x=(float)envelope->get_node(grab.node).tick_offset;
	float node_y=(float)envelope->get_node(grab.node).value;

	node_x/=zoom;
	node_y+=env_normalize;
	node_y=node_y*(float)cur_height/env_total;
	node_y=cur_height-node_y;

	get_painter()->set_cursor_pos(get_global_pos()+Point((int)node_x,(int)node_y));
	//QCursor::setPos(mapToGlobal(QPoint((int)node_x,(int)node_y)));
}


void EnvelopePointEditor::set_envelope(Envelope *p_envelope) {

	envelope=p_envelope;
	adjust_zoom_to_window();
	recalc_pending=true;
	update();
}


void EnvelopePointEditor::paint_envelope_points(Painter *p) {

	int cur_height=size.height-get_font_h_spacing();
	int cur_width=size.width;
	int real_height=size.height;


	if (cur_height<=0)
		return;

        float env_normalize=-envelope->get_min();
	float env_total=envelope->get_max()-envelope->get_min();

	if (envelope->is_loop_enabled()) {

		float node_begin,node_end;

		node_begin=envelope->get_node( envelope->get_loop_begin() ).tick_offset/zoom;
		node_end=envelope->get_node( envelope->get_loop_end() ).tick_offset/zoom;

		p->draw_fill_rect(Point((int)node_begin,0),Point((int)1,cur_height),color(COLOR_ENVELOPE_LOOP));
		p->draw_fill_rect(Point((int)node_end,0),Point((int)1,cur_height),color(COLOR_ENVELOPE_LOOP));

	}

	if (envelope->is_sustain_loop_enabled()) {

		float node_begin,node_end;

		node_begin=envelope->get_node( envelope->get_sustain_loop_begin() ).tick_offset/zoom;
		node_end=envelope->get_node( envelope->get_sustain_loop_end() ).tick_offset/zoom;

		p->draw_fill_rect(Point((int)node_begin,0),Point((int)1,cur_height),color(COLOR_ENVELOPE_SUSTAIN_LOOP));
		p->draw_fill_rect(Point((int)node_end,0),Point((int)1,cur_height),color(COLOR_ENVELOPE_SUSTAIN_LOOP));
		
	}

	
	p->draw_fill_rect(Point(0,cur_height),Size(cur_width,real_height-cur_height),color(COLOR_ENVELOPE_BG));


	for (int i=0;i<envelope->get_node_count();i++) {


		float node_x,node_y;

		node_x=(float)envelope->get_node(i).tick_offset/zoom;

		node_y=(float)envelope->get_node(i).value;
		node_y+=env_normalize;
		node_y=node_y*(float)cur_height/env_total;
		node_y=cur_height-node_y;

		p->draw_rect(Point((int)node_x-2,(int)node_y-2),Size(6,6),color(COLOR_ENVELOPE_POINT)); //??

	}


//	painter.setPen(QColor(100, 200, 100));

	/* This code calculates the increment for displaying the numbers, so they dont overlap */
	
	int char_w=p->get_font_char_width( font(FONT_ENVELOPE), 'X');
	int font_h=p->get_font_height( font(FONT_ENVELOPE) );
	int font_ascent=p->get_font_ascent( font(FONT_ENVELOPE) );
	float avg_string_width=char_w*5;

	float increment=1.0;
	int step=0;

	while ((increment/zoom)<avg_string_width) {


		if (step % 2)
			increment*=2;
		else
			increment*=5;
		step++;

	}

	
			
        for (float i=0.0f;i<(cur_width*zoom);i+=increment) {

		int real_screen_offset=int((float)i/zoom);

		String num=String::num(i);
		int num_w=num.length()*char_w;
		
		if (i>0.0f)
			p->draw_text(font(FONT_ENVELOPE),Point(real_screen_offset-num_w/2,cur_height+font_ascent),num,color(COLOR_ENVELOPE_FONT));
	}


	String str;

	if (grab.moving_point) {

		str="Zoom: " + String::num(int(zoom*100.0)) +"%. Moving #" + String::num(grab.node+1) +"/" +String::num(envelope->get_node_count()) +", pos: " + String::num(envelope->get_node(grab.node).tick_offset) +","+ String::num(envelope->get_node(grab.node).value);
	} else {

		str="Zoom: " + String::num(int(zoom*100.0)) +"%. "+String::num(envelope->get_node_count())+" Nodes.";

	}


	p->draw_text(font(FONT_ENVELOPE),Point(2,cur_height+font_ascent+font_h),str,color(COLOR_ENVELOPE_FONT));

}

void EnvelopePointEditor::draw_envelope_pos(int p_x,Painter *p) {

//	int cur_width=size.width;
	int cur_height=size.height-get_font_h_spacing()-1;
//	int real_height=size.height;

	if (cur_height<=0)
		return;

        float env_normalize=-envelope->get_min();
	float env_total=envelope->get_max()-envelope->get_min();
	if (env_total==0)
		return; //canut be draw, broken envelope
	int center=int(env_normalize*(float)cur_height/env_total);
	center=cur_height-center;

	
	
	p->draw_fill_rect(Point(p_x,0),Size(1,cur_height),color(COLOR_ENVELOPE_BG));


	int real_x_offset=int(p_x*zoom);
	int prev_real_x_offset=int((p_x-1)*zoom);

	int line_size=1;


	if (real_x_offset!=prev_real_x_offset) {

		if ((real_x_offset % 5)==0) {

			line_size=3;
		}

		if ((real_x_offset % 10)==0) {

			line_size=5;
		}

		if ((real_x_offset % 50)==0) {

			line_size=7;
		}

		if ((real_x_offset % 100)==0) {

			line_size=9;
		}

		if ((real_x_offset % 500)==0) {

			line_size=11;
		}
	}


	int end=center+(line_size/2);

	if (end>=(cur_height))
		end=cur_height;

	int begin=center-(line_size/2);
	p->draw_fill_rect(Point(p_x,begin),Size(1,end-begin+1),color(COLOR_ENVELOPE_SCALE));


	float val=envelope->get_interp_height_at_pos((float)p_x*zoom);

	if (p_x*zoom>envelope->get_node(envelope->get_node_count()-1).tick_offset)
		return; //dont draw what it's pointless to draw

	
	val+=env_normalize;
	val=val*(float)cur_height/env_total;
	val=cur_height-val;

	float prev_val=envelope->get_interp_height_at_pos((float)(p_x-1)*zoom);
	prev_val+=env_normalize;
	prev_val=prev_val*(float)cur_height/env_total;
	prev_val=cur_height-prev_val;


	Color line_c=color(COLOR_ENVELOPE_LINE);
	// "El-Cheapo" Antialiasing..
	Color bg=color(COLOR_ENVELOPE_BG);
	line_c.r=((int)line_c.r+(int)bg.r)/2;
	line_c.g=((int)line_c.g+(int)bg.g)/2;;
	line_c.b=((int)line_c.b+(int)bg.b)/2;;

	int from=(int)prev_val;
	int to=(int)val;
	if (from>to) {
		
		int aux=from;
		from=to;
		to=aux;
	}
	to+=1;
	p->draw_fill_rect(Point(p_x,from-1),Size(1,(to-from)+2),line_c); //must make it better

	p->draw_fill_rect(Point(p_x,from),Size(1,to-from),color(COLOR_ENVELOPE_LINE)); //must make it better

	
}

void EnvelopePointEditor::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {


	if (envelope==NULL) return;

	if (recalc_pending) {
		adjust_zoom_to_window();
		recalc_pending=false;
	}



	Painter *p=get_painter();


	for (int i=0;i<size.width;i++) {

		draw_envelope_pos(i,p);
	}

	paint_envelope_points(p);

	int cur_height=size.height-get_font_h_spacing()-1;
	
	Player *pl=editor->get_player();
	for (int i=0;i<pl->get_virtual_channels();i++) {
		
		int voice_pos=-1;
		
		
		if (pl->get_voice_envelope( i, Instrument::VOLUME_ENVELOPE)==envelope ) {
			
			voice_pos=pl->get_voice_envelope_pos(i,Instrument::VOLUME_ENVELOPE);
			
		} else if (pl->get_voice_envelope( i, Instrument::PAN_ENVELOPE)==envelope ) {
			
			voice_pos=pl->get_voice_envelope_pos(i,Instrument::PAN_ENVELOPE);
			
		} else if (pl->get_voice_envelope( i, Instrument::PITCH_ENVELOPE)==envelope ) {
			
			voice_pos=pl->get_voice_envelope_pos(i,Instrument::PITCH_ENVELOPE);
		}
			
		if (voice_pos==-1)
			continue;

		voice_pos=(int)((float)voice_pos/zoom);
		if (voice_pos>=size.width)
			continue;
		
		p->draw_fill_rect(Point(voice_pos,0),Point(1,cur_height),color(COLOR_ENVELOPE_LINE));		
	}

}





EnvelopePointEditor::EnvelopePointEditor(Editor *p_editor) {

	editor=p_editor;
	set_bg_on_updates( false );
	envelope=NULL;
	zoom=0.25f;

	grab.moving_point=false;
	grab.node=-1;
	grab.x=0;
	grab.y=0;
	recalc_pending=true;
	
}

EnvelopePointEditor::~EnvelopePointEditor()
{
}
