#include "CameraController.h"
#include "Utilities.h"
#include "GameManager.h"

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
    camera = Camera::createOrthographic(448, 252, 0.1f, 1000); // 640x360
    this->addChild(camera);
    this->setName("CameraPlayer");
    GameManager::getInstance().shakeCamera = [this]() { this->shake(10, 0.15); };
    this->scheduleUpdate();
    return true;
}

void CameraController::update(float dt) {
    if (target && !isShaking) {
        Vec2 targetPos = target->getPosition();
        camera->setPosition3D(Vec3(targetPos.x, targetPos.y, 200)); // Đặt camera theo tọa độ của target, Z = 500 để đảm bảo góc nhìn trên xuống.
        camera->lookAt(Vec3(targetPos.x, targetPos.y, 0)); // Định hướng camera để nó nhìn thẳng vào target
    }
}

void CameraController::shake(float intensity, float duration) {
    isShaking = true;
    float elapsed = 0.0f;
    Vec3 originalPos = camera->getPosition3D();

    this->schedule([=](float dt) mutable { // mutable để cho phép chỉnh sửa giá tri thuộc tính copy bên trong (elapsed)
        elapsed += dt;
        if (elapsed >= duration) {
            // Hết thời gian rung, trả về vị trí gốc và hủy schedule
            camera->setPosition3D(originalPos);
            this->unschedule("shake_loop");
            isShaking = false;
            return;
        }

        float shakeX = (rand_0_1() * 2 - 1) * intensity;
        float shakeY = (rand_0_1() * 2 - 1) * intensity;
        camera->setPosition3D(originalPos + Vec3(shakeX, shakeY, 0));
    }, 1/60, "shake_loop");


    // this->scheduleOnce([=](float) {
    //     camera->setPosition3D(originalPos);
    // }, duration, "stop_shake");
}