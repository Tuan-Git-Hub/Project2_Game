#ifndef __BOX_3_H__
#define __BOX_3_H__

#include "Box.h"

class Box3 : public Box
{
private:
    // Tên các ảnh đã được load trước để tạo init
    std::string name_textureBox3 = "Box3_Idle";
    const char* box3_Hit_PNG = "Box3_Hit_%d.png";
    int numberOf_Box3_Hit = 2;
    const char* box3_Break_PNG = "Box3_Break_%d.png";
public:
    Box3() : Box("Box3", 6)
    {
        nameTextureBox = name_textureBox3;
        boxHitPNG = box3_Hit_PNG;
        numberOfBoxHit = numberOf_Box3_Hit;
        boxBreakPNG = box3_Break_PNG;
    };

    static Box* createBox3()
    {
        Box *ret = new Box3();
        if (ret && ret->init())
        {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    };

    ~Box3() override {};
};

#endif // __BOX_2_H__