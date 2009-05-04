
#include "sample.h"

const char * Sample::get_name() const {

	return name;
}
void Sample::set_name(const char *p_name) {

	if (p_name==NULL) {
		name[0]=0;
		return;
	}
	
	
	bool done=false;
	for (int i=0;i<NAME_MAX_LEN;i++) {
		
		
		name[i]=done?0:p_name[i];
		if (!done && p_name[i]==0)
			done=true;
	}
	
	name[NAME_MAX_LEN-1]=0; /* just in case */
	
}

void Sample::set_default_volume(Uint8 p_vol) {

	default_volume=p_vol;
}
Uint8 Sample::get_default_volume() const{

	return default_volume;
}

void Sample::set_global_volume(Uint8 p_vol) {

	global_volume=p_vol;
}
Uint8 Sample::get_global_volume() const{

	return global_volume;
}

void Sample::set_pan_enabled(bool p_vol) {

	pan_enabled=p_vol;
}
bool Sample::is_pan_enabled() const{

	return pan_enabled;
}

void Sample::set_pan(Uint8 p_pan) {

	pan=p_pan;

}
Uint8 Sample::get_pan() const{

	return pan;
}


void Sample::set_vibrato_type(VibratoType p_vibrato_type) {

	vibrato_type=p_vibrato_type;
}
Sample::VibratoType Sample::get_vibrato_type()  const{

	return vibrato_type;
}

void Sample::set_vibrato_speed(Uint8 p_vibrato_speed) {

	vibrato_speed=p_vibrato_speed;
}
Uint8 Sample::get_vibrato_speed() const {

	return vibrato_speed;
}

void Sample::set_vibrato_depth(Uint8 p_vibrato_depth) {

	vibrato_depth=p_vibrato_depth;
}
Uint8 Sample::get_vibrato_depth() const{

	return vibrato_depth;
}

void Sample::set_vibrato_rate(Uint8 p_vibrato_rate) {

	vibrato_rate=p_vibrato_rate;
}
Uint8 Sample::get_vibrato_rate() const{

	return vibrato_rate;
}

void Sample::set_sample_data(Sample_ID p_ID) {
	
	id=p_ID;
}
Sample_ID Sample::get_sample_data() const{
	
	return id;
}

void Sample::operator=(const Sample &p_sample) {
	
	copy_from(p_sample);
}
void Sample::copy_from(const Sample &p_sample) {
	
	reset();
	set_name(p_sample.get_name());
	
	default_volume=p_sample.default_volume;
	global_volume=p_sample.global_volume;

	pan_enabled=p_sample.pan_enabled;
	pan=p_sample.pan;

	vibrato_type=p_sample.vibrato_type;
	vibrato_speed=p_sample.vibrato_speed;
	vibrato_depth=p_sample.vibrato_depth;
	vibrato_rate=p_sample.vibrato_rate;
	
	if (SampleManager::get_singleton() && !p_sample.id.is_null())
		SampleManager::get_singleton()->copy_to( p_sample.id, id );
}


	


void Sample::reset() {

	
	name[0]=0;

	default_volume=64;
	global_volume=64;

	pan_enabled=false;
	pan=32;

	vibrato_type=VIBRATO_SINE;
	vibrato_speed=0;
	vibrato_depth=0;
	vibrato_rate=0;

	if (!id.is_null() && SampleManager::get_singleton())
		SampleManager::get_singleton()->destroy( id );
	
	id=Sample_ID();
	
}

Sample::Sample(const Sample&p_from) {
	
	reset();
	copy_from(p_from);
}
Sample::Sample() {

	reset();
}

Sample::~Sample() {

	reset();
}
