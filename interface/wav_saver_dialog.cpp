//
// C++ Implementation: wav_saver_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduz@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wav_saver_dialog.h"
#include "file_access_wrapper.h"
#include <stdio.h>
#include "mixer/audio_lock.h"

void WavSaverDialog::save_callback(String p_file) {

	hide();
	
	FileAccessWrapper *f = FileAccessWrapper::create();
	
	if (f->open( p_file.utf8().get_data(), FileAccessWrapper::WRITE )) {
		
		delete f;
		return;
	}
	
	
	f->store_byte_array((const Uint8*)"RIFF",4);

	int total_size=4 /* WAVE */ + 8 /* fmt+size */ + 16 /* format */ + 8 /* data+size */;

	f->store_dword( total_size ); //will store final later

	f->store_byte_array((const Uint8*)"WAVE",4);

	/* FORMAT CHUNK */
	
	f->store_byte_array((const Uint8*)"fmt ",4);

	f->store_dword( 16 ); //standard format, no extra fields

	
	f->store_word( 1 ); // compression code, standard PCM
			

	f->store_word( 2 ); //CHANNELS: 2
	
	int save_hz_val=save_hz->get_line_edit()->get_text().to_int();
	if (save_hz_val<4000)
		save_hz_val=4000;
	
	if (save_hz_val>96000)
		save_hz_val=96000;
		
	f->store_dword( save_hz_val );
	

	/* useless stuff the format asks for */

	int bits_per_sample=32;
	int blockalign = bits_per_sample / 8 * (  2  );
	int bytes_per_sec = save_hz_val*blockalign;
	
	f->store_dword( bytes_per_sec );
	f->store_word(blockalign); // block align (unused)
	f->store_word( bits_per_sample );

	/* DATA CHUNK */

	f->store_byte_array((const Uint8*)"data",4);
	
	
	f->store_dword( 0 ); //data size... wooh
	int data_from=f->get_pos(); 
	
	save_progress->show();
	
	tracker->player->play_stop();
	AudioLock::begin();
	
	tracker->mixer->set_mix_frequency( save_hz_val );
	
	tracker->player->play_start( -1,-1,-1,false );
	
	int current_order=tracker->player->get_current_order();
	while (!tracker->player->reached_end_of_song()) {
	
		int done = tracker->mixer->process(1024);
		Uint32 *dw=(Uint32*)tracker->mixer->get_mixdown_buffer_ptr();
		
		for (int i=0;i<(done*2);i++) {
			
			f->store_dword(dw[i]<<8);
		}
		
		if (current_order!=tracker->player->get_current_order()) {
			
			
			current_order=tracker->player->get_current_order();
		}
		
	}
	
	int datasize=f->get_pos()-data_from;
	
	f->seek(4);
	f->store_dword( total_size+datasize );
	
	f->seek(0x28);
	
	f->store_dword( datasize );
	
	f->close();
	
	
	
	AudioLock::end();
	tracker->player->play_stop();
	save_progress->hide();
	
	
}

void WavSaverDialog::show() {

	FileDialog::show(MODE_SAVE);
	//wb->get_window_top()->set_text( "File Dialog - Save Song as Waveform" );

}

WavSaverDialog::WavSaverDialog(Window *p_parent,Tracker *p_tracker) : FileDialog(p_parent) {

	tracker=p_tracker;
	save_hz=get_extra_vb()->add(new MarginGroup("Sampling Rate"))->add(new ComboBox);
	file_activated_signal.connect(this,&WavSaverDialog::save_callback);
	
	save_hz->add_string("4096");
	save_hz->add_string("8192");
	save_hz->add_string("11025");
	save_hz->add_string("22050");
	save_hz->add_string("44100");
	save_hz->add_string("48000");
	save_hz->add_string("96000");
	
	save_hz->get_line_edit()->set_editable( true );
	save_hz->get_line_edit()->set_text( "44100" );

	add_filter("RIFF WaveForm File","*.wav");
	
	save_progress = new Window(p_parent,Window::MODE_POPUP,Window::SIZE_CENTER);
	
	VBoxContainer *vbc = new VBoxContainer;
	save_progress->set_root_frame( vbc );
	vbc->add(new Label("Saving, Please Wait..."));
	
	
}


WavSaverDialog::~WavSaverDialog() {
	
	delete save_progress;
}


