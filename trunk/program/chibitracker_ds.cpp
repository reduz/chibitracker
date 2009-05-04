//
// C++ Implementation: chibitracker_palm
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mixer/mixer_soft.h"
#include "song/song.h"
#include "drivers/mempool_malloc.h"
#include "mempool_fixedarea.h"
#include "mixer/sample_manager_mempool.h"
#include "fileio/loader_it.h"
#include "drivers/file_access.h"

#include "mixer/sound_driver_manager.h"
#include "pigui/containers/box_container.h"
#include "pigui/containers/scroll_container.h"
#include "pigui/containers/fixed_container.h"
#include "pigui/containers/stack_container.h"
#include "pigui/containers/grid_container.h"
#include "pigui/base/window.h"
#include "pigui/drivers/painter_fb.h"
#include "pigui/widgets/label.h"
#include "pigui/widgets/list.h"
#include "pigui/widgets/button.h"
#include "pigui/widgets/line_edit.h"
#include "pigui/widgets/slider.h"
#include "pigui/widgets/progress_bar.h"
#include "pigui/widgets/scroll_bar.h"
#include "pigui/widgets/updown.h"
#include "pigui/widgets/check_button.h"
#include "pigui/widgets/button_group.h"
#include "interface/ctskin.h"
#include "interface/pattern_editor.h"
#include "interface/pattern_screen.h"
#include "interface/config_file.h"
#include "tracker/tracker.h"
#include "tracker/key_bindings.h"
#include "interface/interface_mini.h"
#include "pigui/signals/signals.h"
#include "nds_backends.h"

#include "pigui/bundles/combo_box.h"
#include "pigui/bundles/spin_box.h"
#include "pigui/bundles/option_button.h"

#include "pigui/widgets/separator.h"

#include "tracker/key_bindings.h"
#include "base/keyboard.h"
#include "error_macros.h"

#include "chibitracker_ds_virtual_kb.h"
#include "chibitracker_ds_virtual_kb_zones.h"

#include <nds.h>

#define CONFIG_DIR_BASE String(".")
#define CONFIG_DIR String(".chibi_ds")
#define CONFIG_FILE String("chibi.cfg")

#define CONFIG_FILE_PATH ( CONFIG_DIR_BASE + "\\" + CONFIG_DIR + "\\" + CONFIG_FILE )
#define CONFIG_DIR_SKINS ( CONFIG_DIR_BASE + "\\" + CONFIG_DIR + "\\skins" )
#define CONFIG_SYSTEM_SKINS ( String(".\\skins") )



static void ensure_config_dir_exists() {

	FileSystem *fs = FileSystem::instance_func();

	ERR_FAIL_COND(fs->change_dir( CONFIG_DIR_BASE ));
	
	if (fs->change_dir( CONFIG_DIR ) ) {
			
		ERR_FAIL_COND( fs->make_dir( CONFIG_DIR ) );
		fs->change_dir( CONFIG_DIR );
	}
	
	if (fs->change_dir( "skins" ) ) {
			
		ERR_FAIL_COND( fs->make_dir( "skins" ) );
	}
	
	
	delete fs;
}

/** DS EVENTS **/

static bool ds_virtual_input_enabled=true;

struct DS_Event {

	enum Type {
	
		EV_PEN_DOWN,
		EV_PEN_UP,
		EV_PEN_MOVE,
		EV_KEY_DOWN,
		EV_KEY_UP,
	};
	
	Type type;
	Uint32 key;
	Uint32 code;
	Uint16 x;
	Uint16 y;
	Uint16 x_rel;
	Uint16 y_rel;
	Uint32 mod;
	bool rep;

};

#define DS_EVENT_BUFFER_SIZE 256

static DS_Event ds_event_buffer[DS_EVENT_BUFFER_SIZE];

static int ds_event_read_ptr=0;
static int ds_event_write_ptr=0;

