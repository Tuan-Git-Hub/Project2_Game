#ifndef __CAMERA_CONTROLLER_H__
#define __CAMERA_CONTROLLER_H__

#include "axmol.h"

class CameraController : public ax::Node
{
public:
    static CameraController* create(ax::Node* target);
    bool init(ax::Node* target);
    void update(float dt);
    void shake(float intensity, float duration);

private:
    ax::Camera* camera;
    ax::Node* target;

};



#endif  //__CAMERA_CONTROLLER_H__