#include "Player.h"
#include "Utilities.h"
#include "SpriteManager.h"
#include "MobileButtons.h"

using namespace ax;

// Gọi hàm tạo đối tượng
Player* Player::create()
{
    Player *player = new Player;
    if (player && player->init()) // Kiểm tra con trỏ player có null ko và hàm init() có tạo thành công
    {
        player->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return player;
    }
    delete player;
    return nullptr;
}

// Hàm khởi tạo init
bool Player::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    // Tạo hành động đứng yên Idle
    auto idleFrames = SpriteManager::getInstance().createVectorSpriteFrame(fileImage_idle_player, "Player_1_Idle_%d.png", 11);
    this->initWithSpriteFrame(idleFrames.front());
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 1.0f/20);
    this->_idleAction =  RepeatForever::create(Animate::create(idleAnimation));
    _idleAction->retain(); // Lưu lại hành động, tránh phải tạo lần sau, vì dùng nhiều
    _currentState = PlayerState::Idle; // Đặt trạng thái ban đầu cơ bản là đứng yên
    this->runAction(_idleAction);

    // Tạo hành động di chuyển Run
    auto runFrames = SpriteManager::getInstance().createVectorSpriteFrame(fileImage_run_player, "Player_1_Run_%d.png", 12);
    auto runAnimation = Animation::createWithSpriteFrames(runFrames, 1.0f/20);
    this->_runAction = RepeatForever::create(Animate::create(runAnimation));
    _runAction->retain();

    // Tạo hành động nhảy Jump
    auto jumpFrames = SpriteFrame::create(fileImage_jump_player, Rect(0, 0, 32, 32));
    if (!jumpFrames)
    {
        Utilities::problemLoading(fileImage_jump_player);
        return false;
    }
    auto jumpAnimation = Animation::createWithSpriteFrames(Vector<SpriteFrame*> {jumpFrames}, 1.0f);
    this->_jumpAction = RepeatForever::create(Animate::create(jumpAnimation));
    _jumpAction->retain();

    // Tạo hành động Fall
    auto fallFrames = SpriteFrame::create(fileImage_fall_player, Rect(0, 0, 32, 32));
    if (!fallFrames)
    {
        Utilities::problemLoading(fileImage_fall_player);
        return false;
    }
    auto fallAnimation = Animation::createWithSpriteFrames(Vector<SpriteFrame*> {fallFrames}, 1.0f);
    this->_fallAction = RepeatForever::create(Animate::create(fallAnimation));
    _fallAction->retain();

    // Tạo hành động double jumpe
    auto doubleJumpFrames = SpriteManager::getInstance().createVectorSpriteFrame(fileImage_doublejump_player, "Player_1_DoubleJump_%d.png", 6);
    doubleJumpFrames.pushBack(fallFrames);
    auto doubleJumpAnimation = Animation::createWithSpriteFrames(doubleJumpFrames, 1.0f/24);
    this->_doublejumpAction = Sequence::create(
        Repeat::create(Animate::create(doubleJumpAnimation), 1),
        CallFunc::create([=] { _currentState = PlayerState::Jump; }),
        nullptr
    );
    _doublejumpAction->retain();

    // Tạo hành động wall jumpe
    auto wallJumpFrames = SpriteManager::getInstance().createVectorSpriteFrame(fileImage_walljump_player, "Player_1_WallJump_%d.png", 5);
    auto wallJumpAnimation = Animation::createWithSpriteFrames(wallJumpFrames, 1.0f/15);
    this->_walljumpAction = RepeatForever::create(Animate::create(wallJumpAnimation));
    _walljumpAction->retain();

    // Tạo hành động hit
    auto hitFrames = SpriteManager::getInstance().createVectorSpriteFrame(fileImage_hit_player, "Player_1_Hit_%d.png", 7);
    auto hitAnimation = Animation::createWithSpriteFrames(hitFrames, 1.0f/14);
    this->_hitAction = Sequence::create(
        Repeat::create(Animate::create(hitAnimation), 3),
        CallFunc::create([=] { _currentState = PlayerState::Idle; }),
        nullptr
    );
    _hitAction->retain();


    // Tạo physicsbody
    auto bodyPlayer = PhysicsBody::createBox(Size(this->getContentSize().x * 0.6f, this->getContentSize().y * 0.6f), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    bodyPlayer->setPositionOffset(Vec2(0, -5)); // Dời PhysicsBody
    bodyPlayer->setDynamic(true);
    bodyPlayer->setContactTestBitmask(0xFFFFFFFF); // Cần để lắng nghe kiểm tra va chạm
    bodyPlayer->setAngularVelocity(0); // Vô hiệu hóa tốc độ xoay
    bodyPlayer->setRotationEnable(false); // Không cho phép vật thể xoay

    this->addComponent(bodyPlayer);
    this->setTag(2); // Gán tag để nhận diện Player trong va chạm

    // Gán sự kiện chạm
    MobileButtons::leftMove = [this]() { this->onLeftKeyPressed(); };
    MobileButtons::rightMove = [this]() { this->onRightKeyPressed(); };
    MobileButtons::jumpMove = [this]() { this->onJumpKeyPressed(); };
    MobileButtons::stopMove = [this]() { this->onKeyReleased(); };

    // Tạo sự kiện listener cho riêng player
    auto contactListenerPlayer = EventListenerPhysicsContact::create();
    contactListenerPlayer->onContactBegin = AX_CALLBACK_1(Player::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListenerPlayer, this);

    scheduleUpdate();
    return true;
}

