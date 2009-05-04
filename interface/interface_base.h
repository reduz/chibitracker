//
// C++ Interface: interface_base
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef INTERFACE_BASE
#define INTERFACE_BASE

#include "signals/signals.h"
using namespace GUI;

class MixerDialog;
class InterfaceSettingsDialog;
class Tracker;
class GUI::Window;
class Mixer;
class ConfigApi;

class InterfaceBase : public SignalTarget {
public:

	virtual void open_song(String p_fname)=0;		

	virtual MixerDialog *get_mixer_dialog()=0;
	virtual InterfaceSettingsDialog *get_ui_settings_dialog()=0;
	virtual bool must_quit()=0;
	virtual void quit_request()=0;
	
	virtual Tracker * get_tracker()=0;
	
	virtual void set_default_keybinds()=0;
	virtual void update_components()=0;	
	
	virtual void show_splash()=0;

	virtual ~InterfaceBase() {}

};




#endif
