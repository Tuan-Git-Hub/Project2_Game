#pragma once

#ifndef __SANDMUDICE_H__
#    define __SANDMUDICE_H__

#    include "Trap.h"

enum class SMIType : int
{
    Default = 0,
    Sand = 1,
    Mud = 2,
    Ice = 3
};

class SandMudIce : public Trap
{
protected:
    SMIType smiType;

public:
    SandMudIce();
    SandMudIce(SMIType _type);
    virtual ~SandMudIce();
 
    bool init(const std::string& spriteFile) override;
    void dealDamage(ax::Node* player) override;
    void activateTrap() override;
    void deactivateTrap() override;
};

#endif  // !__SandMudIce_H__
