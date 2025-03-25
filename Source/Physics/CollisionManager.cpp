#include "CollisionManager.h"

using namespace ax;

void CollisionManager::init()
{
    auto contactListener = EventListenerPhysicsContact::create(); // Tạo sự kiện lắng nghe
    contactListener->onContactBegin = [](PhysicsContact& contact) { // Sử dụng lambda để gọi hàm
        return CollisionManager::onContactBegin(contact);
    };

    // Thêm listener vào event dispatcher của Director
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);
}

bool CollisionManager::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA->getTag() == 2 || nodeB->getTag() == 2) // Nếu 1 trong 2 là Player, để Player tự xử lý
    {
        return true;
    }
}