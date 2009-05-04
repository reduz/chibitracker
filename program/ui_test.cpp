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
#include <stdio.h>
#include <stdlib.h>

#include "song/song.h"
#include "drivers/mempool_malloc.h"
#include "mempool_fixedarea.h"
#include "mixer/sample_manager_mempool.h"
#include "fileio/loader_it.h"
#include "drivers/file_access.h"
#include <stdio.h>
#include <stdlib.h>

#include "pigui/containers/box_container.h"
#include "pigui/containers/scroll_container.h"
#include "pigui/containers/fixed_container.h"
#include "pigui/containers/stack_container.h"
#include "pigui/containers/grid_container.h"
#include "pigui/base/window.h"
#include "pigui/drivers/painter_sdl.h"
#include "pigui/drivers/keycodes_sdl.h"
#include "pigui/drivers/timer_sdl.h"
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

#include "pigui/signals/signals.h"

#include "pigui/bundles/combo_box.h"
#include "pigui/bundles/spin_box.h"
#include "pigui/bundles/option_button.h"

#include "pigui/widgets/separator.h"

//notes:
//pattern - puntero(s) a last read, on write, pointer is invalidated and must search again


class Papete : public GUI::SignalTarget {
public:

	void dice_pete() { printf("Pete!\n"); }
	void dice_pete_int(int p_pete) { printf("Pete con %i\n",p_pete); }
};

class Papetin : public GUI::SignalTarget {
	public:

	void dice_pete() { printf("Petin!\n"); }
	void dice_pete_int(int p_pete) { printf("Petincin con %i\n",p_pete); }
};

