#ifndef __CHECKPOINT_FLAG_H__
#define __CHECKPOINT_FLAG_H__

#include "Item.h"

class CheckpointFlag : public Item
{
protected:
    ax::Action *_flagIdleAction;
    ax::Action* _flagAction;
public:
    CheckpointFlag() : Item()
    {
        nameItem = "CheckpointFlag";
        AXLOG("Checkpoint Flag was created successfully");
    }

    static CheckpointFlag* createCheckpoint();
    bool init();
    void flagOut();
    void updateItem() override;
    ~CheckpointFlag() override;
};

#endif // __CHECKPOINT_FLAG_H__