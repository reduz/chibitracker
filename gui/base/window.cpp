//
// C++ Implementation: window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "window.h"
#include "container.h"


#include <stdio.h>
namespace GUI {

void Window::set_focus(Frame *p_frame) {

	if (p_frame && p_frame->get_window()!=this)
		return;
	
	if (focus_child) {
		focus_child->focus_leave();
		focus_child->update();
	}
	focus_child=p_frame;
	if (p_frame) {
		focus_child->focus_enter();
		focus_child->update();
	}

}

void Window::redraw_screen_over_area(const Rect &p_rect) {


	Window *c=childs;

	redraw_contents_over_area(p_rect);
	
	while (c) {

		if (c->visible) {
			Rect crect=p_rect;
			crect.pos-=c->pos;
				
			c->redraw_screen_over_area(crect);
		}
				
		c=c->next;

	}


}

Point Window::get_global_pos() {

	return pos+(parent?parent->pos:Point());
}

void Window::set_tree_size_changed() {
	
	
}

Timer *Window::get_timer() {
	
	if (!root) {
		
		PRINT_ERROR("No root");
		return 0;
	}
	
	return root->root_data->timer;
}

Painter *Window::get_painter() {
	
	if (!root) {
		
		PRINT_ERROR("No root");
		return 0;
	}
	
	return root->root_data->painter;

}

void Window::set_root_frame(Container *p_root_frame) {

	if (root_frame)
		return;
	root_frame=p_root_frame;
	root_frame->set_window( this );
	p_root_frame->top_size_adjust_signal.connect( this, &Window::top_frame_resized );
	set_root_frame_internal(p_root_frame);
	
	set_size(size);

}

void Window::skin_changed() {
	
	
	
	if (root_frame) {
		

		root_frame->skin_changed();
		
		adjust_size_type();
	}
	
	Window *c=childs;
	
	while (c) {
		
		c->skin_changed();
		c=c->next;
	}
		
	skin_changed_signal.call();
}


Window *Window::get_root() {
	
	return root;
}
Frame * Window::get_root_frame() {
	
	return root_frame;
}

void Window::set_pos(const Point &p_pos) {

	/* WARRRNING - moving stuff */
	
	update( Rect( get_global_pos(), size ) );
	pos=p_pos;
	update( Rect( get_global_pos(), size ) );
	
}

void Window::top_frame_resized(Size p_size) {
	
	set_size(p_size);
	
}

void Window::set_size(const Size& p_size) {

	
	if (!root_frame) {
		size=p_size;
		return;
	}
	
	
	Size new_size = root_frame->get_minimum_size();
	
	if (!no_stretch_root_frame)  {
		
		if (p_size.width > new_size.width)
			new_size.width=p_size.width;
		if (p_size.height > new_size.height)
			new_size.height=p_size.height;
	}
	
	
	
	size=new_size;
	root_frame->resize_tree( new_size );
	Rect size_update( get_global_pos(), size );
	
	size_update.merge( Rect( size_update.pos, new_size) );
	
	update( size_update );
	
	
}

Size Window::get_size() {
	
	return size;
}
void Window::redraw_all(Rect p_custom_area) {


	if (!root_frame)
		return;

	get_painter()->reset_clip_rect_stack();
	get_painter()->reset_local_rect_stack();
	
	if (p_custom_area.has_no_area())
		redraw_screen_over_area(p_custom_area);
	else
		redraw_screen_over_area(Rect( get_global_pos(), size ));
}


void Window::redraw_contents_over_area(const Rect& p_rect) {


	/* Redrawing all should work fine */
	
	get_painter()->reset_clip_rect_stack();
	get_painter()->reset_local_rect_stack();
	
	
	
	Rect local_rect=Rect( get_global_pos(), size );
	
	
	get_painter()->push_local_rect( local_rect );
	Rect expose_rect = Rect( Point(),size ).clip( p_rect ); // create expose
	Rect clip_rect = Rect( p_rect.pos , p_rect.size );
	get_painter()->push_clip_rect( clip_rect );
	
	root_frame->draw_tree( local_rect.pos, local_rect.size, expose_rect );
	
	get_painter()->pop_local_rect();
	get_painter()->pop_clip_rect();
	
	/* debug 
	get_painter()->reset_clip_rect_stack();
	get_painter()->reset_local_rect_stack();
	
	get_painter()->draw_rect( local_rect.pos, local_rect.size, Color(255,0,255)); 
	get_painter()->draw_rect( local_rect.pos+clip_rect.pos, clip_rect.size, Color(0,255,255)); 
	*/

	
}


/*
void Window::check_for_deletes() {

	Window *c=childs;
	Window **cr=&childs;
	
	while (c) {

	
		if (c->queued_delete) {
			
			
			Window *ac=c;
			
			*cr=c->next;
			c=c->next;
			
			delete ac;
			continue;			
		} 
		
		c->check_for_deletes();
		
		cr=&c->next;
		c=c->next;
	}
	
}
*/

void Window::frame_request_resize(Frame *p_frame) {

	size_update_needed=true;
	size_update_from=NULL;

}

void Window::check_size_updates() {

	if (!visible)
		return;
		
	if (size_update_needed && root_frame) {
	
		Frame *from;
		 
		if (size_update_from) {
		
		 	from = size_update_from;		 			 	
		} else {
		
			from=root_frame;
		}
		
		if (from == root_frame) {
		
			Size rfminsize = root_frame->get_minimum_size();

			if (rfminsize.width > size.width || rfminsize.height > size.height)
				set_size(rfminsize);
			else
				from->resize_tree(size);
		
		} else {
		
			from->resize_tree( from->get_size_cache() );
		}
		
		size_update_needed=false;		
		size_update_from=NULL;
		
		update( Rect( from->get_global_pos(), from->get_size_cache() ) );

	}
	
	Window *ch = childs;
	
	while (ch) {
	
		ch->check_size_updates();
		ch=ch->next;
	}

}

void Window::check_for_updates() {

	/* For now, if the window has a child, it wont redraw */

	update_signal.call();
	update_signal.clear();
	frame_signal.call();
	
	if (!root || !root_data ) {
		
		PRINT_ERROR("Not Root!");
		return;
	}

	check_size_updates();
	
	root_data->update_rect_list_locked=true;
	
	get_painter()->reset_clip_rect_stack();
	get_painter()->reset_local_rect_stack();
	
	//int found=0;
	
	while(root_data->update_rect_list) {
		
		UpdateRectList *rect=root_data->update_rect_list;
		
		redraw_screen_over_area( rect->rect );
		get_painter()->update_screen_rect( rect->rect );
		
		root_data->update_rect_list = root_data->update_rect_list->next;
		GUI_DELETE( rect );
	//	found++;
	}
	
	//if (found!=0)
		//printf("have %i rects\n",found);
	
	root_data->update_rect_list_locked=false;
	
	root_data->update_rect_list=0;
	
}


Window *Window::find_window_at_pos(const Point& p_point) {
	
	
	if (!visible)
		return 0;
	Window * w = childs;
	
	Window *found=0;
	while (w) {
		
		Point rpos=p_point - w->pos;
		Window *wf = w->find_window_at_pos( rpos );
		if (wf)
			found=wf;
		
		
		w=w->next;
		
	}
	
	if (found)
		return found;
	
	//childs wont have it? then I do!
	if ( p_point.x >= 0 && p_point.y >= 0 && p_point.x < size.width && p_point.y < size.height)
		return this;
	
	return 0;	
	
}


void Window::mouse_doubleclick(const Point& p_pos,int p_modifier_mask) {
	

	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
		
		Point pos=p_pos-root->focus->get_global_pos();
		root->focus->mouse_doubleclick(pos,p_modifier_mask);
		return;
		
	}

