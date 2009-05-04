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
                          loader_xm.h  -  description
                             -------------------
    begin                : Tue Apr 9 2002
    copyright            : (C) 2002 by red
    email                : red@server
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LOADER_XM_H
#define LOADER_XM_H

#include "fileio/loader.h"


/**
  *@author red
  */




class Loader_XM : public Loader  {


	struct XM_Header {

   		Uint8 idtext[18];
     		Uint8 songname[21];
       		Uint8 hex1a; // ?
         	Uint8 trackername[21];
          	Uint16 version;
           	Uint32 headersize; //from here
            	
            	Uint16 songlength; //pattern ordertable
             	Uint16 restart_pos;
              	Uint16 channels_used;
                Uint16 patterns_used;
                Uint16 instruments_used;
                Uint16 use_linear_freq;
                Uint16 tempo;
                Uint16 speed;
                Uint8 orderlist[256];

	} header;
  	
	FileAccessWrapper *file;
        
	Error load_instrument_internal(Instrument *pint,bool p_xi,int p_cpos, int p_hsize, int p_sampnumb=-1);
	Song *song;

public:

	bool can_load_song() { return true; }
	bool can_load_sample() { return false; }
	bool can_load_instrument() { return true; }
	
	Error load_song(const char *p_file,Song *p_song);
	Error load_sample(const char *p_file,Sample *p_sample);
	Error load_instrument(const char *p_file,Song *p_song,int p_instr_idx);

	
	Loader_XM(FileAccessWrapper *p_file);
	~Loader_XM();
};



#endif
