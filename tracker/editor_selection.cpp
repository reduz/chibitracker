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
/***************************************************************************
                          editor_selection.cpp  -  description
                             -------------------
    begin                : Thu Jan 25 2001
    copyright            : (C) 2001 by Juan Linietsky
    email                : reduz@anime.com.ar
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "editor.h"

void Editor::normalize_selection() {

	int tmpval;

	if (selection_begin_x>selection_end_x) {

		tmpval=selection_begin_x;
		selection_begin_x=selection_end_x;
		selection_end_x=tmpval;

	}

	if (selection_begin_y>selection_end_y) {

		tmpval=selection_begin_y;
		selection_begin_y=selection_end_y;
		selection_end_y=tmpval;
	}

	if (selection_begin_y<0) selection_begin_y=0;
	if (selection_end_y>(song->get_pattern(current_pattern)->get_length()-1)) selection_end_y=(song->get_pattern(current_pattern)->get_length()-1);

	set_flag_redraw_all();
}



void Editor::mark_block_begining() {


	selection_begin_x=cursor_x;
	selection_begin_y=cursor_y;

	if (!selection_active) {

		selection_end_x=cursor_x;
		selection_end_y=cursor_y;
		selection_active=1;
	}

	normalize_selection();

}

void Editor::selection_begin_at_cursor() {

	mouse_selection_begin_x=cursor_x;
	mouse_selection_begin_y=cursor_y;


}

void Editor::selection_end_at_cursor() {

	selection_begin_x=mouse_selection_begin_x;
	selection_begin_y=mouse_selection_begin_y;

	selection_end_x=cursor_x;
	selection_end_y=cursor_y;
	selection_active=1;
	normalize_selection();

}

void Editor::mark_block_end() {


	selection_end_x=cursor_x;
	selection_end_y=cursor_y;

	if (!selection_active) {

		selection_begin_x=cursor_x;
		selection_begin_y=cursor_y;
		selection_active=1;
	}

	normalize_selection();

}

void Editor::selection_release() {

	selection_active=false;

}

void Editor::mark_column_all() {


	if ( selection_active
	    && (selection_begin_x==selection_end_x)
	    && (selection_begin_x==cursor_x)	
            && (selection_begin_y==0)
            && (selection_end_y==(song->get_pattern(current_pattern)->get_length()-1)) ) {

		selection_begin_x=0;
		selection_end_x=Pattern::WIDTH-1;
	} else {

         	selection_begin_x=cursor_x;
         	selection_end_x=cursor_x;
		selection_begin_y=0;
		selection_end_y=(song->get_pattern(current_pattern)->get_length()-1);
		selection_active=true;
	}

	normalize_selection();

	

}

	

void Editor::shift_selection_begin() {
	
	shift_selection_in_progress=true;
	shift_selection_begin_x=cursor_x;
	shift_selection_begin_y=cursor_y;
}


void Editor::shift_selection_update() {

	if (shift_selection_in_progress) {

	        selection_active=true;
		selection_begin_x=shift_selection_begin_x;
		selection_begin_y=shift_selection_begin_y;	
		selection_end_x=cursor_x;
		selection_end_y=cursor_y;
		
		normalize_selection();
	}
}

void Editor::shift_selection_end() {

	shift_selection_in_progress=false;		

}


void Editor::selection_raise() {

	int i,j;

	if ( selection_active ) {


		push_current_pattern_to_undo_list(PATTERN_RAISE_NOTES_ON_SELECTION);

		for (i=selection_begin_y;i<=selection_end_y;i++)

			for (j=selection_begin_x;j<=selection_end_x;j++) {

				Note n=song->get_pattern(current_pattern)->get_note(j,i);
				n.raise();
				
				song->get_pattern(current_pattern)->set_note(j,i,n);

			}

	} else perform_raise_at_cursor();

}

void Editor::selection_lower() {

	int i,j;

	if ( selection_active ) {

		push_current_pattern_to_undo_list(PATTERN_LOWER_NOTES_ON_SELECTION);

		for (i=selection_begin_y;i<=selection_end_y;i++)

			for (j=selection_begin_x;j<=selection_end_x;j++) {

			Note n=song->get_pattern(current_pattern)->get_note(j,i);
			n.lower();
				
			song->get_pattern(current_pattern)->set_note(j,i,n);

		}

	} else perform_lower_at_cursor();

}


void Editor::selection_raise_octave() {
	
	int i,j;
	
	if ( selection_active ) {
		
		
		push_current_pattern_to_undo_list(PATTERN_RAISE_NOTES_ON_SELECTION);
		
		for (i=selection_begin_y;i<=selection_end_y;i++)
			
			for (j=selection_begin_x;j<=selection_end_x;j++) {
				
				Note n=song->get_pattern(current_pattern)->get_note(j,i);
				if (n.note<Note::NOTES) {
					n.note+=12;
					if (n.note>=Note::NOTES) 
						n.note=Note::NOTES-1;
				}
				
				
				song->get_pattern(current_pattern)->set_note(j,i,n);
				
			}
		
	} else perform_raise_at_cursor();
	
}

void Editor::selection_lower_octave() {
	
	int i,j;
	
	if ( selection_active ) {
		
		push_current_pattern_to_undo_list(PATTERN_LOWER_NOTES_ON_SELECTION);
		
		for (i=selection_begin_y;i<=selection_end_y;i++)
			
			for (j=selection_begin_x;j<=selection_end_x;j++) {
				
				Note n=song->get_pattern(current_pattern)->get_note(j,i);
				
				if (n.note<Note::NOTES) {
					if (n.note>11) 
						n.note-=12;
					else
						n.note=0;
				}
		
				
				song->get_pattern(current_pattern)->set_note(j,i,n);
				
			}
		
	} else perform_lower_at_cursor();
	
}

void Editor::selection_zap() {

	int i,j;

	push_current_pattern_to_undo_list(PATTERN_SELECTION_ZAP);

	selection_copy();
	
	for (i=selection_begin_y;i<=selection_end_y;i++)

		for (j=selection_begin_x;j<=selection_end_x;j++) {

			song->get_pattern(current_pattern)->set_note(j,i,Note());

		}

}
	
void Editor::selection_parameter_ramp() {

	int i,j;
	int value_begin,value_end;

	push_current_pattern_to_undo_list(PATTERN_SELECTION_RAMP_PARAMETERS);


        if (selection_active && selection_begin_y<selection_end_y) {


		for (j=selection_begin_x;j<=selection_end_x;j++) {
		
				value_begin=song->get_pattern(current_pattern)->get_note(j,selection_begin_y).parameter;
				value_end=song->get_pattern(current_pattern)->get_note(j,selection_end_y).parameter;
	
			for (i=selection_begin_y;i<=selection_end_y;i++) {
	
				Note n=song->get_pattern(current_pattern)->get_note(j,i);
	                        n.parameter=value_begin+(value_end-value_begin)*(i-selection_begin_y)/(selection_end_y-selection_begin_y);
				
				song->get_pattern(current_pattern)->set_note(j,i,n);
			}
	
		}
	
	}

}
	
void Editor::selection_volume_ramp() {

	int i,j;
	int value_begin,value_end;

	push_current_pattern_to_undo_list(PATTERN_SELECTION_RAMP_VOLUMES);

        if (selection_active && selection_begin_y<selection_end_y ) {
	
		for (j=selection_begin_x;j<=selection_end_x;j++) {
		
				value_begin=song->get_pattern(current_pattern)->get_note(j,selection_begin_y).volume;
				value_end=song->get_pattern(current_pattern)->get_note(j,selection_end_y).volume;
	
			if (value_begin<65 && value_end<65) {

				for (i=selection_begin_y;i<=selection_end_y;i++) {
					Note n=song->get_pattern(current_pattern)->get_note(j,i);
		
		                        n.volume=value_begin+(value_end-value_begin)*(i-selection_begin_y)/(selection_end_y-selection_begin_y);
					
					song->get_pattern(current_pattern)->set_note(j,i,n);
					
				}
			}
	
		}
	
	}
	
}

//ClipBoard

void Editor::selection_copy() {

	int i,j;

	clipboard_width=((selection_end_x-selection_begin_x)+1);
	clipboard.set_length((selection_end_y-selection_begin_y)+1);

	for (i=selection_begin_x;i<=selection_end_x;i++)

		for (j=selection_begin_y;j<=selection_end_y;j++) {


		clipboard.set_note(i-selection_begin_x,j-selection_begin_y ,song->get_pattern(current_pattern)->get_note(i,j));

		}

}


void Editor::selection_paste_overwrite() {

	int i,j;
	int limit_x,limit_y;


	push_current_pattern_to_undo_list(PATTERN_SELECTION_PASTE_OVERWRITE);

	limit_x=cursor_x+(clipboard_width-1);
	if (limit_x>Pattern::WIDTH-1) limit_x=Pattern::WIDTH-1;
	
	limit_y=cursor_y+(clipboard.get_length()-1);
	if (limit_y>(song->get_pattern(current_pattern)->get_length()-1)) limit_y=(song->get_pattern(current_pattern)->get_length()-1);
	
	for (i=cursor_x;i<=limit_x;i++)

		for (j=cursor_y;j<=limit_y;j++) {

		song->get_pattern(current_pattern)->set_note(i,j, clipboard.get_note(i-cursor_x,j-cursor_y) );

	}
}

void Editor::selection_paste_mix() {

	int i,j;
	int limit_x,limit_y;
	Note empty_note;


	push_current_pattern_to_undo_list(PATTERN_SELECTION_PASTE_MIX);

	limit_x=cursor_x+(clipboard_width-1);
	if (limit_x>Pattern::WIDTH-1) limit_x=Pattern::WIDTH-1;
	
	limit_y=cursor_y+(clipboard.get_length()-1);
	if (limit_y>(song->get_pattern(current_pattern)->get_length()-1)) limit_y=(song->get_pattern(current_pattern)->get_length()-1);
	
	for (i=cursor_x;i<=limit_x;i++)
	
		for (j=cursor_y;j<=limit_y;j++) {

			if (song->get_pattern(current_pattern)->get_note(i,j).is_empty()) {

				song->get_pattern(current_pattern)->set_note(i,j,clipboard.get_note(i-cursor_x,j-cursor_y) );
			}
		
		}
}


void Editor::selection_paste_insert() {

	int i,j;
	int limit_x,limit_y;

	push_current_pattern_to_undo_list(PATTERN_SELECTION_PASTE_INSERT);

	limit_x=cursor_x+(clipboard_width-1);
	if (limit_x>Pattern::WIDTH-1) limit_x=Pattern::WIDTH-1;

	limit_y=cursor_y+(clipboard.get_length()-1);
	if (limit_y>(song->get_pattern(current_pattern)->get_length()-1)) limit_y=(song->get_pattern(current_pattern)->get_length()-1);

	for (i=cursor_x;i<=limit_x;i++) {


		for(j=(song->get_pattern(current_pattern)->get_length()-1);j>(limit_y);j--) {

			song->get_pattern(current_pattern)->set_note(i,j,song->get_pattern(current_pattern)->get_note(i,j-clipboard.get_length()));
		}

		for (j=cursor_y;j<=limit_y;j++) {

			song->get_pattern(current_pattern)->set_note(i,j,clipboard.get_note(i-cursor_x,j-cursor_y));

		}

	}
}

void Editor::selection_volume_scale(int p_percent) {

	int i,j;
	int value;
	Note tmp_note;

	/* And you would think this little function should be easy... well.. no. */

	push_current_pattern_to_undo_list(PATTERN_SELECTION_VOLUME_SCALE);

        if (selection_active && selection_begin_y<selection_end_y ) {

		for (j=selection_begin_x;j<=selection_end_x;j++) {

				for (i=selection_begin_y;i<=selection_end_y;i++) {

					tmp_note=song->get_pattern(current_pattern)->get_note(j,i);

					value=tmp_note.volume;

					if (value==Note::EMPTY) {

						if ((tmp_note.note<Note::NOTES)) {

							value=64;
						} else {

	                                         	continue;
						}

					}

					value=(value*p_percent)/100;

					if (value>64) value=64;

					Note n =song->get_pattern(current_pattern)->get_note(j,i);
					n.volume=value;
						
					song->get_pattern(current_pattern)->set_note(j,i,n);
			}
		}
	}
}
void Editor::selection_set_instrument_mask() {

	int i,j;
	Note tmp_note;

	/* And you would think this little function should be easy... well.. yes. */

	push_current_pattern_to_undo_list(PATTERN_SELECTION_SET_INSTRUMENT_MASK);

        if (selection_active && selection_begin_y<selection_end_y ) {
	
		for (j=selection_begin_x;j<=selection_end_x;j++) {

			for (i=selection_begin_y;i<=selection_end_y;i++) {
		
				tmp_note=song->get_pattern(current_pattern)->get_note(j,i);

				if (tmp_note.instrument!=Note::EMPTY) {

					Note n =song->get_pattern(current_pattern)->get_note(j,i);
					n.instrument=last_instrument;		
					song->get_pattern(current_pattern)->set_note(j,i,n);
				}
			}

		}
	}
}
void Editor::selection_set_volume_mask() {

	int i,j;

	/* And you would think this little function should be easy... well.. yes. */

	push_current_pattern_to_undo_list(PATTERN_SELECTION_SET_VOLUME_MASK);

        if (selection_active && selection_begin_y<=selection_end_y ) {

		for (j=selection_begin_x;j<=selection_end_x;j++) {

			for (i=selection_begin_y;i<=selection_end_y;i++) {

				Note n=song->get_pattern(current_pattern)->get_note(j,i);
				n.volume=last_volume;
				
				song->get_pattern(current_pattern)->set_note(j,i,n);
			}
		}
	}
}