// Hàm đặt trạng thái và cập nhật trạng thái của nhân vật
void Player::setState(PlayerState state)
{
    if (_currentState != state)
    {
        _currentState = state;
        this->stopAllActions();

        switch (state)
        {
            case PlayerState::Idle:
                this->runAction(_idleAction);
                break;
            case PlayerState::Run:
                this->runAction(_runAction);
                break;
            case PlayerState::Jump:
                this->runAction(_jumpAction);
                break;
            case PlayerState::Fall:
                this->runAction(_fallAction);
                break;
            case PlayerState::DoubleJump:
                this->runAction(_doublejumpAction);
                break;
            case PlayerState::WallJump:
                this->runAction(_walljumpAction);
                break;
            case PlayerState::Hit:
                this->runAction(_hitAction);
                break;
        }
    }
}

// Hàm gọi action
void Player::idle()
{
    _velocity = {0, 0};
    isOnGround = true;
    hasDoubleJump = true;
    isOnLeftWall = false;
    isOnRightWall = false;
    setState(PlayerState::Idle);
}

void Player::moveLeft()
{
    setState(PlayerState::Run);
}

void Player::moveRight()
{
    setState(PlayerState::Run);
}

void Player::jump()
{
    isOnGround = false;
    setState(PlayerState::Jump);
}

void Player::fall()
{
    isOnGround = false;
    setState(PlayerState::Fall);
}

void Player::double_jump()
{
    hasDoubleJump = false;
    setState(PlayerState::DoubleJump);
}

void Player::wall_jump_Left()
{
    isOnLeftWall = true;
    isOnRightWall = false;
    hasDoubleJump = true;
    setState(PlayerState::WallJump);
}

void Player::wall_jump_Right()
{
    isOnRightWall = true;
    isOnLeftWall = false;
    hasDoubleJump = true;
    setState(PlayerState::WallJump);
}

void Player::hit()
{
    _hp -= 1;
    _velocity = {-50, 0};
    setState(PlayerState::Hit);
}

// Các hàm kích hoạt khi nhấn nút
void Player::onLeftKeyPressed()
{
    if (_currentState != PlayerState::Hit)
    {
        setScaleX(-1);
        _velocity.x = -_speedRun;
        if (isOnGround)
        {
            this->moveLeft();
        }
    }
}

void Player::onRightKeyPressed()
{
    if (_currentState != PlayerState::Hit)
    {
        setScaleX(1);
        _velocity.x = _speedRun;
        if (isOnGround)
        {
            this->moveRight();
        }
    }
}

void Player::onJumpKeyPressed()
{
    if (_currentState != PlayerState::Hit)
    {    
        if (isOnGround)
        {
            _velocity.y = _speedJump;
            this->jump();
        }
        else if (!isOnGround && hasDoubleJump)
        {
            this->getPhysicsBody()->setVelocity(Vec2(0, 0));
            this->double_jump();
        }
        else if (isOnLeftWall)
        {
            _velocity = { -_speedRun, _speedJump };
            this->jump();
        }
        else if (isOnRightWall)
        {
            _velocity = { _speedRun, _speedJump };
            this->jump();
        }
    }
}

void Player::onKeyReleased()
{
    if (_currentState != PlayerState::Hit)
    {
        if (isOnGround)
            this->idle(); 
    }
}

bool Player::onContactBegin(PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    // Xác định Player trong va chạm
    Node* playerNode = (nodeA->getTag() == 2) ? nodeA : nodeB;
    Node* otherNode = (playerNode == nodeA) ? nodeB : nodeA;

    if (otherNode->getTag() == 1 && _currentState != PlayerState::Run)
    {
        AXLOG("Player is on ground");
        this->idle();
    }
    return true;
}

// Hàm update
void Player::update(float dt)
{
    float _lastPositionY = _position.y; // vị trí trước đó
    _position = this->getPosition();
    _position += _velocity * dt;

    if (_currentState != PlayerState::Hit)
    {    
        Vec2 _prePosition = _position;
        if (_currentState == PlayerState::Jump && _lastPositionY > _position.y)
        {
            this->fall();
        }
    }
    this->setPosition(_position);
    //AXLOG("position: %f, %f, %f", _position.x, _position.y, this->getPositionY());
    //AXLOG("velocity: %f, %f", _velocity.x, _velocity.y);
    AXLOG("velocity: %f, %f", this->getPhysicsBody()->getVelocity().x, this->getPhysicsBody()->getVelocity().y);
    //AXLOG("velocity: %f", this->getPhysicsBody()->getVelocityAtLocalPoint(_position).y);
}


// Hàm hủy
Player::~Player()
{
    AX_SAFE_RELEASE(_idleAction); // Xóa hành động vì đã lưu vào bộ nhớ ở trên
    AX_SAFE_RELEASE(_runAction);
    AX_SAFE_RELEASE(_jumpAction);
    AX_SAFE_RELEASE(_fallAction);
    AX_SAFE_RELEASE(_doublejumpAction);
    AX_SAFE_RELEASE(_walljumpAction);
    AX_SAFE_RELEASE(_hitAction);
}