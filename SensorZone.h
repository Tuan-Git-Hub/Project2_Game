#ifndef __SENSOR_ZONE_H__
#define __SENSOR_ZONE_H__

#include "axmol.h"
#include "Trap.h"

#include <unordered_set>
#include <iostream>

USING_NS_AX;

class SensorZone: public ax::Node
{
protected:
    ax::PhysicsBody* sensorBody;                  // Vùng cảm biến vật lý
    std::unordered_set<ax::Node*> objectsInZone;  // Lưu các đối tượng đang ở trong vùng

public:
    SensorZone();
    virtual ~SensorZone();
    SensorZone(const ax::Size& size);
    

    // Khi có vật thể vào vùng cảm biến
    void onEnter(ax::Node* obj);

    // Khi có vật thể rời khỏi vùng cảm biến
    void onExit(ax::Node* obj);

    // Kiểm tra các vật thể đang ở trong vùng cảm biến
    void checkObjectsInZone();

    // Tác động lên các vật thể trong vùng cảm biến:
    virtual void doActionToAll();
    virtual void doActionToOne(ax::Node* obj);
    
};

#endif