void Editor::selection_expand() {

	int i,j;
	push_current_pattern_to_undo_list(PATTERN_SELECTION_EXPAND_X2);

        if (selection_active && selection_begin_y<selection_end_y ) {

		int expand_max=(selection_end_y-selection_begin_y);
		expand_max*=2;
		expand_max+=selection_begin_y;
		if (expand_max>=song->get_pattern(current_pattern)->get_length())
			expand_max=song->get_pattern(current_pattern)->get_length()-1;

		for (j=selection_begin_x;j<=selection_end_x;j++) {

			for (i=expand_max;i>=selection_begin_y;i--) {

				int src_note=i-selection_begin_y;

				if ((src_note%2)==0) {
					src_note/=2;
					src_note+=selection_begin_y;
					song->get_pattern(current_pattern)->set_note(j,i,song->get_pattern(current_pattern)->get_note(j,src_note));
				} else
					song->get_pattern(current_pattern)->set_note(j,i,Note());
			}

		}
	}
}

void Editor::selection_shrink() {

	int i,j;
	push_current_pattern_to_undo_list(PATTERN_SELECTION_SHRINK_X2);

        if (selection_active && selection_begin_y<selection_end_y ) {

		for (j=selection_begin_x;j<=selection_end_x;j++) {

			for (i=selection_begin_y;i<=selection_end_y;i++) {

				int src_note=i-selection_begin_y;
				if (src_note>=((selection_end_y-selection_begin_y)/2)) {

					song->get_pattern(current_pattern)->set_note(j,i,Note());
				} else {
					src_note*=2;
					src_note+=selection_begin_y;
					song->get_pattern(current_pattern)->set_note(j,i,song->get_pattern(current_pattern)->get_note(j,src_note));
				}
			}
		}
	}
}

