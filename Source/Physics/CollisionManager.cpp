#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "Player.h"

using namespace ax;

void CollisionManager::init()
{
    auto contactListener = EventListenerPhysicsContact::create(); // Tạo sự kiện lắng nghe
    contactListener->onContactBegin = [](PhysicsContact& contact) { // Sử dụng lambda để gọi hàm
        return CollisionManager::onContactBegin(contact);
    };
    contactListener->onContactSeparate = [](PhysicsContact& contact) {
        return CollisionManager::onContactSeparate(contact);
    };

    // Thêm listener vào event dispatcher của Director
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);
}

bool CollisionManager::onContactBegin(PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Kiểm tra nếu là player thì gọi hàm xử lý của player
    if (bodyA->getCategoryBitmask() == ObjectBitmask::Player || bodyB->getCategoryBitmask() == ObjectBitmask::Player)
    {
        auto playerNode = (bodyA->getCategoryBitmask() == ObjectBitmask::Player) ? (bodyA->getNode()) : (bodyB->getNode());
        auto otherNode = (playerNode == bodyA->getNode()) ? (bodyB->getNode()) : (bodyA->getNode());
        static_cast<Player*>(playerNode)->handleBeginCollisionWith(otherNode, contact);
    }
    return true;
}

void CollisionManager::onContactSeparate(PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Kiểm tra nếu là player thì gọi hàm xử lý của player
    if (bodyA->getCategoryBitmask() == ObjectBitmask::Player || bodyB->getCategoryBitmask() == ObjectBitmask::Player)
    {
        auto playerNode = (bodyA->getCategoryBitmask() == ObjectBitmask::Player) ? (bodyA->getNode()) : (bodyB->getNode());
        auto otherNode = (playerNode == bodyA->getNode()) ? (bodyB->getNode()) : (bodyA->getNode());
        static_cast<Player*>(playerNode)->handleSeparateCollisionWith(otherNode, contact);
    }
}