void vkb_draw_zone(int p_zone,Color p_color) {

	if (p_zone<0 || p_zone>ZONE_MAX)
		return;
	
	Rect rect = GUI::Rect(Point(vkb_zone_rects[p_zone][0],
				vkb_zone_rects[p_zone][1]),
				Point(vkb_zone_rects[p_zone][2],
				vkb_zone_rects[p_zone][3]));

	Uint16 c;
	c=p_color.b>>3;
	c<<=5;
	c|=p_color.g>>3;
	c<<=5;
	c|=p_color.r>>3;
	c|=0x8000;

	u16* video_buffer_main = (u16*)(0x6200000); //BG_BMP_RAM(0);
	//video_buffer_main = (u16*)SCREEN_BASE_BLOCK_SUB(screen_base)
	
	for (int i=0;i<rect.size.width;i++) {
	
		int pos = (rect.pos.y*256)+rect.pos.x+i;
		video_buffer_main[pos]=c;
		video_buffer_main[pos+rect.size.height*256]=c;	
	}

	for (int i=0;i<rect.size.height;i++) {
	
		int pos = ((rect.pos.y+i)*256)+rect.pos.x;
		video_buffer_main[pos]=c;
		video_buffer_main[pos+rect.size.width]=c;	
	}

}

void vkb_clear_zone(int p_zone) {

	if (p_zone<0 || p_zone>ZONE_MAX)
		return;
	
	Rect rect = GUI::Rect(Point(vkb_zone_rects[p_zone][0],
				vkb_zone_rects[p_zone][1]),
				Point(vkb_zone_rects[p_zone][2],
				vkb_zone_rects[p_zone][3]));


	u16* video_buffer_main = (u16*)(0x6200000); //BG_BMP_RAM(0);
	//video_buffer_main = (u16*)SCREEN_BASE_BLOCK_SUB(screen_base)
	
	for (int i=0;i<rect.size.width;i++) {
	
		int pos = (rect.pos.y*256)+rect.pos.x+i;
		video_buffer_main[pos]=virtual_kb_builtin_data[pos]|0x8000;;
		pos+=rect.size.height*256;
		video_buffer_main[pos]=virtual_kb_builtin_data[pos]|0x8000;;	
	}

	for (int i=0;i<rect.size.height;i++) {
	
		int pos = ((rect.pos.y+i)*256)+rect.pos.x;
		video_buffer_main[pos]=virtual_kb_builtin_data[pos]|0x8000;
		pos+=rect.size.width;
		video_buffer_main[pos]=virtual_kb_builtin_data[pos]|0x8000;;	
	}

}

static bool ds_pop_event(DS_Event * p_event) {

	if (ds_event_read_ptr==ds_event_write_ptr)
		return false;
		
	if (!p_event)
		return true;
		
	*p_event = ds_event_buffer[ ds_event_read_ptr ];
	
	ds_event_read_ptr = ( ds_event_read_ptr + 1 ) % DS_EVENT_BUFFER_SIZE;
	return true;
}

static Uint32 held_prev=0;
touchPosition t_old;
static int last_key_down=-1;
static int last_key_down_code=-1;
static int last_key_zone=-1;

unsigned int key_mask=0;

void ds_test_key(Uint32 flag, Uint32 held, Uint32 code, Uint32 key) {

	if ( ((ds_event_write_ptr+1)%DS_EVENT_BUFFER_SIZE) == ds_event_read_ptr )
		return; // full buffer
	if ((held&flag) == (held_prev&flag))
		return;
	
	ds_event_buffer[ds_event_write_ptr].type=(held&flag)?DS_Event::EV_KEY_DOWN:DS_Event::EV_KEY_UP;
	ds_event_buffer[ds_event_write_ptr].code=code;
	ds_event_buffer[ds_event_write_ptr].key=key;
	ds_event_buffer[ds_event_write_ptr].rep=false;
	ds_event_buffer[ds_event_write_ptr].mod=key_mask;
	
	
	ds_event_write_ptr = ( ds_event_write_ptr + 1 ) % DS_EVENT_BUFFER_SIZE;
	
}