int main(int argc,char *argv[]) {

/*
	if (argc==1) {
		printf("usage %s: IT file\n",argv[0]);
		return 255;
	}

	//MemPool_Malloc mempool;
	MemPool_FixedArea mempool(malloc(1024*1024),1024*1024);
	SampleManager_MemPool sample_manager;

	Song song;
	
	Loader_IT loader;
	
	FileAccess file;
	
	switch (file.open(argv[1],FileAccess::READ)) {
		
		case FileAccess::ERROR_ALREADY_IN_USE: printf("ERROR: Already in Use\n"); return 255; break;
		case FileAccess::ERROR_INVALID_PARAMETERS: printf("ERROR: Invalid Parameters\n"); return 255; break;
		case FileAccess::ERROR_OPENING_FILE: printf("ERROR: Opening file\n"); return 255; break;
		case FileAccess::ERROR_READING_FILE: printf("ERROR: Reading file\n"); return 255; break;
		case FileAccess::ERROR_WRITING_FILE: printf("ERROR: Writing file\n"); return 255; break;
		default: printf("File Open OK\n"); break;
		
	}
	
	switch( loader.load_song(&file,&song) ) {
		
		case Loader::FILE_OK: printf("FILE OK\n"); break;
		case Loader::FILE_UNRECOGNIZED: printf("FILE UNRECOGNIZED\n"); return 255; break;
		case Loader::FILE_CORRUPTED: printf("FILE CORRUPTED\n"); return 255; break;
		case Loader::FILE_OUT_OF_MEMORY: printf("FILE OUT OF MEMORY\n"); return 255; break;
		
	}

*/
	Papete papete;
	Papetin papetin;

	GUI::Method method_ptr(&papete, &Papete::dice_pete);

	method_ptr.call();
	
	GUI::Method1<int> method_ptr_int(&papete, &Papete::dice_pete_int);

	
	
	method_ptr_int.call(22);
	
	GUI::Signal< GUI::Method > sigie;
	
	sigie.connect( &papete, &Papete::dice_pete );
	sigie.connect( &papetin, &Papetin::dice_pete );
	sigie.call();
		
	GUI::Signal< GUI::Method1<int> > sigie2;
	
	sigie2.connect( &papete, &Papete::dice_pete_int );
	sigie2.connect( &papetin, &Papetin::dice_pete_int );
	sigie2.call(33);
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
 
	SDL_EnableUNICODE(1);
	
	
	
	
	Uint32 flags =SDL_HWSURFACE | SDL_RESIZABLE | ((argc>1)?SDL_FULLSCREEN:0);
	SDL_Surface *screen = SDL_SetVideoMode  (640, 480, ((argc>1)?atoi(argv[1]):32), flags);
	
	GUI::PainterSDL painter(screen);;
	GUI::TimerSDL timer;
	
	GUI::Skin skin;
	GUI::Window window( &painter,&timer, &skin );
	
	window.set_size( GUI::Size( 640,480 ) );
	
	GUI::HBoxContainer * hbc = new GUI::HBoxContainer;
	
	window.set_root_frame( hbc );
	
		
	//GUI::VBoxContainer * vbc1 = hbc->add( new GUI::VBoxContainer, 1 );

	
	GUI::VBoxContainer * vbc2 = hbc->add( new GUI::VBoxContainer, 2 );
	GUI::VBoxContainer * vbc3 = hbc->add( new GUI::VBoxContainer, 1 );
	
	GUI::List * listie = vbc2->add( new GUI::List, 2 );
	listie->set_number_elements( 1 );
	listie->set_number_min_integers( 3 );
	listie->add_string( "Hola" );
	listie->add_string( "Mundo" );
	listie->add_string( "Feliz!!!" );
	listie->add_string( "bla" );
	listie->add_string( "blabla" );
	listie->add_string( "chachara" );
	listie->add_string( "moshomosho" );
	listie->add_string( "pyonyoun" );
	listie->add_string( "geregere" );
	listie->set_multi_mode( true );
	listie->set_editable( true );
	
	GUI::ScrollBar *sb=vbc2->add( new GUI::ScrollBar, 0 );
	sb->set_auto_hide( true );
	sb->set_range( listie->get_range() );;
	
	
	//GUI::HBoxContainer *hbc2 = vbc2->add( new GUI::HBoxContainer,3);
	//hbc2->set_minimum_size( GUI::Size(200,200) );
	GUI::HBoxContainer *hbc3 = vbc2->add( new GUI::HBoxContainer,0);
	
	GUI::ButtonGroup butg;
	
	butg.add( hbc3->add( new GUI::Button("Scroll") , 0 ) );
	butg.add( hbc3->add( new GUI::Button("Fixed") , 0 ) );
	butg.add( hbc3->add( new GUI::Button("Grid") , 0 ) );

	
	GUI::StackContainer *stc = vbc2->add( new GUI::StackContainer,2 );
	
	stc->child_raised_signal.connect( &butg, &GUI::ButtonGroup::set_current );
	butg.current_button_changed_signal.connect( stc, &GUI::StackContainer::raise );
	
	/* ADD SCRILLVIEW TO STACK */
	GUI::HBoxContainer *scroll_hb = stc->add( new GUI::HBoxContainer,0 );
	
	GUI::ScrollContainer *sc = scroll_hb->add( new GUI::ScrollContainer, 1 );
	
	GUI::VBoxContainer *sc_vb = new GUI::VBoxContainer;
	sc->set_frame( sc_vb );

	for (int i=0;i<50;i++) {

		sc_vb->add( new GUI::Label("Label Number: "+GUI::String::num(i+1)), 0 );
	}
	
	scroll_hb->add( new GUI::ScrollBar(GUI::VERTICAL), 0 )->set_range(sc->get_v_range());
	
	/* ADD FIXEDPOSITIONS TO STACK */
	
	GUI::FixedContainer *fc = stc->add( new GUI::FixedContainer );
	fc->add( new GUI::Label("Fixed..") , GUI::Point( 20,10 ) );
	fc->add( new GUI::Button("Stuff..") , GUI::Point( 40,50 ) );
	fc->add( new GUI::CheckButton("Indeed..") , GUI::Point( 70,80 ) );


	GUI::GridContainer *gc = stc->add( new GUI::GridContainer(3) );

	for(int i=0;i<9;i++) {

		gc->add( new GUI::Label("Meh "+GUI::String::num(i+1)), true,true );

	}

	
	vbc2->add( new GUI::Button("Bajo El Sol"), 1);
	
	GUI::HBoxContainer *hbc4 = vbc3->add( new GUI::HBoxContainer,0);
		
	hbc4->add( new GUI::Label("Hola"), 2);
	hbc4->add( new GUI::Button("Mundo"), 1);
	hbc4->add( new GUI::Button("Nuevo"), 1);
	//hbc4->add( new GUI::Button("Nuevo"), 1);
	vbc3->add( new GUI::Button("Punyon"),0);
	vbc3->add( new GUI::LineEdit, 0);	
	GUI::HBoxContainer *slid_upd = vbc3->add( new GUI::HBoxContainer, 0);
	GUI::Slider *sldr = slid_upd->add( new GUI::Slider, 1);	
	slid_upd->add( new GUI::UpDown, 0)->set_range( sldr->get_range() );
	GUI::ComboBox *cb=vbc3->add( new GUI::ComboBox( GUI::ComboBox::MODE_ADD), 0);
	cb->add_string( "Hola" );
	cb->add_string( "Meshe" );
	cb->add_string( "Mosho" );
	cb->add_string( "Muuu!" );
	
	GUI::Slider *master = vbc3->add( new GUI::Slider(GUI::VERTICAL), 0);
	master->set_minimum_size( GUI::Size( 0, 100 ) );
	GUI::Slider *slave = vbc3->add( new GUI::Slider, 0);	
	slave->set_range( master->get_range() );
	
	vbc3->add( new GUI::SpinBox,0);
	
	
	vbc3->add( new GUI::ProgressBar,0)->set_range( master->get_range() );

	vbc3->add( new GUI::HSeparator, 0);
	vbc3->add( new GUI::CheckButton("Check Me!"),0);
	
	GUI::OptionButton *ob = vbc3->add( new GUI::OptionButton );
	ob->add_item( "One" );
	ob->add_item( "Little" );
	ob->add_item( "Kid" );
	ob->add_item( "Ate" );
	ob->add_item( "A" );
	ob->add_item( "Frog" );
	
	printf("huh?\n");
	window.set_size( GUI::Size( screen->w , screen->h ) );
	window.redraw_all();
	window.update();
	GUI::KeycodesSDL keycode_sdl_translator;


	GUI::BitmapID b=painter.load_bitmap("ctlogo.bmp");
	
	printf("%i,%i",painter.get_bitmap_size(b).width,painter.get_bitmap_size(b).height);
	
	for (int y=0;y<painter.get_bitmap_size(b).height;y++) {
		for (int x=0;x<painter.get_bitmap_size(b).width;x++) {
		
			GUI::Color c=painter.get_bitmap_pixel(b,GUI::Point(x,y));
			int dc=0;
			if (c.r==255 && c.b==255 && c.g==255) {
				
				dc=254;
			} else if (c.r==255 && c.b==255 && c.g==0) {
			
				dc=255;
			} else {
				dc=( (int)c.r + (int)c.g + (int)c.b ) /3;
			}
			
			printf(",%i",dc);
		}
	}

	printf("\n");	
	bool done=false;
	
	SDL_Event event;
    
	while((!done) && (SDL_WaitEvent(&event))) {
		
		switch(event.type) {
			//case SDL_USEREVENT:
		//		HandleUserEvents(&event);
	//			break;
			case SDL_VIDEORESIZE:
				printf("??\n");	
	
				screen = SDL_SetVideoMode  (event.resize.w,event.resize.h, 32, flags);
				window.set_size( GUI::Size(event.resize.w,event.resize.h ) );
				window.redraw_all();
				window.update();
				
				break;
			case SDL_KEYUP:
			case SDL_KEYDOWN: {
                // Handle any key presses here.
				if (event.key.keysym.sym==SDLK_ESCAPE)
					done=true;
				
				unsigned int mod=0;
				
				if (  event.key.keysym.mod & (KMOD_LSHIFT|KMOD_RSHIFT))
					mod|=GUI::KEY_MASK_SHIFT;
				
				if (  event.key.keysym.mod & (KMOD_LALT|KMOD_RALT))
					mod|=GUI::KEY_MASK_ALT;
				
				if (  event.key.keysym.mod & (KMOD_LCTRL|KMOD_RCTRL))
					mod|=GUI::KEY_MASK_CTRL;
								  
				if (  event.key.keysym.mod & (KMOD_LMETA|KMOD_RMETA))
					mod|=GUI::KEY_MASK_META;
				
				window.key( event.key.keysym.unicode, keycode_sdl_translator.get_code(event.key.keysym.sym), event.key.state==SDL_PRESSED,false, mod );
				
				
				if (event.key.keysym.sym==SDLK_F1) {

					painter.draw_fill_rect( GUI::Point() , GUI::Size( screen->w , screen->h ), GUI::Color(100,20,33) );
					SDL_UpdateRect(screen, 0,0,0,0);
				}

				if (event.key.keysym.sym==SDLK_F2) {

					painter.update_screen();
				}

				
				printf("Pressed Key %s, unicode %i\n",GUI::Keyboard::get_code_name( keycode_sdl_translator.get_code(event.key.keysym.sym)).ascii().get_data(),event.key.keysym.unicode);
			} break;

			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN: {
				
				SDLMod	mod_state=SDL_GetModState();

				unsigned int mod=0;
				
				if (  mod_state & (KMOD_LSHIFT|KMOD_RSHIFT))
					mod|=GUI::KEY_MASK_SHIFT;
				
				if (  mod_state & (KMOD_LALT|KMOD_RALT))
					mod|=GUI::KEY_MASK_ALT;
				
				if (  mod_state & (KMOD_LCTRL|KMOD_RCTRL))
					mod|=GUI::KEY_MASK_CTRL;
								  
				if (  mod_state & (KMOD_LMETA|KMOD_RMETA))
					mod|=GUI::KEY_MASK_META;
				
				
				window.mouse_button( GUI::Point( event.button.x, event.button.y ), event.button.button, event.button.type==SDL_MOUSEBUTTONDOWN, mod );
			} break;
				
			case SDL_MOUSEMOTION:

				window.mouse_motion( GUI::Point( event.motion.x, event.motion.y ), GUI::Point( event.motion.xrel, event.motion.yrel ), event.motion.state );
				break;
			case SDL_QUIT:
				done = true;
				break;

			default: {}
				break;

		}   // End switch
            
		static int i=0;
//		printf("check updates! - %i\n",i++);
		window.check_for_updates();
		//SDL_UpdateRect   (screen, 0, 0, 0, 0);
		
	}   // End while
        
	SDL_Quit();
	return 0;
}
