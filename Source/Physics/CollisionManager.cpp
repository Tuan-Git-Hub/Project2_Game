#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "MapManager.h"

using namespace ax;

void CollisionManager::init()
{
    auto contactListener = EventListenerPhysicsContact::create(); // Tạo sự kiện lắng nghe
    contactListener->onContactBegin = [](PhysicsContact& contact) { // Sử dụng lambda để gọi hàm
        return CollisionManager::onContactBegin(contact);
    };
    contactListener->onContactPreSolve = [](PhysicsContact& contact, PhysicsContactPreSolve& solve) { // Sử dụng lambda để gọi hàm
        return CollisionManager::onContactPreSolve(contact, solve);
    };
    contactListener->onContactSeparate = [](PhysicsContact& contact) {
        return CollisionManager::onContactSeparate(contact);
    };

    // Thêm listener vào event dispatcher của Director
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(contactListener, 1);
}

// Khi Bắt đầu va chạm
bool CollisionManager::onContactBegin(PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Kiểm tra nếu là player thì gọi hàm xử lý của player
    if (bodyA->getCategoryBitmask() == ObjectBitmask::Player || bodyB->getCategoryBitmask() == ObjectBitmask::Player)
    {
        auto playerNode = (bodyA->getCategoryBitmask() == ObjectBitmask::Player) ? (bodyA->getNode()) : (bodyB->getNode());
        auto otherNode = (playerNode == bodyA->getNode()) ? (bodyB->getNode()) : (bodyA->getNode());   
        if (otherNode->getPhysicsBody()->getCategoryBitmask() == ObjectBitmask::Trampoline)
        {
            static_cast<Trampoline*>(otherNode)->activateTrap();
        }
        else if (otherNode->getPhysicsBody()->getCategoryBitmask() == ObjectBitmask::Fruits)
        {
            static_cast<Item*>(otherNode)->updateItem();
        }
        else if (otherNode->getPhysicsBody()->getCategoryBitmask() == ObjectBitmask::Trigger)
        {
            MapManager::triggerHiddenMap(otherNode);
        }
        static_cast<Player*>(playerNode)->handleBeginCollisionWith(otherNode, contact);
    }
    return true;
}

// Khi duy trì va chạm
bool CollisionManager::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Kiểm tra nếu là player thì gọi hàm xử lý của player
    if (bodyA->getCategoryBitmask() == ObjectBitmask::Player || bodyB->getCategoryBitmask() == ObjectBitmask::Player)
    {
        auto playerNode = (bodyA->getCategoryBitmask() == ObjectBitmask::Player) ? (bodyA->getNode()) : (bodyB->getNode());
        auto otherNode = (playerNode == bodyA->getNode()) ? (bodyB->getNode()) : (bodyA->getNode());
        static_cast<Player*>(playerNode)->handlePreSolveCollisionWith(otherNode, contact, solve);
    }
    return true;
}

// Khi va chạm tách ra
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