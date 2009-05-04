//
// C++ Implementation: nds_backends
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "nds_backends.h"
#include <unistd.h>
#include "mixer/mixer.h"

FileSystem *FileSystemNDS::create_fs() {
	
	return new FileSystemNDS;
}

bool FileSystemNDS::list_dir_begin() {
	
	list_dir_end(); //close any previous dir opening!
	
//	char real_current_dir_name[2048]; //is this enough?!	
	//getcwd(real_current_dir_name,2048);	
	//chdir(curent_path.utf8().get_data());
	dir_stream = diropen(current_dir.utf8().get_data());
	//chdir(real_current_dir_name);	
	
	if (!dir_stream)
		return true; //error!
	
	dirreset (dir_stream);
	return false;
}

bool FileSystemNDS::file_exists(String p_file) {
	
	
	struct stat flags;	
	if ((stat(p_file.utf8().get_data(),&flags)!=0))
		return false;
	
	if (S_ISDIR(flags.st_mode))
		return false;
	
	return true;
	
}

String FileSystemNDS::get_next(bool* p_is_dir) {
	
	if (!dir_stream)
		return "";
	
	
	char filename[256]; // to hold a full filename and string terminator	
	struct stat st;
	
	if (dirnext(dir_stream, filename, &st) != 0) {
		list_dir_end();
		return "";
	}
	
	
	String fname;
	if (fname.parse_utf8(filename))
		fname=filename; //no utf8, maybe latin?
	
	String f=current_dir+"/"+fname;
	
	if (p_is_dir) {
			
		*p_is_dir=st.st_mode & S_IFDIR;		
	}
		
	
	return fname;
	
}
void FileSystemNDS::list_dir_end() {
	
	if (dir_stream)
		dirclose(dir_stream);	
	dir_stream=0;
}

int FileSystemNDS::get_drive_count() {
	
	return 0;
}
String FileSystemNDS::get_drive(int p_drive) {
	
	return "";
}

bool FileSystemNDS::make_dir(String p_dir) {
	
	char real_current_dir_name[2048];
	getcwd(real_current_dir_name,2048);
	chdir(current_dir.utf8().get_data()); //ascii since this may be unicode or wathever the host os wants
	
	bool success=(mkdir(p_dir.utf8().get_data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0);
	
	chdir(real_current_dir_name);
	
	return !success;
}


bool FileSystemNDS::change_dir(String p_dir) {
	
	char real_current_dir_name[2048];
	getcwd(real_current_dir_name,2048);
	String prev_dir;
	if (prev_dir.parse_utf8(real_current_dir_name))
		prev_dir=real_current_dir_name; //no utf8, maybe latin?

	chdir(current_dir.utf8().get_data()); //ascii since this may be unicode or wathever the host os wants
	bool worked=(chdir(p_dir.utf8().get_data())==0); // we can only give this utf8
	

	if (worked) {
	
		getcwd(real_current_dir_name,2048);
		if (current_dir.parse_utf8(real_current_dir_name))
			current_dir=real_current_dir_name; //no utf8, maybe latin?
	}
	
	chdir(prev_dir.utf8().get_data());
	
	return !worked;
	
}

String FileSystemNDS::get_current_dir() {
	
	return current_dir;
}

void FileSystemNDS::set_default_filesystem() {
	
	FileSystem::instance_func=&FileSystemNDS::create_fs;
}

FileSystemNDS::FileSystemNDS() {
	
	dir_stream=0;
	current_dir=".";	
	
	/* determine drive count */

	change_dir(current_dir);
	
}


FileSystemNDS::~FileSystemNDS() {
	
	list_dir_end();
}



/******** SOUND DRIVER ************/




void SoundDriverNDS::lock() { 
	
	lockc++;
}
void SoundDriverNDS::unlock() {
	
	lockc--;
	
}
const char * SoundDriverNDS::get_name() {
	
	return "NDS Hardware";
}
Uint16 SoundDriverNDS::get_max_level_l() { 
	
	return 0;
}

Uint16 SoundDriverNDS::get_max_level_r() { 
	
	
	return 0;
}
bool SoundDriverNDS::is_active() {
	
	
	return true;
}	

bool SoundDriverNDS::init() {
	return false;
}	
void SoundDriverNDS::finish() {
	
	
}
void SoundDriverNDS::set_audio_params(AudioParams *p_params) {
	
//	params=p_params;
}	

SoundDriverNDS::SoundDriverNDS() {
	
	lockc=false;
}
SoundDriverNDS::~SoundDriverNDS() {
	
	
}


	
TimerID TimerNDS::create_timer( Method p_callback, int p_interval_msec ) {
	
	return -1;
}
void TimerNDS::remove_timer(TimerID p_timer) {
	
	
}
int TimerNDS::get_interval(TimerID p_timer) {
	
	return 0;
}
void TimerNDS::change_timer_interval(TimerID p_timer,int p_interval_msec) {
	
	
}
unsigned long int TimerNDS::get_tick_ms()  {
	
	return 0;
}
		
 /* get a millisecond tick, for delta times and not synced to clock */

TimerNDS::TimerNDS()  {


}
TimerNDS::~TimerNDS()  {


}
	

	