void ds_update_event_queue() {

	scanKeys();
	
	Uint32 held =  keysHeld();
	
	if ((held&KEY_L) && !(held_prev&KEY_L)) {
		
		ds_virtual_input_enabled=!ds_virtual_input_enabled;
		if (ds_virtual_input_enabled)
			lcdMainOnTop();	
		else
			lcdMainOnBottom();	
		
	}


	ds_test_key( BIT(6), held, 0, GUI::KEY_UP);
	ds_test_key( BIT(7), held, 0, GUI::KEY_DOWN);
	ds_test_key( BIT(5), held, 0, GUI::KEY_LEFT);
	ds_test_key( BIT(4), held, 0, GUI::KEY_RIGHT);
	ds_test_key( KEY_R, held, 0, GUI::KEY_F5);
	
	
	touchPosition t = touchReadXY();
	
	/* check mouse button */
	if ( ((ds_event_write_ptr+1)%DS_EVENT_BUFFER_SIZE) == ds_event_read_ptr ) {
		held_prev=held;
		return; // full buffer
	}
	
	if (!ds_virtual_input_enabled) {
			
		if ((held&KEY_TOUCH) != (held_prev&KEY_TOUCH)) {
		
		
			ds_event_buffer[ds_event_write_ptr].type=(held&KEY_TOUCH)?DS_Event::EV_PEN_DOWN:DS_Event::EV_PEN_UP;
			ds_event_buffer[ds_event_write_ptr].x=t.px;
			ds_event_buffer[ds_event_write_ptr].y=t.py;
			ds_event_write_ptr = ( ds_event_write_ptr + 1 ) % DS_EVENT_BUFFER_SIZE;
			
		} else if (held&KEY_TOUCH && (t.px != t_old.px || t.py != t_old.py )) {
		
			ds_event_buffer[ds_event_write_ptr].type=DS_Event::EV_PEN_MOVE;
			ds_event_buffer[ds_event_write_ptr].x=t.px;
			ds_event_buffer[ds_event_write_ptr].y=t.py;
			ds_event_buffer[ds_event_write_ptr].x_rel=t.px-t_old.px;
			ds_event_buffer[ds_event_write_ptr].y_rel=t.py-t_old.py;
			ds_event_write_ptr = ( ds_event_write_ptr + 1 ) % DS_EVENT_BUFFER_SIZE;
			
		}
	} else {
	
		if ((held&KEY_TOUCH) != (held_prev&KEY_TOUCH)) {
		
			if (held&KEY_TOUCH) {
			/* pen press */
				for (int i=0;i<ZONE_MAX;i++) {
					
					Rect r = GUI::Rect(Point(vkb_zone_rects[i][0],
							vkb_zone_rects[i][1]),
							Point(vkb_zone_rects[i][2],
							vkb_zone_rects[i][3]));
							
					if (!r.contains( GUI::Point( t.px, t.py ) ) )
								continue;
								
					bool clear=false;
					if (i==ZONE_SHIFT) {
						if (key_mask&KEY_MASK_SHIFT) {
							key_mask&=~GUI::KEY_MASK_SHIFT;
							clear=true;
						} else
							key_mask|=GUI::KEY_MASK_SHIFT;
					}
					if (i==ZONE_ALT) {
						if (key_mask&KEY_MASK_ALT) {
							key_mask&=~GUI::KEY_MASK_ALT;
							clear=true;
						} else
							key_mask|=GUI::KEY_MASK_ALT;
					}
					if (i==ZONE_CTRL) {
						if (key_mask&KEY_MASK_CTRL) {
							key_mask&=~GUI::KEY_MASK_CTRL;
							clear=true;
						} else
							key_mask|=GUI::KEY_MASK_CTRL;
					}
								
					ds_event_buffer[ds_event_write_ptr].type=DS_Event::EV_KEY_DOWN;
					ds_event_buffer[ds_event_write_ptr].code=vkb_zone_keys[i][1];
					ds_event_buffer[ds_event_write_ptr].key=vkb_zone_keys[i][0];
					ds_event_buffer[ds_event_write_ptr].rep=false;
					ds_event_buffer[ds_event_write_ptr].mod=key_mask;
					
					ds_event_write_ptr = ( ds_event_write_ptr + 1 ) % DS_EVENT_BUFFER_SIZE;
					last_key_down=vkb_zone_keys[i][0];
					last_key_down_code=vkb_zone_keys[i][1];
					last_key_zone=i;
					if (clear)
						vkb_clear_zone(i);
					else
						vkb_draw_zone(i,Color(110,180,255));
					
					
					break;
				}
						
						
			} else if (last_key_down!=-1) {
			
				ds_event_buffer[ds_event_write_ptr].type=DS_Event::EV_KEY_UP;
				ds_event_buffer[ds_event_write_ptr].code=last_key_down_code;
				ds_event_buffer[ds_event_write_ptr].key=last_key_down;
				ds_event_buffer[ds_event_write_ptr].rep=false;
				ds_event_buffer[ds_event_write_ptr].mod=key_mask;
				ds_event_write_ptr = ( ds_event_write_ptr + 1 ) % DS_EVENT_BUFFER_SIZE;
				last_key_down=-1;
				last_key_down_code=-1;
				
				if (last_key_zone==ZONE_SHIFT || last_key_zone==ZONE_ALT || last_key_zone==ZONE_CTRL) {
				
					
				
				} else {
									
					vkb_clear_zone(last_key_zone);
					if (key_mask) {
						key_mask=0;
						vkb_clear_zone(ZONE_ALT);
						vkb_clear_zone(ZONE_SHIFT);
						vkb_clear_zone(ZONE_CTRL);
					}
					
					
				}
				
				last_key_zone=-1;
				
				
			}
		}
	}
	
	// BIT(12) touch
	// BIT(13) lid

	held_prev=held;
	t_old=t;
}

