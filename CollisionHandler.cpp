#include "CollisionHandler.h"

USING_NS_AX;

bool CollisionHandler::onContactBegin(ax::PhysicsContact& contact)
{
    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Lấy dữ liệu va chạm
    const ax::PhysicsContactData* contactData = contact.getContactData();
    if (!contactData)
        return false;

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
        // Vector pháp tuyến của va chạm
        /*
            Hướng của vector pháp tuyến (normal vector) trong một va chạm vật lý
            luôn hướng ra khỏi bodyA và trỏ về phía body
        B*/
        ax::Vec2 normal = contactData->normal;

        
        /*
                *   Lấy hướng của vector pháp tuyến (normal)

                        Nếu trapNode là bodyA, thì normal đã đúng hướng.

                        Nếu trapNode là bodyB, thì normal cần đảo dấu.

                    Sử dụng một biến Vec2 correctedNormal để chuẩn hóa hướng pháp tuyến.

                        Nếu trapNode là bodyA, correctedNormal = normal.

                        Nếu trapNode là bodyB, correctedNormal = -normal.
                */
       

        // Chuẩn hóa hướng pháp tuyến dựa trên trapNode
        Vec2 correctedNormal = (trapNode == bodyA->getNode()) ? normal : -normal;

        auto trap = dynamic_cast<Trap*>(trapNode);
        if (trap)
        {
            switch (trap->getTag())
            {
            case (static_cast<int>(TrapType::Spike)):
                static_cast<Spike*>(trap)->dealDamage(passiveNode);
                break;
            case (static_cast<int>(TrapType::Trampoline)):
            {
                // Lấy góc xoay của Trampoline
                float rotation = static_cast<Trampoline*>(trap)->getRotation();

                bool isValidCollision = false;

                if (rotation == 0.0f)
                {
                    // Đứng thẳng, chỉ chấp nhận va chạm từ trên xuống
                    isValidCollision = (correctedNormal.y > 0.98f);
                }
                else if (rotation == -90.0f)
                {
                    // Xoay trái 90°, chỉ chấp nhận va chạm từ bên phải
                    isValidCollision = (correctedNormal.x < -0.98f);
                }
                else if (rotation == 90.0f)
                {
                    // Xoay phải 90°, chỉ chấp nhận va chạm từ bên trái
                    isValidCollision = (correctedNormal.x > 0.98f);
                }

                if (isValidCollision)
                {
                    AXLOG("🟢 Kích hoạt Trampoline!");
                    static_cast<Trampoline*>(trap)->activateTrap();
                    static_cast<Trampoline*>(trap)->dealDamage(passiveNode);
                }
            }
                break;
            case (static_cast<int>(TrapType::Fire)):
            {
                if (correctedNormal.y > 0.98f)
                {
                    AXLOG("🔥 Va chạm tại MẶT TRÊN của bẫy Fire!");
                    static_cast<Fire*>(trap)->activateTrap();
                    static_cast<Fire*>(trap)->dealDamage(passiveNode);
                }
            }
                break;
            case (static_cast<int>(TrapType::Fan)):
                static_cast<Fan*>(trap)->activateTrap();
                static_cast<Fan*>(trap)->dealDamage(passiveNode);
                break;
            default:
                trap->dealDamage(passiveNode);
                break;
            }
        }
    }

    Node* affectZoneNode = nullptr;
    if (bodyA->getCategoryBitmask() == 0x04)
    {
        AXLOG("Đi vào vùng ảnh hưởng");
        affectZoneNode = bodyA->getNode();
    }
    else if (bodyB->getCategoryBitmask() == 0x04)
    {
        AXLOG("Đi vào vùng ảnh hưởng");
        affectZoneNode = bodyB->getNode();
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
