#ifndef SAMPLE_H
#define SAMPLE_H


#include "mixer/sample_manager.h"
#include "globals/config.h"

class Sample {

public:
	enum VibratoType {
		VIBRATO_SINE,
		VIBRATO_SAW,
		VIBRATO_SQUARE,
		VIBRATO_RANDOM

	};

private:
	
	enum { NAME_MAX_LEN=26 };
	
	char name[NAME_MAX_LEN];

	Uint8 default_volume; /* 0.. 64 */
	Uint8 global_volume; /* 0.. 64 */

	bool pan_enabled;
	Uint8 pan;  /* 0.. 64 */

	VibratoType vibrato_type;
	Uint8 vibrato_speed; /* 0.. 64 */
	Uint8 vibrato_depth; /* 0.. 64 */
	Uint8 vibrato_rate; /* 0.. 64 */

	Sample_ID id;
	
	void copy_from(const Sample &p_sample);
public:

	
	void operator=(const Sample &p_sample);
	
	const char * get_name() const;
	void set_name(const char *p_name);

	void set_default_volume(Uint8 p_vol);
	Uint8 get_default_volume() const;
	
	void set_global_volume(Uint8 p_vol);
	Uint8 get_global_volume() const;
	
	void set_pan_enabled(bool p_vol);
	bool is_pan_enabled() const;
	
	void set_pan(Uint8 p_pan);
	Uint8 get_pan() const;

	void set_vibrato_type(VibratoType p_vibrato_type);
	VibratoType get_vibrato_type() const;

	void set_vibrato_speed(Uint8 p_vibrato_speed) ;
	Uint8 get_vibrato_speed() const;

	void set_vibrato_depth(Uint8 p_vibrato_depth);
	Uint8 get_vibrato_depth() const;

	void set_vibrato_rate(Uint8 p_vibrato_rate);
	Uint8 get_vibrato_rate() const;

	void set_sample_data(Sample_ID);
	Sample_ID get_sample_data() const;
	
	void reset();
	
	Sample(const Sample&p_from);
	Sample();
	~Sample();
			
};




#endif
