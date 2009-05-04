

#include "audio_lock.h"
#include "sound_driver_manager.h"



void AudioLock::begin() {

	SoundDriverManager::lock_driver();
}

void AudioLock::end() {

	SoundDriverManager::unlock_driver();

}







