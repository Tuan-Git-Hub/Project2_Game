#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "axmol.h"

class Player : public ax::Sprite
{
    enum class PlayerState
    {
        Idle,
        Run,
        Jump,
        Fall,
        DoubleJump,
        WallJump,
        Hit
    };

    private:
        PlayerState _currentState;
        ax::Action* _idleAction;
        ax::Action* _runAction;
        ax::Action* _jumpAction;
        ax::Action* _fallAction;
        ax::Action* _doublejumpAction;
        ax::Action* _walljumpAction;
        ax::Action* _hitAction;
        ax::PhysicsBody* _psbodyPlayer;

        int _hp = 3;
        ax::Vec2 _position = {100, 100}; // Vị trí ban đầu
        ax::Vec2 _velocity = {0, 0}; // vận tốc
        float _speedRun = 150.0f; // Tốc chạy cơ bản
        float _speedJump = 300.0f; // Tốc độ nhảy

        bool isOnGround = true;
        bool isOnSand = false;
        bool isOnMud = false;
        bool isOnIce = false;
        bool hasDoubleJump = true;
        bool isTouchLeftWall = false;
        bool isTouchRightWall = false;
        bool isHit = false;
        int _touchpoint = 0; // Số điểm va chạm
    public:

        static Player *create();
        bool init();
        ~Player();

        void setState(PlayerState state);
        void update(float dt);

        void idle();
        void moveLeft();
        void moveRight();
        void jump();
        void jump_while_on_wall();
        void fall();
        void double_jump();
        void wall_jump_Left();
        void wall_jump_Right();
        void hit();

        void onLeftKeyPressed();
        void onRightKeyPressed();
        void onJumpKeyPressed();
        void onKeyReleased();
        void checkBeforeCollision();

        void handleBeginCollisionWith(ax::Node* node, ax::PhysicsContact& contact);
        void handlePreSolveCollisionWith(ax::Node* node, ax::PhysicsContact& contact, ax::PhysicsContactPreSolve& solve);
        void handleSeparateCollisionWith(ax::Node* node, ax::PhysicsContact& contact);

        int getHP_Player() { return _hp; };
        void delete_A_HP();

};

#endif  // __PLAYER_H__