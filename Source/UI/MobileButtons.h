#ifndef __MOBILE_BUTTONS_H__
#define __MOBILE_BUTTONS_H__

#include "axmol.h"

class MobileButtons : public ax::Node
{
private:
    static MobileButtons* instance;
    MobileButtons() {}; // Đảm bảo rằng chỉ có thể tạo ra một instance duy nhất
    MobileButtons(const MobileButtons&) = delete; // Ngăn sao chép
    MobileButtons& operator=(const MobileButtons&) = delete; // Ngăn gán object

public:
    static MobileButtons* getInstance();
    bool init();
    
    std::function<void()> leftMove;
    std::function<void()> rightMove;
    std::function<void()> jumpMove;
    std::function<void()> stopMove;

    bool isHoldingMoveLeft = false;
    bool isHoldingMoveRight = false;
    bool isHoldingJump = false;

    void update(float dt);
    static void deleteInstance();
};


#endif  // __MOBILE_BUTTONS_H__