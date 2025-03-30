#include "CollisionHandler.h"

USING_NS_AX;

bool CollisionHandler::onContactBegin(ax::PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    AXLOG("Có va chạm xảy ra!");

    // Kiểm tra nếu Logo chạm Tường
    if ((bodyA->getCategoryBitmask() == 0x01 && bodyB->getCategoryBitmask() == 0x03) ||
        (bodyA->getCategoryBitmask() == 0x03 && bodyB->getCategoryBitmask() == 0x01))
    {
        AXLOG("Logo đã chạm Tường!");
    }

    // Kiểm tra nếu va chạm với bẫy (CategoryBitmask = 0x02)
    Node* trapNode      = nullptr;
    Node* passiveNode   = nullptr;

    if (bodyA->getCategoryBitmask() == 0x02)
    {
        trapNode    = bodyA->getNode();
        passiveNode = bodyB->getNode();
    }
    else if (bodyB->getCategoryBitmask() == 0x02)
    {
        trapNode    = bodyB->getNode();
        passiveNode = bodyA->getNode();
    }

    if (trapNode && passiveNode)
    {
        // Lấy toạ độ điểm va chạm
        ax::Vec2 contactPoint = contact.getContactData()->points[0];

        // Chuyển về toạ độ theo AnchorPoint
        Vec2 _contactPoint = contactPoint - trapNode->getPosition();

        auto trap = dynamic_cast<Trap*>(trapNode);
        if (trap)
        {
            switch (trap->getTag())
            {
            case (static_cast<int>(TrapType::Spike)):
                static_cast<Spike*>(trap)->dealDamage(passiveNode);
                break;
            case (static_cast<int>(TrapType::Trampoline)):
                static_cast<Trampoline*>(trap)->activateTrap();
                static_cast<Trampoline*>(trap)->dealDamage(passiveNode);
                break;
            case (static_cast<int>(TrapType::Fire)):
                static_cast<Fire*>(trap)->activateTrap();
                static_cast<Fire*>(trap)->dealDamage(passiveNode);
                break;
            default:
                trap->dealDamage(passiveNode);
                break;
            }
        }
    }

    return true;
}

bool CollisionHandler::onContactPreSolve(ax::PhysicsContact& contact, ax::PhysicsContactPreSolve& solve)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Kiểm tra nếu va chạm với bẫy (CategoryBitmask = 0x02)
    Node* trapNode    = nullptr;
    Node* passiveNode = nullptr;

    if (bodyA->getCategoryBitmask() == 0x02)
    {
        trapNode    = bodyA->getNode();
        passiveNode = bodyB->getNode();
    }
    else if (bodyB->getCategoryBitmask() == 0x02)
    {
        trapNode    = bodyB->getNode();
        passiveNode = bodyA->getNode();
    }

    if (trapNode && passiveNode)
    {
        auto trap = dynamic_cast<Trap*>(trapNode);
        if (trap)
        {
            switch (trap->getTag())
            {
            case (static_cast<int>(TrapType::SandMudIce)):
                static_cast<SandMudIce*>(trap)->activateTrap();
                static_cast<SandMudIce*>(trap)->dealDamage(passiveNode);
                break;
            default:
                trap->dealDamage(passiveNode);
                break;
            }
        }
    }
    return true;
}
