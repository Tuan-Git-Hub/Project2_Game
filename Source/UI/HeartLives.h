#ifndef __HEART_LIVES_H__
#define __HEART_LIVES_H__

#include "axmol.h"

class HeartLives : public ax::Node
{
private:
    ax::Vector<ax::Sprite*> vt_hearts;
    int quantity = 3;
public:
    static HeartLives* createHeartLives();
    bool init();
    void addAHeart();
    void deleteAHeart();
};


#endif  // __HEART_LIVES_H__