	if (!root_frame)
		return;
	

	Point local_pos;
	Frame *child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
	
	if (!child)
		return; //nothing found!
	

	
	child->mouse_doubleclick( local_pos, p_modifier_mask );
	
}
void Window::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	
	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
		
		if (!p_press) {

			
			Point pos=p_pos-root->focus->get_global_pos();
				
			root->focus->mouse_button( pos , p_button, p_press, p_modifier_mask );
			return;
			
		} else {
			
			Window *at_click=find_window_at_pos(p_pos);

			bool post_popup_hide=false;
			while (root->root_data->modal_stack) {


				if (root->root_data->modal_stack->window->mode==MODE_POPUP && at_click!=root->root_data->modal_stack->window) {
					
					/* good bye popup */
						
					root->root_data->modal_stack->window->popup_cancel_signal.call();
					root->root_data->modal_stack->window->hide();
					

					post_popup_hide=true;
					continue; /* Try again, if there still is a modal stack */
					
				}
				
				/* ok, found modal, and has all the right to keep the event */

				Point pos=p_pos-root->root_data->modal_stack->window->get_global_pos();
				root->focus=root->root_data->modal_stack->window; //in case there is any doubt or the user did something stupid
				root->root_data->modal_stack->window->mouse_button( pos, p_button, p_press, p_modifier_mask );
				
				
				return;
			}
			
			if (at_click) { //clicked somewhere meaningful
				/* focus change */
				
				/* TODO proper focus change and window raise */
				focus=at_click;
				Point pos=p_pos-root->focus->get_global_pos();
				if (post_popup_hide)
					focus->mouse_motion( pos, Point(), 0 );
				focus->mouse_button( pos, p_button, p_press, p_modifier_mask );
				return;
			}
				
		}
			
	}
	

	
	if (!root_frame)
		return;
	
	Point local_pos;
	Frame *child=0;
	
	if (drag.child) {
		
		child=drag.child;
		local_pos=p_pos-drag.pos;
	} else {
		child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
	}
	
	
	if (!child)
		return; //nothing found!
	
	if (p_button<=3) {
		
		if (p_press) {

			drag.button_mask|=(1<<(p_button-1));
			if (!drag.child) {
				drag.child=child;
				drag.pos=p_pos-local_pos;
			} 
							
			/* Check if we gained focus on click */
			if (p_button==BUTTON_LEFT && child!=focus_child && child->get_focus_mode()!=FOCUS_NONE) {
				
				set_focus(child);
			}
			
		} else {
				
		
			drag.button_mask&=~(1<<(p_button-1));
			
			if (!drag.button_mask) { //not dragging anymore
				
				if (child==drag.child) { 
					
					//send event now
					child->mouse_button( local_pos, p_button, p_press, p_modifier_mask );
				
					/* try to find if child changed */
					child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
					
					if (last_under_cursor && last_under_cursor!=drag.child) {
						
						last_under_cursor->mouse_enter();
					}
					
					drag.child=0;
					
					return;
				}
			
				
			}
		}

	}
	child->mouse_button( local_pos, p_button, p_press, p_modifier_mask );
	
}
void Window::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {


	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
		
		Point pos=p_pos-root->focus->get_global_pos();
		
		root->focus->mouse_motion( pos , p_rel, p_button_mask );
		return;
		
	}
		
	if (!root_frame)
		return;
	
	Point local_pos;
	Frame *child=root_frame->get_child_at_pos( p_pos, size, &local_pos );
	
		
	/* Send enter/exit events */
	if (last_under_cursor!=child) {
		
		if (last_under_cursor && (!drag.child || last_under_cursor==drag.child))
			last_under_cursor->mouse_leave();
		
		if (child && (!drag.child || drag.child==child))
			child->mouse_enter();
		
		last_under_cursor=child;
	}
	
	if (drag.child) {
		
		child=drag.child;
		local_pos=p_pos-drag.pos;
	}
	
	if (!child)
		return; //nothing found!
	
	
	child->mouse_motion( local_pos, p_rel, p_button_mask );
	
}

