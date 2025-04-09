#ifndef __HEART_LIVES_H__
#define __HEART_LIVES_H__

#include "axmol.h"

class HeartLives : public ax::Node
{
private:
    ax::Vector<ax::Sprite*> vt_hearts;
    int quantity = 3;

    static HeartLives* instance;
    HeartLives() {}; // Đảm bảo rằng chỉ có thể tạo ra một instance duy nhất
    HeartLives(const HeartLives&) = delete; // Ngăn sao chép
    HeartLives& operator=(const HeartLives&) = delete; // Ngăn gán object
public:
    static HeartLives* getInstance();
    bool init();
    void addAHeart();
    void deleteAHeart();

    static void deleteInstance();
};


#endif  // __HEART_LIVES_H__