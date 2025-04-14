#ifndef __SPIKE_H__
#define __SPIKE_H__

#include "Trap.h"

class Spike : public Trap
{
public:
    Spike();
    virtual ~Spike();

    bool init() override;
    void dealDamage(ax::Node* player) override;
    void activateTrap() override;
    void deactivateTrap() override;
};

#endif  // __SPIKE_H__