namespace GUI {


void __print_error(const char *p_file,int p_line,const char* p_error) {
	
	printf("ERROR: at %s:%i -- %s\n",p_file,p_line,p_error);

	
}

}
     

static void draw_virtual_kb() {

	u16* video_buffer_main = (u16*)(0x6200000); //BG_BMP_RAM(0);
	//video_buffer_main = (u16*)SCREEN_BASE_BLOCK_SUB(screen_base)
	for (int i=0;i<192*256;i++) {
	
		video_buffer_main[i]=virtual_kb_builtin_data[i]|0x8000;
	}

}


/**
	@author Juan Linietsky <reduzio@gmail.com>
 */


#define DEFAULT_H 192
#define DEFAULT_W 256

void vblank_int() {


}


extern "C" {

/** Chibitracker needs a larger stack than the very few 16k the DS provides, so a 128k one is provided */

#define STACK_KB 128

Uint8 *gMainRamStack;

extern void CallFunctionWithMainRamStack(void (*function)(void));

void InitMainRamStack()
{
    // Set the pointer to the top of it, since the stack grows downward.
    // Make sure you subtract 0x20000 if you ever free the memory.
    gMainRamStack = (Uint8*)malloc(STACK_KB * 1024) + (STACK_KB * 1024);
}

#undef STACK_KB

};

