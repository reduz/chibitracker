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
                          editor_orderlist.cpp  -  description
                             -------------------
    begin                : Fri Apr 20 2001
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



void Editor::orderlist_move_up() {

        if (orderlist_cursor_y>0) orderlist_cursor_y--;

}

void Editor::orderlist_move_down() {

	if (orderlist_cursor_y<Song::MAX_ORDERS-1) orderlist_cursor_y++;
	
}

void Editor::orderlist_move_left() {

        if (orderlist_cursor_x>0) orderlist_cursor_x--;
	
}

void Editor::orderlist_move_right() {

        if (orderlist_cursor_x<2) orderlist_cursor_x++;

}

void Editor::orderlist_page_up() {

       orderlist_cursor_y-=8;
       if (orderlist_cursor_y<0) orderlist_cursor_y=0;
}

void Editor::orderlist_page_down() {

       orderlist_cursor_y+=8;
       if (orderlist_cursor_y>Song::MAX_ORDERS-1) orderlist_cursor_y=Song::MAX_ORDERS-1;
}


void Editor::orderlist_add_separator() {

	song->set_order(orderlist_cursor_y,ORDER_BREAK);
	orderlist_move_down();
}

void Editor::orderlist_clear_order() {

	song->set_order(orderlist_cursor_y,Note::EMPTY);
	orderlist_move_down();
}

void Editor::orderlist_insert_value(int number) {

	int tmpvalue;

	tmpvalue=song->get_order(orderlist_cursor_y);

	if (tmpvalue>=199) tmpvalue=0;

	switch (orderlist_cursor_x) {

		case 0: {

			tmpvalue=tmpvalue-((tmpvalue/100)*100)+number*100;
		} break;
		case 1: {

			tmpvalue=tmpvalue-(((tmpvalue/10) % 10)*10)+number*10;	
		} break;
		case 2: {
			tmpvalue=tmpvalue-(tmpvalue % 10)+number;		
		} break;

	}

	if (tmpvalue>=199) tmpvalue=199;

	song->set_order(orderlist_cursor_y,tmpvalue);

	if (orderlist_cursor_x<2) orderlist_move_right();
	else { orderlist_cursor_x=0; orderlist_move_down();}
}

void Editor::orderlist_insert() {
	
	int i;

	for (i=Song::MAX_ORDERS-1;i>orderlist_cursor_y;i--) {

		song->set_order(i,song->get_order(i-1));
	}
	song->set_order(orderlist_cursor_y,Note::EMPTY);
}

void Editor::orderlist_set_cursor_row(int p_row) {
	
	if (p_row<0)
		p_row=0;
	if (p_row>=Song::MAX_ORDERS)
		p_row=Song::MAX_ORDERS-1;
	
	orderlist_cursor_y=p_row;
}
void Editor::orderlist_delete() {

	int i;

	for (i=orderlist_cursor_y;i<Song::MAX_ORDERS-1;i++) {

		song->set_order(i,song->get_order(i+1));
	}
	song->set_order(Song::MAX_ORDERS-1,Note::EMPTY);
}

void Editor::orderlist_get_cursor_string(int p_order,char p_str[4]) {

	p_str[0]='0';
	p_str[1]='0';
	p_str[2]='0';
	p_str[3]=0;
	
	char *tmporder=p_str;
        int aux;
       	aux=song->get_order(p_order);
     	switch (aux) {

     		case 255: {

     			tmporder[0]='.';
     			tmporder[1]='.';
     			tmporder[2]='.';
     		}break;
     		case 254: {

     			tmporder[0]='+';
     			tmporder[1]='+';
     			tmporder[2]='+';
     		}break;
     		default: {

     			tmporder[0]='0'+aux/100;
     			tmporder[1]='0'+(aux/10)%10;
     			tmporder[2]='0'+aux%10;
     		}break;

     	}


}

