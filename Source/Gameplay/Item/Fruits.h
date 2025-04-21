#ifndef __FRUITS_H__
#define __FRUITS_H__

#include "Item.h"

class Fruits : public Item
{
protected:
    ax::Action *_idleAction;
    ax::Action *_collectedAction;

    const char* fruits[8] = {
        "Apple_%d.png",
        "Bananas_%d.png",
        "Cherries_%d.png",
        "Kiwi_%d.png",
        "Melon_%d.png",
        "Orange_%d.png",
        "Pineapple_%d.png",
        "Strawberry_%d.png"
    };
    const char* fruitsIdlePNG = "Apple_%d.png";
    const char* fruitsCollectedPNG = "Fruits_Collected_%d.png";

public:
    Fruits() : Item()
    {
        nameItem = "Fruit";
        MaxHP = 1;
        CurrentHP = 1;
        AXLOG("Fruit was created successfully");
    };

    static Fruits* createFruit();
    bool init();
    void updateItem() override;
    ~Fruits() override;

    void collected();
};

#endif // __FRUITS_H__