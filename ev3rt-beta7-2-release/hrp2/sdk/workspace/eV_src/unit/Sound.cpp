#include "Sound.h"


/**
 * コンストラクタ
 */
// Sound::Sound()
//     	{
// 			ev3_speaker_set_volume(10);
// }
// static *Sound Sound::getInstance(){
//     static Sound sound;
//     ev3_speaker_set_volume(10);
//     return &sound;
// }

void Sound::setVolume(uint8_t volume){
	ev3_speaker_set_volume(volume);
}

void Sound::sound_main()
{
	if (bang_flag== true) {
		bang();
		bang_flag = false;
	}

	if (bomb_flag== true) {
		bomb();
		bomb_flag = false;
	}

	if (dryy_flag== true) {
		dryy();
		dryy_flag = false;
	}

	if (gon_flag== true) {
		gon();
		gon_flag = false;
	}

	if (kinoko_flag== true) {
		kinoko();
		kinoko_flag = false;
	}

	if (nya_flag== true) {
		nya();
		nya_flag = false;
	}

	if (ok_flag== true) {
		ok();
		ok_flag = false;
	}

	if (pon_flag== true) {
		pon();
		pon_flag = false;
	}

	if (punch_flag== true) {
		punch();
		punch_flag = false;
	}

	if (ready_flag== true) {
		ready();
		ready_flag = false;
	}

	if (rin_flag== true) {
		rin();
		rin_flag = false;
	}

	if (trumpet_flag== true) {
		trumpet();
		trumpet_flag = false;
	}

}

void Sound::bang()
{
	ev3_memfile_load("/ev3rt/sounds/bang.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::bomb()
{
	ev3_memfile_load("/ev3rt/sounds/bomb.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::dryy()
{
	ev3_memfile_load("/ev3rt/sounds/dryy.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::gon()
{
	ev3_memfile_load("/ev3rt/sounds/gon.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::kinoko()
{
	ev3_memfile_load("/ev3rt/sounds/kinoko.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::nya()
{
	ev3_memfile_load("/ev3rt/sounds/nya.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::ok()
{
	ev3_memfile_load("/ev3rt/sounds/ok.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::pon()
{
	ev3_memfile_load("/ev3rt/sounds/pon.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::punch()
{
	ev3_memfile_load("/ev3rt/sounds/punch.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}


void Sound::ready()
{
	ev3_memfile_load("/ev3rt/sounds/ready.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}

void Sound::rin()
{
	ev3_memfile_load("/ev3rt/sounds/rin.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}


void Sound::trumpet()
{
	ev3_memfile_load("/ev3rt/sounds/trumpet.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}
