#include "WindZone.h"

WindZone::WindZone() : SensorZone() {}
WindZone::~WindZone()
{
    AXLOG("Hủy WindZone!");
}
WindZone::WindZone(const ax::Size& size)
{
    this->setContentSize(size);

    this->setAnchorPoint(Vec2(0.5, 0));

    sensorBody = ax::PhysicsBody::createBox(size, ax::PhysicsMaterial(0, 0, 0));
    sensorBody->setDynamic(false);
    sensorBody->setCategoryBitmask(0x04);           // Gán loại của vùng cảm biến
    sensorBody->setContactTestBitmask(0x01);  // Kiểm tra va chạm với người chơi
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
    this->setTag(static_cast<int>(TrapType::WindZone));  // Đánh dấu bẫy tương ứng với vùng Sensor

    this->setPhysicsBody(sensorBody);
}

void WindZone::doActionToAll()
{
    AXLOG("Gây ảnh hưởng lên %zu vật thể trong vùng WindZone", objectsInZone.size());
    AXLOG("Đẩy MỌI vật thể trong vùng WindZone");

    for (auto* obj : objectsInZone)
    {
        if (!obj)
        {
            AXLOG("⚠️ Cảnh báo: obj là nullptr!");
            continue;
        }

        auto objBody = obj->getPhysicsBody();
        if (objBody)
        {
            AXLOG("Khỗi lượng của đối tượng:  %.3f", objBody->getMass());
            objBody->applyImpulse(DeftForceVector);
            AXLOG("✅ Đã đẩy đối tượng: %p", obj);
        }
        else
        {
            AXLOG("⚠️ Cảnh báo: Đối tượng %p không có PhysicsBody!", obj);
        }
    }
}

void WindZone::doActionToOne(ax::Node* obj) {
    AXLOG("Gây ảnh hưởng lên %zu vật thể trong vùng WindZone", objectsInZone.size());
    AXLOG("Đẩy MỘT vật thể trong vùng WindZone: %p", obj);

        if (!obj)
        {
            AXLOG("⚠️ Cảnh báo: obj là nullptr!");
        }

        if (objectsInZone.find(obj) == objectsInZone.end())
        {
            AXLOG("⚠️ Cảnh báo: obj không nằm trong objectsInZone!");
        }

        else
        {
            auto objBody = obj->getPhysicsBody();
            if (objBody)
            {
                AXLOG("Khỗi lượng của đối tượng:  %.3f", objBody->getMass());
                objBody->applyImpulse(DeftForceVector);
                AXLOG("✅ Đã đẩy đối tượng: %p", obj);
            }
            else
            {
                AXLOG("⚠️ Cảnh báo: Đối tượng %p không có PhysicsBody!", obj);
            }
        }
}
