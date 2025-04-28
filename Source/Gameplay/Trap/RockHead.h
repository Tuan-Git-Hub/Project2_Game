#ifndef __ROCK_HEAD_H__
#define __ROCK_HEAD_H__

#include "Trap.h"

class RockHead : public Trap
{
protected:
    float amplitude;     // Biên độ dao động (độ lệch lớn nhất)
    float frequency;     // Tần số dao động (số lần lặp lại mỗi giây)

public:
    RockHead();
    ~RockHead();
    bool init() override;

    void activateTrap(std::string axis);
    void deactivateTrap() override;

    void setAmplitude(float _amp);
    float getAmplitude();

    void setFrequency(float _freq);
    float getFrequency();

    ax::Animation* getAnimation(std::string animName);

    void startBehavior(std::string axis);
    void stopBehavior();
};

#endif  // !__ROCK_HEAD_H__