void Editor::selection_wipe_stray_volumes() {

	int i,j;
	Note tmp_note;

	/* And you would think this little function should be easy... well.. yes. */

	push_current_pattern_to_undo_list(PATTERN_SELECTION_WIPE_STRAY_VOLUMES);

        if (selection_active && selection_begin_y<selection_end_y ) {

		for (j=selection_begin_x;j<=selection_end_x;j++) {

				for (i=selection_begin_y;i<=selection_end_y;i++) {

					tmp_note=song->get_pattern(current_pattern)->get_note(j,i);
					if ((tmp_note.instrument==Note::EMPTY) && ( (tmp_note.note==Note::EMPTY) || (tmp_note.note==Note::CUT) || (tmp_note.note==Note::OFF)) ) {

						Note n=song->get_pattern(current_pattern)->get_note(j,i);
						n.volume=Note::EMPTY;
						song->get_pattern(current_pattern)->set_note(j,i,n);
					}
				}
		}
	}
}

void Editor::transpose(Uint8 *p_values) {
	
	int i,j;
	Note tmp_note;

	/* And you would think this little function should be easy... well.. yes. */

	push_current_pattern_to_undo_list(PATTERN_SELECTION_TRANSPOSE);

	if (selection_active && selection_begin_y<selection_end_y ) {

		for (j=selection_begin_x;j<=selection_end_x;j++) {

			for (i=selection_begin_y;i<=selection_end_y;i++) {

				tmp_note=song->get_pattern(current_pattern)->get_note(j,i);
				
				
				if (tmp_note.note<Note::NOTES) {

					Note n=tmp_note;
					
					int octave=n.note/12;
					int note=n.note%12;
					
					
					note=p_values[note];
					
					n.note=note+octave*12;
					if (n.note>=Note::NOTES)
						n.note-=12; //if it goes tru, reduce
					
					song->get_pattern(current_pattern)->set_note(j,i,n);
				}
			}
		}
	}
	
	
}
/* Time scale functions, by James Martin */