static void chibi_main() {

	//powerON(POWER_ALL_2D);
	irqInit();
    	irqSet(IRQ_VBLANK, vblank_int);
        irqEnable(IRQ_VBLANK);
     
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
        videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);        //sub bg 0 will
        vramSetBankC(VRAM_C_SUB_BG);
	   vramSetMainBanks(   VRAM_A_MAIN_BG_0x06000000, VRAM_B_LCD,
                                                VRAM_C_SUB_BG_0x06200000, VRAM_D_LCD);


    //    SUB_BG0_CR = BG_MAP_BASE(31);

   //     BG_PALETTE_SUB[255] = RGB15(31,31,31);  //by default font will be rende

        //consoleInit() is a lot more flexible but this gets you up and running
        //consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16); 

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\t16 bit bitmap demo");


	BG3_CR = BG_BMP16_256x256;

	BG3_XDX = 1 << 8;
	BG3_XDY = 0;
	BG3_YDX = 0;
	BG3_YDY = 1 << 8;

	BG3_CX = 0;
	BG3_CY = 0;
		
	SUB_BG3_CR = BG_BMP16_256x256;

	SUB_BG3_XDX = 1 << 8;
	SUB_BG3_XDY = 0;
	SUB_BG3_YDX = 0;
	SUB_BG3_YDY = 1 << 8;

	SUB_BG3_CX = 0;
	SUB_BG3_CY = 0;
		
	fatInitDefault(); /* initialize libfat */
	
	u16* video_buffer_main = (u16*)(0x6000000); //BG_BMP_RAM(0);
	//u16* video_buffer_sub =  (u16*)BG_BMP_RAM_SUB(0);	
	
	PainterFB *painter = new PainterFB(new unsigned short[DEFAULT_W*DEFAULT_H],video_buffer_main,Size(DEFAULT_W,DEFAULT_H),PainterFB::FB_15,true);


	MemPool_Malloc *mempool = new MemPool_Malloc;
	
	FileAccess::set_as_default();

	FileSystemNDS::set_default_filesystem();
	
	/* CONFIG DIR */
	ensure_config_dir_exists();
	
	
	FileAccessWrapper * config_file_access = FileAccessWrapper::create();
	
	ConfigFile *config_file = new ConfigFile(config_file_access);
	config_file->set_path( CONFIG_FILE_PATH );

	SampleManager_MemPool * sample_manager = new SampleManager_MemPool;
	
	Mixer_Soft * mixer_software = new Mixer_Soft;
	SoundDriverManager::get_audio_params().set_mixer(mixer_software);
	
	SoundDriverNDS *sound_driver_nds = new SoundDriverNDS;
	
	SoundDriverManager::register_driver( sound_driver_nds );
	
	painter->draw_fill_rect(Point(20,20),Size(100,100),Color(255,0,255));
	painter->update_screen();
	
	TimerNDS *timer = new TimerNDS;
	
	ScrollBar::set_can_focus_by_default( false );
	
	CTSkin *skin = new CTSkin;
	
	Window *window = new Window( painter, timer, skin );
	
	
	InterfaceMini *interface = new InterfaceMini(window,mixer_software,config_file);
	
	
	interface->get_mixer_dialog()->set_mixer( mixer_software );
	interface->get_ui_settings_dialog()->set_theme_path( CONFIG_DIR_SKINS );
	interface->get_ui_settings_dialog()->set_system_theme_path( CONFIG_SYSTEM_SKINS );
	
	window->set_size( GUI::Size( DEFAULT_W,DEFAULT_H ) );
	
	window->redraw_all();
	window->update();
	
	SoundDriverManager::init_driver(0);
	
	bool done=false;
	
	Uint32 last_click_tick=0;
	bool can_dblclick=false;
	
	draw_virtual_kb();

	while(!done) {
		DS_Event ev;
		
		while (!ds_pop_event(&ev)) {
			ds_update_event_queue();
			swiWaitForVBlank();
		}
		
		switch (ev.type) {
			
			case DS_Event::EV_KEY_DOWN:
			case DS_Event::EV_KEY_UP: {
				
				window->key( ev.code, ev.key, ev.type==DS_Event::EV_KEY_DOWN, ev.rep, ev.mod );
			
			} break;
			case DS_Event::EV_PEN_UP:
			case DS_Event::EV_PEN_DOWN: {
			
				Uint32 last_click_delta=0;
				
				
				if (ev.type==DS_Event::EV_PEN_DOWN) {
					
					/*
					last_click_delta=
					last_click_tick=SDL_GetTicks();
					*/
				}
				
				window->mouse_button( GUI::Point( ev.x, ev.y ), 1, ev.type==DS_Event::EV_PEN_DOWN, ev.mod );
				
				/*
				if (can_dblclick && last_click_delta<250 && event.button.type==SDL_MOUSEBUTTONDOWN && event.button.button==BUTTON_LEFT) {
					window->mouse_doubleclick( GUI::Point( event.button.x, event.button.y ), mod );
					can_dblclick=false;

				} else
					can_dblclick=true; */
				
			
			} break;
			case DS_Event::EV_PEN_MOVE: {
			
				window->mouse_motion( Point( ev.x,ev.y), Point(ev.x_rel, ev.y_rel), ev.mod );
			
			} break;
		
		
		}

		
//	swiWaitForVBlank();		
		
		window->check_for_updates();
		done=interface->must_quit();
		
	}


}


int main(int argc,char *argv[]) {

	InitMainRamStack();
	CallFunctionWithMainRamStack( chibi_main );
	
}
