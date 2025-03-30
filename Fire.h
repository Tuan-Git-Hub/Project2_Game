#pragma once

#ifndef __FIRE_H__
#define __FIRE_H__

#include "axmol.h"
#include "Trap.h"

class Fire : public Trap
{
public:
    Fire();
    ~Fire();
    bool init(const std::string& spriteFile) override;
    void dealDamage(ax::Node* player) override;
    void activateTrap() override;
    void deactivateTrap() override;
};

#endif  // !__FIRE_H__
