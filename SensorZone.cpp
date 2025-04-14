#include "SensorZone.h"

SensorZone::SensorZone()
{
    AXLOG("Khởi tạo mặc định!");
    sensorBody = nullptr;
    for (auto* obj : objectsInZone)
    {
        obj = nullptr;
    }
}

SensorZone::~SensorZone()
{
    AXLOG("Hủy SensorZone!");
}

SensorZone::SensorZone(const ax::Size& size)
{
    this->setContentSize(size);
    sensorBody = ax::PhysicsBody::createBox(size, ax::PhysicsMaterial(0, 0, 0));
    sensorBody->setDynamic(false);
    sensorBody->setCategoryBitmask(0x04);           // Gán loại của vùng cảm biến
    sensorBody->setContactTestBitmask(0x01);  // Kiểm tra va chạm với tất cả
    sensorBody->setCollisionBitmask(0x00);          // Không va chạm vật lý

    // Đặt sensorBody là vùng cảm biến
    sensorBody->setGravityEnable(false);
    sensorBody->setDynamic(false);
    this->setTag(static_cast<int>(TrapType::GenericTrap));  // Đánh dấu bẫy tương ứng với vùng Sensor

    this->setPhysicsBody(sensorBody);
}

void SensorZone::onEnter(ax::Node* obj)
{
    if (objectsInZone.find(obj) == objectsInZone.end())
    {
        objectsInZone.insert(obj);
        AXLOG("ĐÃ VÀO vùng sensor, ĐƯA đối tượng vào objectsInZone");
    }
}

void SensorZone::onExit(ax:: Node* obj)
{
    if (objectsInZone.erase(obj))
    {
        AXLOG("ĐÃ RỜI KHỎI vùng sensor, XÓA đối tượng khỏi objectsInZone");
    }
}

void SensorZone::checkObjectsInZone()
{
    for (auto* obj : objectsInZone)
    {
        std::cout << "🟡 " << obj->getName() << " đang trong vùng sensor.\n";
    }
}

void SensorZone::doActionToAll()
{
    AXLOG("Gây ảnh hưởng đến MỌI đối tượng!");
}

void SensorZone::doActionToOne(ax::Node* obj)
{
    AXLOG("Gây ảnh hưởng đến MỘT đối tượng: %p", obj);
}
