#ifndef __DATA_SETTING_MUSIC_H__
#define __DATA_SETTING_MUSIC_H__

#include "axmol.h"

class DataSettingMusic
{
private:
    DataSettingMusic() {}; // Đảm bảo rằng chỉ có thể tạo ra một instance duy nhất
    DataSettingMusic(const DataSettingMusic&) = delete; // Ngăn sao chép
    DataSettingMusic& operator=(const DataSettingMusic&) = delete; // Ngăn gán object

    bool _isMuteMusic = false;
    bool _isMuteSFX = false;
    int _percentVolume = 96;
    
public:
    static DataSettingMusic& getInstance();

    bool isMuteMusic() const { return _isMuteMusic; };
    bool isMuteSFX() const { return _isMuteSFX; };
    int getPercentVolume() const { return _percentVolume; };
    float getDecimalVolume() { return _percentVolume/100.0f; };

    void toggleMusic()  { _isMuteMusic = !_isMuteMusic; };;
    void toggleSFX() { _isMuteSFX = !_isMuteSFX; };
    void setPercentVolume(int p) { _percentVolume = p; };       
};


#endif  // __DATA_SETTING_MUSIC_H__