#pragma once

#ifndef __TRAMPOLINE_H__
#define __TRAMPOLINE_H__

#include "axmol.h"
#include "Trap.h"

class Trampoline : public Trap
{
protected:
    ax::Vec2 DefltApplyVelo = ax::Vec2(500, 0);  // Vector biến thiên vận tốc, mặc định hướng lên trên
public:
    Trampoline();
    ~Trampoline();
    bool init(const std::string& spriteFile) override;
    void dealDamage(ax::Node* player) override;
    void activateTrap() override;
    void deactivateTrap() override;
};

#endif  // !__TRAMPOLINE_H__
