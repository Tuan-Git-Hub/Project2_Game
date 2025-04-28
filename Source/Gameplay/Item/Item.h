#ifndef __ITEM_H__
#define __ITEM_H__

#include "axmol.h"

enum class ItemType
{
    None,
    Box1,
    Box2,
    Box3,
    Fruits,
    CheckpointFlag,
};

class Item : public ax::Sprite
{
protected:
    // Một số thuộc tích cơ bản
    // Tên
    std::string nameItem;
    // Số máu
    int MaxHP;
    int CurrentHP;

public:
    virtual const std::string& getNameItem() { return nameItem; };

    virtual int getMaxHPItem() const { return MaxHP; };
    virtual int getCurrentHPItem() const { return CurrentHP; };
    virtual void minusCurrentHPItem(int dame) { CurrentHP -= dame; }
    
    virtual void updateItem() {};
    virtual ~Item() {
        AXLOG("%s has been cancelled", nameItem.c_str());
    };

};

#endif // __ITEM_H__