void Window::key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {

	
	if (!parent && (root->focus!=this)) {
		//deliver to whoever has the focus, if root does not have it
				
		root->focus->key( p_unicode, p_scan_code, p_press, p_repeat, p_modifier_mask );
		return;
		
	}

	
	
	if (focus_child) {

		bool eat = focus_child->key( p_unicode, p_scan_code, p_press, p_repeat, p_modifier_mask );
	
		if (eat)
			return;
		
		
		if (p_press && p_scan_code==KEY_TAB  && !(p_modifier_mask&KEY_MASK_SHIFT)  && focus_child->get_parent()) {
	
			Frame *next_f=focus_child->get_parent()->next_in_focus( focus_child );
			if (next_f) {
	
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
			
			return;
		}
		
		if (p_press && p_scan_code==KEY_TAB && p_modifier_mask&KEY_MASK_SHIFT && focus_child->get_parent()) {
	
			Frame *prev_f=focus_child->get_parent()->prev_in_focus( focus_child );
			if (prev_f) {
	
				focus_child->focus_leave();
				focus_child->update();
				focus_child=prev_f;
				prev_f->focus_enter();
				prev_f->update();
				
			}
			
			return;
		}
		
		if (p_press && p_scan_code==KEY_UP) {
			
			int mindist=9999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos(),UP, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
		}
		
		if (p_press && p_scan_code==KEY_LEFT) {
			
			int mindist=99999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos(),LEFT, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
			
		}
		
		if (p_press && p_scan_code==KEY_RIGHT) {
			
			int mindist=9999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos()+Size(focus_child->get_assigned_size().width,0),RIGHT, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
		}
		
		if (p_press && p_scan_code==KEY_DOWN) {
			
			int mindist=9999999;
			Frame *next_f = root_frame->find_closest_focusable_to( focus_child->get_global_pos()+Size(0,focus_child->get_assigned_size().height),DOWN, mindist );
			if (next_f) {
				
				focus_child->focus_leave();
				focus_child->update();
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}			
		}
		
	} else {
		
		
		if (p_press && p_scan_code==KEY_TAB  && !(p_modifier_mask&KEY_MASK_SHIFT)) {
			
			Frame *next_f=root_frame->next_in_focus(0);
			if (next_f) {
				
				focus_child=next_f;
				next_f->focus_enter();
				next_f->update();
				
			}
			
			return;
		}
		
		if (p_press && p_scan_code==KEY_TAB && p_modifier_mask&KEY_MASK_SHIFT) {
			
			Frame *prev_f=root_frame->prev_in_focus(0);
			if (prev_f) {
				
				focus_child=prev_f;
				prev_f->focus_enter();
				prev_f->update();
				
			}
			
			return;
		}
		
	}
	
	if (p_press && p_scan_code==KEY_ESCAPE && parent && mode==MODE_POPUP && root->root_data->modal_stack && root->root_data->modal_stack->window==this) {
		
		popup_cancel_signal.call();
		hide();
					
	}
	/** No one handled the keypress, emiting it! */
	
	key_unhandled_signal.call(p_unicode,p_scan_code,p_press,p_repeat,p_modifier_mask);
	
}

void Window::clear_focus() {
	
	
}
void Window::frame_hide_notify(Frame *p_frame) {
	
	/* determine if focused frame is here */
	
	if (!focus_child)
		return;
	
	Frame *from=focus_child;
	bool found=false;
	
	while(from) {
		
		if (from==p_frame) {
			found=true;
			break;
		}
		from=from->get_parent();
	}
	
	if (!found)
		return; //nothing to do
	
	from->focus_leave();
	focus_child=root_frame->next_in_focus(); //see if something else can be focused
	
}

void Window::frame_deleted_notify(Frame *p_frame) {
	
	/* May seem like overload, but if a frame is deleted and has an window, I need to make sure
	it isnt used from here for anything else */
	
	if (last_under_cursor==p_frame)
		last_under_cursor=0;
	if (root_frame==p_frame)
		root_frame=0;
	if (drag.child==p_frame)
		drag.child=0;
	if (focus_child==p_frame)
		focus_child=0;
	if (size_update_from==p_frame)
		size_update_from=0;
	
}

void Window::update_rect_merge(UpdateRectList **p_rect) {
	
	
	UpdateRectList **ur=&root_data->update_rect_list;
	Rect testrect=(*p_rect)->rect;
	
	while (*ur) {
	
		
		if ((*ur!=*p_rect) && (*ur)->rect.intersects_with(testrect)) {
			//they intersect
			/* erase src rect from list */				UpdateRectList *to_erase=*p_rect;		
										
			//printf("ERASED %p, now %p points to %p\n",to_erase,p_rect,(*p_rect)->next);
			*p_rect=(*p_rect)->next;
			
			if (ur==&to_erase->next) { // in this case, ur will be invalidated
				
				ur=p_rect;
			}
			    
			GUI_DELETE( to_erase );
			/* Test wether a merge must be done */
			
			if ((*ur)->rect.contains( testrect ) ) //need to merge?
				return; //no.. nothing else needs to be done		
			(*ur)->rect= (*ur)->rect.merge( testrect );
			
			/* Once Merged, repeat update_rect_merge */
			//printf("MERGED %p\n",*ur);
			update_rect_merge(ur);
			//printf("MERGE END\n");
			return;
		}
		
		ur=&((*ur)->next);
	}
	
}

void Window::add_update_rect(const Rect& p_rect) {
	
	if (!root) {
		
		PRINT_ERROR("Not Root!");
		return;
	};
	
	if (root_data->update_rect_list_locked) {
		
		PRINT_ERROR("update rect list locked");
		return;
	}
	
	UpdateRectList *ur= GUI_NEW( UpdateRectList );
	ur->rect=p_rect;
	ur->next=root->root_data->update_rect_list;
	root->root_data->update_rect_list=ur;
	
	
	update_rect_merge(&root->root_data->update_rect_list);
	
}


void Window::update(const Rect& p_rect) {

	if (no_local_updates)
		return;
	
	Rect r=p_rect;
	if (parent) {
		
		r.pos+=get_global_pos();
	}
	
	root->add_update_rect( r );
	
/*	
	draw_widgets_over_area( p_rect );
	Rect global=p_rect;
	global.pos+=get_global_pos();
	
	get_painter()->update_screen_rect( global );	
*/
}

void Window::update() {
	
	get_painter()->update_screen();
}

Frame * Window::get_focused_child() {
	
	return focus_child;
}

/*
void Window::queue_delete() {


	queued_delete=true;
}
*/

void Window::initialize() {


	parent=0;
	
	
	root_frame=0;
	focus_child=0;
	last_under_cursor=0;
	drag.child=0;
	drag.button_mask=0;
	mode=MODE_NORMAL;
	size_mode=SIZE_NORMAL;
	visible=true;
	//queued_delete=false;
	next=0;
	childs=0;
	root=NULL;
	focus=this;
	
	size_update_needed=true;
	size_update_from=0;
	no_local_updates=false;
	no_stretch_root_frame=false;
	
	root_data=0;
}

void Window::set_no_local_updates(bool p_disable) {
	
	no_local_updates=p_disable;
}

void Window::set_no_stretch_root_frames(bool p_disable) {

	//no_stretch_root_frame=p_disable;
}

void Window::adjust_size_type() {

	if (!parent)
		return;

	switch (size_mode) {

		case SIZE_CENTER: {
			
			set_size( root_frame->get_minimum_size() );
			pos=(get_painter()->get_display_size()-size)/2;
			if (parent)
				pos-=parent->get_global_pos();
			
		} break;
		case SIZE_TOPLEVEL_CENTER: {
			
			set_size( get_painter()->get_display_size()*3/4 );
	
			pos=(get_painter()->get_display_size()-size)/2;
			if (parent)
				pos-=parent->get_global_pos();
			
		} break;
		case SIZE_NORMAL: {
			
			/* Adjust so it doesnt show out of window */
			
			Point glob_pos=get_global_pos();
			Point root_size=get_painter()->get_display_size(); //more reliable than window
			
			if ( glob_pos.y<0 )				
				pos.y-=glob_pos.y;
			if ( glob_pos.x<0 )				
				pos.x-=glob_pos.x;
			
			if ( (glob_pos.y+size.y)>root_size.y)
				pos.y-=(glob_pos.y+size.y)-root_size.y;
			if ( (glob_pos.x+size.x)>root_size.x)
				pos.x-=(glob_pos.x+size.x)-root_size.x;
		} break;
		
	}

}

void Window::show() {

	if (visible)
		return;
	
	if (mode==MODE_MODAL || mode==MODE_POPUP) {

		
		if (root && root->focus && root->focus->last_under_cursor) {
			
			root->focus->last_under_cursor->mouse_leave();	
			root->focus->last_under_cursor=0;
		}
		
		if (root->focus)
			root->focus->drag.child=0; //disable drag on whoever, just in case we clicked
		root->focus=this;
		ModalStack * ms = GUI_NEW( ModalStack );
		ms->window=this;
		ms->next=root->root_data->modal_stack;
		root->root_data->modal_stack=ms;
		raise();
	}

	/* Adjust size */
	if (parent && root_frame) {
		
		adjust_size_type();
	}
	
	visible=true;
	drag.child=0;
	
	if (!parent)
		return;
	
	if (root_frame && !focus_child) {
		Frame *prev_f=root_frame->prev_in_focus(0);
		if (prev_f) {
			
			focus_child=prev_f;
			prev_f->focus_enter();
			prev_f->update();
			
		}	
	}
	
	update( Rect( Point(), size) );
	
}

void Window::raise_window(Window *p_child) {


	
	bool found=false;

	Window *w=childs;
	Window **wr=&childs;


	if (w && !w->next) {//pointless, have only one child

		return;
	}
	
	while (w) {



		if (w==p_child) {

			if (!w->next) {
				

				return; //also pointless, already at end
			}

//			printf("RAISE found!\n");
			found=true;

			*wr=w->next;
			w=w->next;
			continue;
		}

		if (!w->next && found) {

//			printf("RAISE at end!\n");
			p_child->next=0;
			w->next=p_child;

		}

		wr=&w->next;
		w=w->next;

	}

	
	if (found) {
		//repaint!

	}
	
}

void Window::raise() {

	if (parent)
		parent->raise_window(this);

}

void Window::remove_from_modal_stack() {

	if (mode!=MODE_MODAL && mode!=MODE_POPUP)
		return; //otherwise it will never be in the modal stack
	if (!root)
		return;
	

	ModalStack * m = root->root_data->modal_stack;
	ModalStack ** mr = &root->root_data->modal_stack;


	while (m) {


		if (m->window==this) {


			ModalStack *msa=m;
			*mr=m->next;
			m=m->next;
			continue;
				
		}

		mr=&m->next;
		m=m->next;
			
	}

	if (root->focus==this) {
		if (root->root_data->modal_stack)
			root->focus=root->root_data->modal_stack->window;
		else if (parent)
			root->focus=parent;

	}
		
}

void Window::hide() {

	remove_from_modal_stack();
	visible=false;
	if (!parent)
		return;

	update( Rect( Point(), size) );

	if (focus_child)
		focus_child->window_hid();

	//parent->update(Rect(pos,size));
}

Skin *Window::get_skin() {
	
	if (!root) {
		
		PRINT_ERROR("No root");
		return NULL;
	}
		
	return root->root_data->skin;	
	
}

Window::Mode Window::get_mode() {
		
	return mode;	
}

bool Window::is_visible() {
	
	return visible;
}

Window::Window(Window *p_parent,Mode p_mode, SizeMode p_size_mode) {

	
	initialize();
	mode=p_mode;
	size_mode=p_size_mode;
	root=p_parent->root;
	focus=0;
	
	if (p_mode==MODE_MODAL || p_mode==MODE_POPUP) {

		visible=false;

	}

	
	parent=p_parent;
	
	next=p_parent->childs;
	p_parent->childs=this;

	
}

Window::Window(Painter *p_painter,Timer *p_timer,Skin *p_skin) {


	
	initialize();

	root=this;
	root_data = GUI_NEW( RootWindowData );
	root_data->painter=p_painter;
	root_data->skin=p_skin;
	root_data->timer=p_timer;

}

Window::~Window() {

	remove_from_modal_stack();

	if (root->focus==this) {
		
		root->focus=parent;
	}
	
//	if (parent)
		//parent->child_window.child=0; //parent lost a child
	
	// get rid of childs first 
	
	if (root_frame) {
		GUI_DELETE( root_frame );
	}
	
	
	while (childs) {
		
		GUI_DELETE(childs);	
	}
	
	if (parent) {
		
		Window **w=&parent->childs;
		while (*w) {
		
			if (*w==this) {
			
				*w=(*w)->next;
				break;
			}
		
			w=&(*w)->next;
		}
		
	}
	
	
	if (root_data) {
		GUI_DELETE( root_data );
	}
}



}
