#ifndef __BOX_1_H__
#define __BOX_1_H__

#include "Box.h"

class Box1 : public Box
{
private:
    // Tên các ảnh đã được load trước để tạo init
    std::string name_textureBox1 = "Box1_Idle";
    const char* box1_Hit_PNG = "Box1_Hit_%d.png";
    int numberOf_Box1_Hit = 3;
    const char* box1_Break_PNG = "Box1_Break_%d.png";
public:
    Box1() : Box("Box1", 2)
    {
        nameTextureBox = name_textureBox1;
        boxHitPNG = box1_Hit_PNG;
        numberOfBoxHit = numberOf_Box1_Hit;
        boxBreakPNG = box1_Break_PNG;
    };

    static Box* createBox1()
    {
        Box *ret = new Box1();
        if (ret && ret->init())
        {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    };

    ~Box1() override {};
};

#endif // __BOX_1_H__