#include "WindZone.h"

WindZone::WindZone() : SensorZone() {}
WindZone::WindZone(const ax::Size& size)
{
    this->setContentSize(size);

    this->setAnchorPoint(Vec2(0.5, 0));

    sensorBody = ax::PhysicsBody::createBox(size, ax::PhysicsMaterial(0, 0, 0));
    sensorBody->setDynamic(false);
    sensorBody->setCategoryBitmask(0x04);           // Gán loại của vùng cảm biến
    sensorBody->setContactTestBitmask(0xFFFFFFFF);  // Kiểm tra va chạm với người chơi
    sensorBody->setCollisionBitmask(0x00);          // Không va chạm vật lý

    // Đặt sensorBody là vùng cảm biến
    sensorBody->setGravityEnable(false);
    sensorBody->setDynamic(false);
    sensorBody->setTag(static_cast<int>(TrapType::WindZone));  // Đánh dấu bẫy tương ứng với vùng Sensor

    this->setPhysicsBody(sensorBody);
}

WindZone::WindZone(const ax::Size& size, ax::Vec2& _DeftForceVector)
{
    this->setContentSize(size);
    this->setAnchorPoint(Vec2(0.5, 0));

    this->DeftForceVector = _DeftForceVector;

    sensorBody = ax::PhysicsBody::createBox(size, ax::PhysicsMaterial(0, 0, 0));
    sensorBody->setDynamic(false);
    sensorBody->setCategoryBitmask(0x04);           // Gán loại của vùng cảm biến
    sensorBody->setContactTestBitmask(0xFFFFFFFF);  // Kiểm tra va chạm với người chơi
    sensorBody->setCollisionBitmask(0x00);          // Không va chạm vật lý

    // Đặt sensorBody là vùng cảm biến
    sensorBody->setGravityEnable(false);
    sensorBody->setDynamic(false);
    sensorBody->setTag(static_cast<int>(TrapType::WindZone));  // Đánh dấu bẫy tương ứng với vùng Sensor

    this->setPhysicsBody(sensorBody);
}

void WindZone::doAction()
{
    AXLOG("Gây ảnh hưởng lên vật thể trong vùng ảnh hưởng!");
    for (auto* obj : objectsInZone)
    {
        if (obj)
        {
            auto objBody = obj->getPhysicsBody();
            objBody->applyForce(DeftForceVector);
        }
        else
        {
            AXLOG("Cảnh báo: Player không có PhysicsBody! Không thể tác động lực.");
        }
    }
}
