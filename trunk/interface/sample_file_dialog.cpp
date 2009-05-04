//
// C++ Implementation: sample_file_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample_file_dialog.h"

#include "tracker/key_bindings.h"



/****** FileSystem ********/

bool FileSystemSample::list_dir_begin() {

	if (inside_song) {

		sample_list_idx=0;
	} else {
		return fs->list_dir_begin();
	}
}

String FileSystemSample::get_next(bool* p_is_dir) {

	if (inside_song) {
		
		if (sample_list_idx<0)
			return "";
	
		else if (sample_list_idx==0) {
			
			if (p_is_dir)
				*p_is_dir=true;
			sample_list_idx++;
			return "..";
		} else {
			
			while(true) {
				
				if (sample_list_idx>Song::MAX_SAMPLES) {
					
					sample_list_idx=-1;
					return "";	// ??
					
				}
					
				Sample *s=song.get_sample( sample_list_idx -1 );
				sample_list_idx++;
				if (!s) {
					
					sample_list_idx=-1;
					return "";	// ??
				}
				
				if (s->get_sample_data().is_null()) {
					
					continue;
				}
				
				*p_is_dir=false;
				String sample_num=((sample_list_idx-1)<10)?("0"+String::num(sample_list_idx-1)):String::num(sample_list_idx-1);
				return sample_num +" - "+ s->get_name();
			}
		}
		
	} else {
		
		return fs->get_next( p_is_dir );
	}
}
void FileSystemSample::list_dir_end() {

	if (inside_song) {
	
		sample_list_idx=-1;
	} else {
		
		fs->list_dir_end();
	}
	
}

int FileSystemSample::get_drive_count() {

	return fs->get_drive_count();
}
String FileSystemSample::get_drive(int p_drive) {

	return fs->get_drive(p_drive);
}

bool FileSystemSample::change_dir(String p_dir) {

	if (p_dir.length()==0)
		return true;
	
	
	if (inside_song && p_dir=="..") {
		
		//exit
		
		fs->change_dir( pre_inside_song_path );
		tracker->virtual_piano->sample_stop_all();
		tracker->virtual_piano->sample_set( Sample_ID() );				
		song.reset();
		inside_song=false;
		
		return false;
		
	} if (file_exists( get_current_dir()+"/"+p_dir ) && tracker->file_manager.load_song( (get_current_dir()+"/"+p_dir).utf8().get_data(), & song )==Loader::FILE_OK) {
		
		
		inside_song=true;
		inside_song_path=p_dir;
		sample_list_idx=-1;
		pre_inside_song_path=fs->get_current_dir();
		return false;
		
	} else {
		if (fs->change_dir(p_dir)) {
			
			
			
			return true;
		} else {
			
			
			
			tracker->virtual_piano->sample_stop_all();
			tracker->virtual_piano->sample_set( Sample_ID() );				
			song.reset(); //clear up stuff
			inside_song=false;
			return false;
		}
	}
	
	return false;
}

void FileSystemSample::load_sample_from_inside_song(String p_file,Sample *p_sample) {
	if (p_file.find("-")>0) {
		
		int sample_idx=p_file.substr( 0, p_file.find("-") ).to_int();
		if (sample_idx>=1 && sample_idx<=Song::MAX_SAMPLES && !song.get_sample(sample_idx-1)->get_sample_data().is_null()) {
			
			*p_sample=*song.get_sample(sample_idx-1);	
		}
	}
	
}
void FileSystemSample::set_sample_in_virtual_piano(String p_file) {
	
	
	if (!external_sample.get_sample_data().is_null()) {
		
		external_sample.reset();
	}
	
	if (inside_song) {
		
		
		if (p_file.find("-")>0) {
			
			int sample_idx=p_file.substr( 0, p_file.find("-") ).to_int();
			
			if (sample_idx>=1 && sample_idx<=Song::MAX_SAMPLES && !song.get_sample(sample_idx-1)->get_sample_data().is_null()) {
				
				
				tracker->virtual_piano->sample_set( song.get_sample(sample_idx-1)->get_sample_data() );		
								
			}
		}
	} else {
		
		
		if (tracker->file_manager.load_sample( p_file.utf8().get_data(), &external_sample )!=Loader::FILE_OK)
			return;
		
		tracker->virtual_piano->sample_set( external_sample.get_sample_data() );				
		
	}
}

String FileSystemSample::get_current_dir() {

	if (inside_song)
		return inside_song_path;
	else
		return fs->get_current_dir();
}

