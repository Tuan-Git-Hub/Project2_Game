#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "axmol.h"
#include "audio/AudioEngine.h"
#include "DataSettingMusic.h"

namespace AudioPaths 
{
    const std::string MAINMENU_BACKGROUND = "res/sound_game/Music/Retro/Retro Beat.ogg";
    const std::string LEVEL1_BACKGROUND = "res/sound_game/Music/Loops/Swinging Pants.ogg";
    const std::string LEVEL2_BACKGROUND = "res/sound_game/Music/Loops/Retro Mystic.ogg";
    const std::string LEVEL3_BACKGROUND = "res/sound_game/Music/Loops/Time Driving.ogg";
    const std::string LEVEL4_BACKGROUND = "res/sound_game/Music/Loops/Night at the Beach.ogg";

    const std::string CLICK = "res/sound_game/SFX/click/click_001.ogg";
    const std::string GAMEOVER_SFX = "res/sound_game/SFX/gameoversfx/game-over-SFX.wav";
    const std::string PLAYER_RUN = "res/sound_game/SFX/player/footstep_grass_000.ogg";
    const std::string PLAYER_JUMP = "res/sound_game/SFX/player/jump1.ogg";
    const std::string PLAYER_HIT = "res/sound_game/SFX/player/hit_01.ogg";
    const std::string ADD_POINT = "res/sound_game/SFX/point/add_point.wav";
    const std::string HIT_BOX = "res/sound_game/SFX/box/hit_box.wav";
    const std::string BROKEN_BOX = "res/sound_game/SFX/box/broken_box.ogg";
    const std::string CHECKPOINT = "res/sound_game/SFX/checkpoint/jingles-steel_08.ogg";
    const std::string TRAMPOLINE = "res/sound_game/SFX/Trap/hydraulic-cut.wav";
    const std::string FIRE = "res/sound_game/SFX/Trap/fire-cut.wav";
    const std::string FAN = "res/sound_game/SFX/Trap/fan-94808.mp3";

};

class SoundManager
{
public:
    static DataSettingMusic* getdataMusic();
    static void playBackground(const std::string& name, bool loop = true);
    static void stopBackground();

    static void playEffect(const std::string& name);
    static void playEffectWithCooldown(const std::string& name, float time);
    static void setVolume(float volume);
    static void pause_Music();
    static void resume_Music();

private:
    static int _backgroundID;
    static DataSettingMusic* dataMusic;
};


#endif  // __SOUND_MANAGER_H__