//
// C++ InterfaceMini: interface
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INTERFACE_MINI_H
#define INTERFACE_MINI_H

#include "mixer/mixer.h"
#include "base/window.h"
#include "widgets/tab_bar.h"
#include "widgets/icon.h"
#include "containers/stack_container.h"
#include "bundles/menu_box.h"
#include "bundles/message_box.h"
#include "bundles/file_dialog.h"

#include "interface/config_api.h"
#include "interface/pattern_screen.h"
#include "interface/sample_screen.h"
#include "interface/instrument_screen.h"
#include "interface/variables_screen.h"
#include "interface/playback_button.h"
#include "interface/main_vu.h"

#include "interface/sound_driver_dialog.h"
#include "interface/mixer_dialog.h"
#include "interface/keyboard_dialog.h"
#include "interface/interface_settings_dialog.h"
#include "interface/interface_help.h"
#include "interface/wav_saver_dialog.h"

#include "interface/paths_dialog.h"

#include "tracker/tracker.h"
#include "tracker/key_bindings.h" 

#include "interface/interface_base.h"

using namespace GUI;

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class InterfaceMini : public InterfaceBase {

	
	enum {
		
		STATUS_OVERRIDE_TIME_DEFAULT=2500
	};

	enum {
		SCREEN_PATTERN,
		SCREEN_SAMPLES,
		SCREEN_INSTRUMENTS,
		SCREEN_VARIABLES,
		SCREEN_MAX
	};

	enum {
		
		SONG_MENU_NEW,
		SONG_MENU_OPEN,
		SONG_MENU_SAVE,
		SONG_MENU_SAVE_AS,
		SONG_MENU_EXPORT_WAV,
		SONG_MENU_CLEANUP,
		SONG_MENU_QUIT,
	};
	
	enum {
		
		CONFIG_MENU_AUDIO,
		CONFIG_MENU_MIXER,
		CONFIG_MENU_KEYS,
		CONFIG_MENU_THEME,
		CONFIG_MENU_PATHS,
	};
	
	enum {
		
		HELP_MENU_PATTERN,
		HELP_MENU_SKINS,
		HELP_MENU_ABOUT,
	};
	
	Tracker tracker;
	DefaultPaths paths;

	MenuBox *file;
	MenuBox *settings;
	MenuBox *help;
	
	int status_override_time;

	Window *window;
	VBoxContainer *top;
	StackContainer *main_stack;

	void default_octave_changed(double p_to);
	
	TabBar *tabs;
	
	MessageBox *error_msg;
	MessageBox *about_msg;
	/***/
	
	HBoxContainer *top_bar;
	PatternScreen *pattern_screen;
	SampleScreen *sample_screen;
	InstrumentScreen *instrument_screen;
	VariablesScreen *variables_screen;
	KeyBind kb; //keybinds

	void check_instrument_visibility();
	
	void button_rw_pressed();
	void button_play_pressed();
	void button_stop_pressed();
	void button_fw_pressed();
	void button_play_pattern_pressed();
	void button_play_cursor_pressed();
	void button_play_order_pressed();
	
	void main_key(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask);
	
	void pre_screen_changed_from(int p_which);
	void screen_changed_to(int p_which);
	
	Frame *old_focus[SCREEN_MAX];

	
	void song_menu_selected(int p_option);
	void config_menu_selected(int p_option);
	void help_menu_selected(int p_option);
	FileDialog *file_dialog;
	

	
	void new_song_accept();
	
	struct NewSong {
		
		Window *window;
		CheckButton *patterns;
		CheckButton *instruments;
		CheckButton *samples;
		CheckButton *variables;
		
	} new_song;
	
	void cleanup_accept();
	
	struct CleanUp {
		
		Window *window;
		CheckButton *patterns;
		CheckButton *instruments;
		CheckButton *samples;
		CheckButton *orders;
		CheckButton *def_pan;
		CheckButton *def_vol;
		
	} cleanup;
	
	Window *confirm_quit_window;
	
	
	void quit_accept();
	bool quit;
	
	/* File */
	
	String last_filename;
	void song_file_callback(String p_fname);
	
	/* Settings Dialogs */
	
	SoundDriverDialog *sound_driver_dialog;
	MixerDialog *mixer_dialog;
	KeyboardDialog *kb_dialog;
	InterfaceSettingsDialog *ui_dialog;
	InterfaceHelp *uihelp_dialog;
	InterfaceHelp *skins_dialog;
	WavSaverDialog *wav_saver;
	PathsDialog *path_dialog;
	
	TimerID timer;
	void ui_repaint_interval_changed(int p_to);
	
	void interface_update_timer_callback();
	
	void instrument_mask_changed(int p_to);
	
	void update_title_bar();
	
	ConfigApi *config;
	
	void skin_changed();
	void setup_logos();
	
	void status_override(String p_message);
	
	void paths_changed();
	
	
public:
	
	
	MixerDialog *get_mixer_dialog() { return mixer_dialog; }
	InterfaceSettingsDialog *get_ui_settings_dialog() { return ui_dialog; }
	bool must_quit();
	void quit_request();
	
	Tracker * get_tracker();
	
	void set_default_keybinds();
	void update_components();	
	
	void show_splash();

	InterfaceMini(Window *p_window,Mixer *p_mixer,ConfigApi *p_config);
	~InterfaceMini();

};

#endif
