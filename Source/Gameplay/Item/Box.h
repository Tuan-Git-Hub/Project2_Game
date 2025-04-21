#ifndef __BOX_H__
#define __BOX_H__

#include "Item.h"

class Box : public Item
{
protected:
    ax::Action *_idleAction;
    ax::Action *_hitAction;
    ax::Node* fragmentNode;

    std::string nameTextureBox;
    const char* boxHitPNG;
    int numberOfBoxHit;
    const char* boxBreakPNG;

    bool isHit = false;

public:
    Box(const std::string& name, int max_hp) : Item()
    {
        nameItem = name;
        MaxHP = max_hp;
        CurrentHP = max_hp;
        AXLOG("%s was created successfully", name.c_str());
    };

    bool init();
    void updateItem() override;
    ~Box() override;

    void idle();
    void hit();
    void breakBox();

};

#endif // __BOX_H__