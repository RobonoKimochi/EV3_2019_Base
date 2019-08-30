#ifndef EV3_UNIT_SOUND_H_
#define EV3_UNIT_SOUND_H_

#include "ev3api.h"
#include "app_config.h"

class Sound
{
    public:
        static Sound *getInstance(){
            static Sound sound;
            ev3_speaker_set_volume(SOUND_VOL);
            return &sound;
        }

        void sound_main();

        // Sound();
        void bang();
        void bomb();
        void dryy();
        void gon();
        void kinoko();
        void nya();
        void ok();
        void pon();
        void punch();
        void ready();
        void rin();
        void trumpet();

        // 音を鳴らすフラグ
        bool bang_flag = false;
        bool bomb_flag = false;
        bool dryy_flag = false;
        bool gon_flag = false;
        bool kinoko_flag = false;
        bool nya_flag = false;
        bool ok_flag = false;
        bool pon_flag = false;
        bool punch_flag = false;
        bool ready_flag = false;
        bool rin_flag = false;
        bool trumpet_flag = false;

        void setVolume(uint8_t volume);
    private:
        uint8_t mVolume;
        memfile_t soundfile;
};

#endif  // EV3_UNIT_SOUND_H_
