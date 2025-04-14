#ifndef __WIND_ZONE_H__
#define __WIND_ZONE_H__
#include "axmol.h"
#include "Trap.h"

#include <iostream>
#include "SensorZone.h"

USING_NS_AX;

class WindZone: public SensorZone
{
protected:
    ax::Vec2 DeftForceVector = ax::Vec2(0, 30000.0f);

public:
    WindZone();
    virtual ~WindZone();
    WindZone(const ax::Size& size);
    WindZone(const ax::Size& size, ax::Vec2& deftForceVector);
    void doActionToAll() override;
    void doActionToOne(ax::Node* obj) override;

};

#endif
