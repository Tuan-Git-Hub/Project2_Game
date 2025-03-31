#include "CameraController.h"
#include "Utilities.h"

using namespace ax;

CameraController* CameraController::create(Node* target)
{
    CameraController *caco = new CameraController;
    if (caco && caco->init(target)) // Kiểm tra con trỏ 
    {
        caco->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return caco;
    }
    delete caco;
    return nullptr;
}

bool CameraController::init(Node* target)
{
    if (!Node::init())
        return false;
    this->target = target;
    camera = Camera::createOrthographic(640, 360, 0.1f, 1000);
    this->addChild(camera);
    this->scheduleUpdate();
    return true;
}

void CameraController::update(float dt) {
    if (target) {
        Vec2 targetPos = target->getPosition();
        camera->setPosition3D(Vec3(targetPos.x, targetPos.y, 200)); // Đặt camera theo tọa độ của target, Z = 500 để đảm bảo góc nhìn trên xuống.
        camera->lookAt(Vec3(targetPos.x, targetPos.y, 0)); // Định hướng camera để nó nhìn thẳng vào target
    }
}

void CameraController::shake(float intensity, float duration) {
    float shakeX = (rand_0_1() * 2 - 1) * intensity;
    float shakeY = (rand_0_1() * 2 - 1) * intensity;

    Vec3 originalPos = camera->getPosition3D();
    camera->setPosition3D(originalPos + Vec3(shakeX, shakeY, 0));

    this->scheduleOnce([=](float) {
        camera->setPosition3D(originalPos);
    }, duration, "stop_shake");
}