#ifndef __ITEM_FACTORY_H__
#define __ITEM_FACTORY_H__

#include "Box1.h"
#include "Box2.h"
#include "Box3.h"
#include "Fruits.h"

class ItemFactory
{
public:
    static Item *createItem(ItemType type)
    {
        switch (type)
        {
            case ItemType::Box1:
                return Box1::createBox1();
            case ItemType::Box2:
                return Box2::createBox2();
            case ItemType::Box3:
                return Box3::createBox3();
            case ItemType::Fruits:
                return Fruits::createFruit();
            default:
                return nullptr;
        }
    };
};

#endif // __ITEM_FACTORY_H__