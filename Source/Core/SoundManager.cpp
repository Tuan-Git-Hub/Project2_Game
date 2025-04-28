#include "SoundManager.h"

using namespace ax;

int SoundManager::_backgroundID = -1;
DataSettingMusic* SoundManager::dataMusic = &DataSettingMusic::getInstance();

DataSettingMusic* SoundManager::getdataMusic()
{
    return &DataSettingMusic::getInstance();
};

void SoundManager::playBackground(const std::string& name, bool loop) 
{
    if (dataMusic->isMuteMusic())
    {
        _backgroundID = AudioEngine::play2d(name, loop, 0);
        return;
    }
    if (_backgroundID != -1) 
    {
        AudioEngine::stop(_backgroundID);
    }
    _backgroundID = AudioEngine::play2d(name, loop, dataMusic->getDecimalVolume());
}

void SoundManager::stopBackground() 
{
    if (_backgroundID != -1) 
    {
        AudioEngine::stop(_backgroundID);
        _backgroundID = -1;
    }
}

void SoundManager::playEffect(const std::string& name) 
{
    if (!dataMusic->isMuteSFX())
    {
        AudioEngine::play2d(name, false, dataMusic->getDecimalVolume());
    }
}

void SoundManager::playEffectWithCooldown(const std::string& name, float timecooldown)
{
    if (dataMusic->isMuteSFX())
        return;
    static long long timeBegin = 0;
    auto timeNow = utils::getTimeInMilliseconds();
    if (timeNow - timeBegin > timecooldown * 1000)
    {
        AudioEngine::play2d(name, false, dataMusic->getDecimalVolume());
        timeBegin = timeNow;
    }
    
    //AudioEngine::play2d(name, false, dataMusic->getDecimalVolume());
} 

void SoundManager::setVolume(float volume)
{
    AudioEngine::setVolume(_backgroundID, volume);
    auto v = static_cast<int>(volume*100);
    dataMusic->setPercentVolume(v);
}

void SoundManager::pause_Music()
{
    if (_backgroundID != -1) 
    {
        AudioEngine::pause(_backgroundID);
    }
}

void SoundManager::resume_Music()
{
    if (_backgroundID != -1) 
    {
        AudioEngine::resume(_backgroundID);
        AudioEngine::setVolume(_backgroundID, dataMusic->getDecimalVolume());
    }
}