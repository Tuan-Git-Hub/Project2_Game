#ifndef __BOX_2_H__
#define __BOX_2_H__

#include "Box.h"

class Box2 : public Box
{
private:
    // Tên các ảnh đã được load trước để tạo init
    std::string name_textureBox2 = "Box2_Idle";
    const char* box2_Hit_PNG = "Box2_Hit_%d.png";
    int numberOf_Box2_Hit = 4;
    const char* box2_Break_PNG = "Box2_Break_%d.png";
public:
    Box2() : Box("Box2", 4)
    {
        nameTextureBox = name_textureBox2;
        boxHitPNG = box2_Hit_PNG;
        numberOfBoxHit = numberOf_Box2_Hit;
        boxBreakPNG = box2_Break_PNG;
    };

    static Box* createBox2()
    {
        Box *ret = new Box2();
        if (ret && ret->init())
        {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    };

    ~Box2() override {};
};

#endif // __BOX_2_H__