bool FileSystemSample::make_dir(String p_dir) {

	return fs->make_dir(p_dir);
}

bool FileSystemSample::file_exists(String p_file) {

	return fs->file_exists(p_file);
}

FileSystemSample::FileSystemSample(Tracker *p_tracker) {
	
	tracker=p_tracker;
	fs=FileSystem::instance_func();
	inside_song=false;
	sample_list_idx=-1;
	
}
FileSystemSample::~FileSystemSample() {


	delete fs;
}



/******* DIALOG ***********/

bool SampleFileDialog::ignore_filter() {

	return fs_sample.is_inside_song();

}

void SampleFileDialog::show() {

	if (tracker->song->has_instruments())
		options_mg->show();
	else
		options_mg->hide();
	
	
	FileDialog::show( FileDialog::MODE_OPEN_MULTI );
	tracker->virtual_piano->sample_stop_all();
	
	
}

void SampleFileDialog::key_handler(unsigned long p_unicode, unsigned long p_scan_code,bool p_press,bool p_repeat,int p_modifier_mask) {

	KeyBindList kbind=KeyBind::get_bind( p_scan_code|p_modifier_mask );
	
	if (kbind==KB_MAX)
		return;
	
	
	switch( kbind ) {
	
		case KB_OCTAVE_RAISE: 
		case KB_OCTAVE_LOWER: 
		case KB_PLAYER_STOP_SONG: {
			
			get_root()->key_unhandled_signal.call(p_unicode,p_scan_code,p_press,p_repeat,p_modifier_mask);			
					
		} break;		
		default: {
			
			/* test if this is a note */
			int note=tracker->editor->get_note_from_charcode( kbind );
			if (note>=0) {
		
				if (p_press)
					tracker->virtual_piano->sample_press_key( note);
				else
					tracker->virtual_piano->sample_stop( note );
		
				return;
			}
			
		} break;
	}
	

}

void SampleFileDialog::file_selected(String p_string) {

	if (!tracker)
		return;	
		
	tracker->virtual_piano->sample_stop_all();
	
	if (fs_sample.is_inside_song())
		fs_sample.set_sample_in_virtual_piano(p_string);
	else 
		fs_sample.set_sample_in_virtual_piano(fs_sample.get_current_dir()+"/"+p_string);
	
}

void SampleFileDialog::open_samples_callback() {
	
	
	if (get_mode()!=FileDialog::MODE_OPEN_MULTI)
		return;
	
		
	for (int i=0;i<get_file_count();i++) {

		

		if ((sample_to_load+i)>=Song::MAX_SAMPLES)
			break;
		
		if (fs_sample.is_inside_song()) {
			
			fs_sample.load_sample_from_inside_song( get_file(i), tracker->song->get_sample( sample_to_load+i ) );
		} else {
		
			if (i==0 && !fs_sample.change_dir( get_file( i ) )) {
				
				
				update_file_list();			
				return;
			}
			
			
			Sample *smp=tracker->song->get_sample( sample_to_load+i );
			
			
			Loader::Error err =tracker->file_manager.load_sample( get_path(i).utf8().get_data(), smp );
			
		}
		
		if (make_instruments->is_checked())
			tracker->song->make_instrument_from_sample( sample_to_load + i );
		
	}

	hide();
	samples_opened_signal.call();

}

void SampleFileDialog::set_sample_to_load(int p_sample) {
	
	sample_to_load=p_sample;
}
SampleFileDialog::SampleFileDialog(Window *p_parent,Tracker *p_tracker) : FileDialog(p_parent), fs_sample(p_tracker) {


	tracker=p_tracker;
	set_custom_filesystem( &fs_sample );

	key_unhandled_signal.connect(this,&SampleFileDialog::key_handler);
	
	multi_file_activated_signal.connect( this, &SampleFileDialog::open_samples_callback );
	
	sample_to_load=0;
	
	clear_filter_list();
	add_filter("IT Sample",".its");
	add_filter("RIFF Wave File",".wav");
	add_filter("IT Song (Library)",".it");
	add_filter("XM Song (Library)",".xm");
	add_filter("S3M Song (Library)",".s3m");
	
	options_mg=get_extra_vb()->add( new MarginGroup("Options"));
	make_instruments=options_mg->add( new CheckButton("Make Instrument(s) from Sample(s)"));

	set_incremental_search(false); /* can't use it here */
}


SampleFileDialog::~SampleFileDialog(){


}