#if FLT_ROUNDS == 0

       #define NEARESTINT( r ) ( ((r) < 0) ? ((r) - 0.5) : ((r) + 0.5) )

#elif FLT_ROUNDS == 2

       #define NEARESTINT( r ) ( (r) - 0.5 )

#elif FLT_ROUNDS == 3

       #define NEARESTINT( r ) ( (r) + 0.5 )

#else

       #define NEARESTINT( r ) (r)

#endif

//#define NEARESTINT( r ) lrintf ( (r) + 0.5 )

float Editor::selection_time_scale_curve (
       float f_initial_velocity,
       float f_final_velocity,
       float x                         // must be in the range [0, 1]
       ) {

       // This function returns the output of a cubic function.  The cubic function is
       // selected such that:
       //
       // f(0) = 0
       // f(1) = 1
       // f'(0) = f_initial_velocity
       // f'(1) = f_final_velocity
       //
       // The output is clipped to the range [0, 1].

       float a, b, y;

       b = 3 - 2*f_initial_velocity - f_final_velocity;
       a = 1 - b - f_initial_velocity;

       y = a*x*x*x + b*x*x + f_initial_velocity*x;

       if ( y > 1 ) { y = 1; }
       if ( y < 0 ) { y = 0; }

       return y;
}

bool Editor::selection_time_scale_scale (
       int * p_row,                    // Location of row value to be modified
       int * p_tick,                   // Location of tick value to be modified
       float f_initial_stretch,
       float f_final_stretch,
       float f_total_stretch,
       int offset,
       int p_row_times[Pattern::MAX_LEN + 1],
       int pattern_length
       ) {

       // Return value is true iff out of range

       int old_time, new_time, cy;
       float t;

       old_time = p_row_times[*p_row] + *p_tick;
       old_time -= p_row_times[selection_begin_y];

       if ( f_total_stretch >= 0.01 && f_total_stretch <= 100 ) {

               int selected_ticks = p_row_times[selection_end_y+1];
               selected_ticks -= p_row_times[selection_begin_y];

               t = old_time;
               t /= selected_ticks;
               t = this->selection_time_scale_curve (
                               f_initial_stretch / f_total_stretch,
                               f_final_stretch / f_total_stretch,
                               t
                               );
               t *= selected_ticks;
               t *= f_total_stretch;

       } else {

               t = old_time * f_total_stretch;
       }

       new_time = NEARESTINT ( t );
       new_time += offset + p_row_times[selection_begin_y];

       if ( new_time < 0 ) { return true; }

       for ( cy = 0 ; cy < pattern_length ; cy++ ) {

               if ( new_time >= p_row_times[cy] && new_time < p_row_times[cy+1] ) {

                       *p_row = cy;
                       *p_tick = new_time - p_row_times[cy];

                       if ( *p_tick > 0x0F ) { *p_tick = 0x0F; }

                       return false;
               }
       }

       return true;
}


