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
        // const char* fileImage_idle_player = "res/Player/Ninja Frog/Idle/Player_1_Idle.plist";
        // const char* fileImage_run_player = "res/Player/Ninja Frog/Run/Player_1_Run.plist";
        // const char* fileImage_jump_player = "res/Player/Ninja Frog/Player_1_Jump.png";
        // const char* fileImage_fall_player = "res/Player/Ninja Frog/Player_1_Fall.png";
        // const char* fileImage_doublejump_player = "res/Player/Ninja Frog/DoubleJump/Player_1_DoubleJump.plist";
        // const char* fileImage_walljump_player = "res/Player/Ninja Frog/WallJump/Player_1_WallJump.plist";
        // const char* fileImage_hit_player = "res/Player/Ninja Frog/Hit/Player_1_Hit.plist";

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
        bool hasDoubleJump = true;
        bool isTouchLeftWall = false;
        bool isTouchRightWall = false;
        int _touchpoint = 0; // Số điểm va chạm
    public:

        static Player *create();
        bool init();
        ~Player();

        void setState(PlayerState state);
        //PlayerState getState() const { return _currentState; };
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
        void handleBeginCollisionWith(ax::Node* node, ax::PhysicsContact& contact);
        void handleSeparateCollisionWith(ax::Node* node, ax::PhysicsContact& contact);

        int getHP_Player() { return _hp; };
        void delete_A_HP();

};

#endif  // __PLAYER_H__