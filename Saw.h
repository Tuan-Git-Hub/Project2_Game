#pragma once

#ifndef __SAW_H__
#define __SAW_H__

#include "axmol.h"
#include <cmath>
#include "Trap.h"

class Saw : public Trap
{
protected:
    float amplitude;     // Biên độ dao động (độ lệch lớn nhất)
    float frequency;     // Tần số dao động (số lần lặp lại mỗi giây)

public:
    Saw();
    ~Saw();
    bool init(const std::string& spriteFile) override;
    void dealDamage(ax::Node* player) override;  
    void activateTrap() override;
    void deactivateTrap() override;

    void setAmplitude(float _amp);
    float getAmplitude();

    void setFrequency(float _freq);
    float getFrequency();

    void runAnimation();

    void startOscillation();
    void stopOscillation();

    // void checkWallCollision();
};

#endif  // !__SAW_H__
