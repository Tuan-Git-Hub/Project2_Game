#pragma once

#ifndef __FAN_H__
#    define __FAN_H__

#    include "axmol.h"
#    include "Trap.h"
#    include "WindZone.h"

class Fan : public Trap
{
protected:
    ax::Vec2 DeftForceVector = ax::Vec2(0, 3000.0f);
    WindZone* windZone;

public:
    Fan();
    ~Fan();
    bool init(const std::string& spriteFile) override;
    void dealDamage(ax::Node* player) override;
    void activateTrap() override;
    void deactivateTrap() override;

    WindZone* getWindZone();

};

#endif  // !__FAN_H__