void Editor::selection_time_scale(
       float f_initial_stretch,
       float f_final_stretch,
       float f_total_stretch,
       int offset
       ) {

       push_current_pattern_to_undo_list(PATTERN_SELECTION_TIME_SCALE);



       int i, j, new_begin, new_end, row, tick;
       int p_row_times[Pattern::MAX_LEN + 1];

       Pattern backup_pattern;
       Pattern * p_pattern = song->get_pattern(current_pattern);
       int pattern_length = p_pattern->get_length();

       if ( !selection_active || selection_begin_y > selection_end_y ) { return; }

       // Back up the pattern

       p_pattern->copy_to ( &backup_pattern );

       // Determine row times

       {
               int time = 0;
               int speed = song->get_speed();

               p_row_times[0] = time;

               for ( i = 0 ; i < pattern_length ; i++ ) {

                       for ( j = selection_begin_x ; j <= selection_end_x ; j++ ) {

                               Note n = p_pattern->get_note(j,i);

                               if ( n.command == 0 && n.parameter != 0 ) {

                                       speed = n.parameter;
                               }
                       }

                       time += speed;

                       p_row_times[i+1] = time;
               }
       }

       // Find first and last row to which something might be written

       new_begin = selection_begin_y;
       tick = 0;

       if ( this->selection_time_scale_scale (
                       &new_begin, &tick,
                       f_initial_stretch, f_final_stretch, f_total_stretch,
                       offset,
                       p_row_times, pattern_length
                       )
               ) {

               new_begin = 0;
       }

       new_end = selection_end_y;
       tick = p_row_times[new_end + 1] - p_row_times[new_end] - 1;

       if ( this->selection_time_scale_scale (
                       &new_end, &tick,
                       f_initial_stretch, f_final_stretch, f_total_stretch,
                       offset,
                       p_row_times, pattern_length
                       )
               ) {

               new_end = pattern_length - 1;
       }

       // Erase old notes

       {
               //int lowest = (new_begin < selection_begin_y) ? new_begin : selection_begin_y;
               //int highest = (new_end > selection_end_y) ? new_end : selection_end_y;

               int lowest = selection_begin_y;
               int highest = selection_end_y;

               for ( j = selection_begin_x ; j <= selection_end_x ; j++ ) {

                       for ( i = lowest ; i <= highest ; i++ ) {

                               p_pattern->set_note(j,i, Note ());
                       }
               }
       }

       {
               // Add new notes

               for ( j = selection_begin_x ; j <= selection_end_x ; j++ ) {

                       int last_s_parameter = Note::EMPTY;

                       for ( i = 0 ; i <= selection_end_y ; i++ ) {

                               int delay = 0;
                               Note old_note = backup_pattern.get_note(j,i);
                               Uint8 parameter = old_note.parameter;

                               if ( old_note.command == 18 ) {

                                       if ( parameter == Note::EMPTY ) { parameter = last_s_parameter; }
                                       last_s_parameter = parameter;

                                       if ( (parameter & 0xF0) == 0xD0 ) {

                                               delay = parameter & 0x0F;
                                               if ( delay == 0 ) { delay = 1; }
                                       }
                               }

                               if ( i >= selection_begin_y ) {

                                       Note new_new_note = old_note;

                                       row = i;
                                       tick = delay;

                                       if ( !this->selection_time_scale_scale (
                                                       &row, &tick,
                                                       f_initial_stretch, f_final_stretch, f_total_stretch,
                                                       offset,
                                                       p_row_times, pattern_length
                                                       )
                                               ) {

                                               Note new_old_note = p_pattern->get_note ( j, row );
                                               bool note_copied = false, new_new_is_delayed = false;

                                               if (
                                                       ( new_new_note.note != Note::EMPTY ||
                                                         new_new_note.instrument != Note::EMPTY
                                                       ) && tick
                                                       ) {

                                                       new_new_note.parameter = 0xD0 | tick;
                                                       new_new_note.command = 18;
                                                       new_new_is_delayed = true;

                                               } else {

                                                       if ( delay ) {

                                                               new_new_note.command = Note::EMPTY;
                                                       }
                                               }

                                               // Transfer note/inst if neither are already present

                                               if (
                                                       new_old_note.note == Note::EMPTY &&
                                                       new_old_note.instrument == Note::EMPTY
                                                       ) {

                                                       new_old_note.note = new_new_note.note;
                                                       new_old_note.instrument = new_new_note.instrument;
                                                       note_copied = true;
                                               }

                                               // Transfer volume

                                               new_old_note.volume = new_new_note.volume;

                                               // Transfer command if it is higher priority than what's there

                                               if ( note_copied && new_new_is_delayed ) {

                                                       // Copy SDx

                                                       new_old_note.command = new_new_note.command;
                                                       new_old_note.parameter = new_new_note.parameter;

                                               } else if (
                                                       new_old_note.command == Note::EMPTY &&
                                                       new_new_note.command != Note::EMPTY
                                                       ) {

                                                       // Copy other command

                                                       if ( !new_new_is_delayed ) {

                                                               new_old_note.command = new_new_note.command;
                                                               new_old_note.parameter = new_new_note.parameter;
                                                       }
                                               }

                                               // Set note

                                               p_pattern->set_note ( j, row, new_old_note );
                                       }
                               }
                       }
               }
       }
}


