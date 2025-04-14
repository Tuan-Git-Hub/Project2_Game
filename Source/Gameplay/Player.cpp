#include "Player.h"
#include "Utilities.h"
#include "SpriteManager.h"
#include "MobileButtons.h"
#include "GameManager.h"
#include "GameObjectManager.h"

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
    // Load các file ảnh vào cache
    // SpriteManager::getInstance().loadSpriteFrames({ fileImage_idle_player, fileImage_run_player, fileImage_doublejump_player, fileImage_walljump_player, fileImage_hit_player });
    // SpriteManager::getInstance().loadTextures({
    //     { "playerJump", fileImage_jump_player },
    //     { "playerFall", fileImage_fall_player } 
    // });
    // Lấy thông tin HP từ GameManager
    this->_hp = GameManager::getInstance().getNumberOfHearts();
    // Tạo hành động đứng yên Idle
    auto idleFrames = SpriteManager::getInstance().createVectorSpriteFrame("Player_1_Idle_%d.png", 11);
    this->initWithSpriteFrame(idleFrames.front());
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 1.0f/20);
    this->_idleAction =  RepeatForever::create(Animate::create(idleAnimation));
    _idleAction->retain(); // Lưu lại hành động, tránh phải tạo lần sau, vì dùng nhiều
    _currentState = PlayerState::Idle; // Đặt trạng thái ban đầu cơ bản là đứng yên
    this->runAction(_idleAction);

    // Tạo hành động di chuyển Run
    auto runFrames = SpriteManager::getInstance().createVectorSpriteFrame("Player_1_Run_%d.png", 12);
    auto runAnimation = Animation::createWithSpriteFrames(runFrames, 1.0f/20);
    this->_runAction = RepeatForever::create(Animate::create(runAnimation));
    _runAction->retain();

    // Tạo hành động nhảy Jump
    auto jumpFrames = SpriteFrame::createWithTexture(SpriteManager::getInstance().getTextureByName("playerJump"), Rect(0, 0, 32, 32));
    auto jumpAnimation = Animation::createWithSpriteFrames(Vector<SpriteFrame*> {jumpFrames}, 1.0f);
    this->_jumpAction = RepeatForever::create(Animate::create(jumpAnimation));
    _jumpAction->retain();

    // Tạo hành động Fall
    auto fallFrames = SpriteFrame::createWithTexture(SpriteManager::getInstance().getTextureByName("playerFall"), Rect(0, 0, 32, 32));
    auto fallAnimation = Animation::createWithSpriteFrames(Vector<SpriteFrame*> {fallFrames}, 1.0f);
    this->_fallAction = RepeatForever::create(Animate::create(fallAnimation));
    _fallAction->retain();

    // Tạo hành động double jumpe
    auto doubleJumpFrames = SpriteManager::getInstance().createVectorSpriteFrame("Player_1_DoubleJump_%d.png", 6);
    doubleJumpFrames.pushBack(fallFrames);
    auto doubleJumpAnimation = Animation::createWithSpriteFrames(doubleJumpFrames, 1.0f/24);
    this->_doublejumpAction = Sequence::create(
        Repeat::create(Animate::create(doubleJumpAnimation), 1),
        CallFunc::create([=] { _currentState = PlayerState::Jump; }),
        nullptr
    );
    _doublejumpAction->retain();

    // Tạo hành động wall jumpe
    auto wallJumpFrames = SpriteManager::getInstance().createVectorSpriteFrame("Player_1_WallJump_%d.png", 5);
    auto wallJumpAnimation = Animation::createWithSpriteFrames(wallJumpFrames, 1.0f/15);
    this->_walljumpAction = RepeatForever::create(Animate::create(wallJumpAnimation));
    _walljumpAction->retain();

    // Tạo hành động hit
    auto hitFrames = SpriteManager::getInstance().createVectorSpriteFrame("Player_1_Hit_%d.png", 7);
    auto hitAnimation = Animation::createWithSpriteFrames(hitFrames, 1.0f/20);
    this->_hitAction = Sequence::create(
        Repeat::create(Animate::create(hitAnimation), 3),
        CallFunc::create([=] { if (isOnGround) this->idle(); }),
        nullptr
    );
    _hitAction->retain();


    // Tạo physicsbody
    _psbodyPlayer = PhysicsBody::createBox(Size(this->getContentSize().x * 0.6f, this->getContentSize().y * 0.6f), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    _psbodyPlayer->setPositionOffset(Vec2(0, -5)); // Dời PhysicsBody
    _psbodyPlayer->setDynamic(true);
    _psbodyPlayer->setCategoryBitmask(ObjectBitmask::Player); // Gán bitmask
    _psbodyPlayer->setContactTestBitmask(0xFFFFFFFF); // Cần để lắng nghe kiểm tra va chạm với toàn bộ
    _psbodyPlayer->setCollisionBitmask(0xFFFFFFFF); // Cho va chạm với tất cả
    _psbodyPlayer->setAngularVelocity(0); // Vô hiệu hóa tốc độ xoay
    _psbodyPlayer->setRotationEnable(false); // Không cho phép vật thể xoay

    this->addComponent(_psbodyPlayer);
    this->setTag(2); // Gán tag để nhận diện Player

    // Gán các sự kiện
    // Chạm nút
    MobileButtons::leftMove = [this]() { this->onLeftKeyPressed(); };
    MobileButtons::rightMove = [this]() { this->onRightKeyPressed(); };
    MobileButtons::jumpMove = [this]() { this->onJumpKeyPressed(); };
    MobileButtons::stopMove = [this]() { this->onKeyReleased(); };
    // Thêm mạng HP
    GameManager::getInstance().addAHeartForPlayer = [this]() { this->_hp++; };

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
    _velocity.setZero();
    isOnGround = true;
    hasDoubleJump = true;
    setState(PlayerState::Idle);
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::moveLeft()
{
    setScaleX(-1);
    isTouchLeftWall = false;
    if(isOnGround)
    {
        _velocity.x = -_speedRun;
        setState(PlayerState::Run);
    }
    else
    {
        _velocity.x = -_speedRun/2;
    }
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::moveRight()
{
    setScaleX(1);
    isTouchRightWall = false;
    if(isOnGround)
    {
        _velocity.x = _speedRun;
        setState(PlayerState::Run);
    }
    else
    {
        _velocity.x = _speedRun/2;
    }
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::jump()
{
    _velocity.y = _speedJump;
    isOnGround = false;
    setState(PlayerState::Jump);
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::jump_while_on_wall()
{
    if (isTouchLeftWall)
        _velocity.x = -_speedRun;
    else if (isTouchRightWall)
        _velocity.x = _speedRun;
    this->jump();
}

void Player::fall()
{
    isOnGround = false;
    setState(PlayerState::Fall);
}

void Player::double_jump()
{
    _velocity.y = _speedJump;
    hasDoubleJump = false;
    setState(PlayerState::DoubleJump);
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::wall_jump_Left()
{
    isTouchLeftWall = true;
    isTouchRightWall = false;
    hasDoubleJump = true;
    setState(PlayerState::WallJump);
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::wall_jump_Right()
{
    isTouchRightWall = true;
    isTouchLeftWall = false;
    hasDoubleJump = true;
    setState(PlayerState::WallJump);
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::hit()
{
    _velocity.x = (this->getScaleX()) * -150.0f;
    _velocity.y = 200.0f;
    setState(PlayerState::Hit);
    _psbodyPlayer->setVelocity(_velocity);
}

// Các hàm kích hoạt khi nhấn nút
void Player::onLeftKeyPressed()
{
    if (_currentState != PlayerState::Hit && !isTouchRightWall)
    {
        AXLOG("Player move left");
        this->moveLeft();
    }
}

void Player::onRightKeyPressed()
{
    if (_currentState != PlayerState::Hit && !isTouchLeftWall)
    {
        AXLOG("Player move right");
        this->moveRight();
    }
}

void Player::onJumpKeyPressed()
{
    if (_currentState != PlayerState::Hit)
    {    
        if (isOnGround)
        {
            AXLOG("Player jump");
            this->jump();
        }
        else if (!isOnGround && hasDoubleJump && !isTouchLeftWall && !isTouchRightWall)
        {
            AXLOG("Player double jump");
            this->double_jump();
        }
        else if (_currentState == PlayerState::WallJump)
        {
            AXLOG("Player jump while on the wall");
            this->jump_while_on_wall();
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

void Player::handleBeginCollisionWith(Node* node, PhysicsContact& contact)
{
    auto contactData = contact.getContactData(); // Lấy dữ liệu
    _touchpoint += contactData->count; // cộng số điểm tiếp xúc
    auto norVector = contactData->normal; // Lấy vector pháp tuyến của va chạm
    auto pointContact = contactData->points[0]; // Lấy điểm va chạm
    AXLOG("contact count: %d", _touchpoint);
    AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
    AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
    
    //Lưu thông tin va chạm nếu trùng sẽ bỏ qua
    if (_contactDataPoint.first.isZero() && _contactDataPoint.second.isZero())
    {
        _contactDataPoint.first = norVector;
        _contactDataPoint.second = pointContact;
    }
    else
    {
        if (_contactDataPoint.first == norVector && _contactDataPoint.second == pointContact)
            return;
    }
    if (node->getPhysicsBody()->getCategoryBitmask() == ObjectBitmask::Ground)
    {
        _position = this->getPosition();
        if (node->getName() == "WallTest") // Dùng cho bản test scene 3
            norVector *= -1.0f;

        if (!isOnGround)
        {
            // Tính dot product (tích vô hướng)
            // Khi tính tích vô hướng 2 vector mà 2 vector có chiều dài là 1 thì sẽ ra cos(θ) góc giữa 2 vector
            if (std::abs(norVector.dot(Vec2(0, -1))) > 0.7f && _position.y - pointContact.y > 0) // Xem Lực hướng xuống hoặc lên với điểm va chạm để biết trạng thái player
            {
                AXLOG("Player is on ground");
                AXLOG("contact count: %d", _touchpoint);
                AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
                AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
                this->idle();
            }
            else if (std::abs(norVector.dot(Vec2(0, 1))) > 0.7f && _position.y - pointContact.y < 0)
            {
                AXLOG("Player is hit head");
                AXLOG("contact count: %d", _touchpoint);
                AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
                AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
                this->fall();
            }
            else if (std::abs(norVector.dot(Vec2(1, 0))) > 0.7f && _position.x - pointContact.x < 0)
            {
                AXLOG("Player is on left wall");
                AXLOG("contact count: %d", _touchpoint);
                AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
                AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
                this->wall_jump_Left();
            }
            else if (std::abs(norVector.dot(Vec2(-1, 0))) > 0.7f && _position.x - pointContact.x > 0)
            {
                AXLOG("Player is on right wall");
                AXLOG("contact count: %d", _touchpoint);
                AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
                AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
                this->wall_jump_Right();
            }
        }
        else
        {
            if (std::abs(norVector.dot(Vec2(1, 0))) > 0.7f && _position.x - pointContact.x < 0)
            {
                AXLOG("Player touch the left wall");
                AXLOG("contact count: %d", _touchpoint);
                AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
                AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
                isTouchLeftWall = true;
                this->idle();
            }
            else if (std::abs(norVector.dot(Vec2(-1, 0))) > 0.7f && _position.x - pointContact.x > 0)
            {
                AXLOG("Player touch the right wall");
                AXLOG("contact count: %d", _touchpoint);
                AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
                AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
                isTouchRightWall = true;
                this->idle();
            }
        }
    }
    if (node->getPhysicsBody()->getCategoryBitmask() == ObjectBitmask::SpikeTrap)
    {
        AXLOG("Player touch the spike trap");
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
        
        this->hit();
        this->delete_A_HP();
    }
}

void Player::handleSeparateCollisionWith(Node* node, PhysicsContact& contact)
{
    auto contactData = contact.getContactData(); // Lấy dữ liệu
    _touchpoint -= contactData->count; // trừ đi số điểm đã tiếp xúc
    auto norVector = contactData->normal; // Lấy vector pháp tuyến của va chạm
    auto pointContact = contactData->points[0]; // Lấy điểm va chạm

    AXLOG("contact count: %d", _touchpoint);   
    AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
    AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);

    if (_touchpoint == 0)
    {
        isOnGround = false;
        isTouchLeftWall = false;
        isTouchRightWall = false;
    }
    // Xóa đi thông tin va chạm vì đã tách ra
    _contactDataPoint.first.setZero();
    _contactDataPoint.second.setZero();
}

void Player::delete_A_HP()
{
    this->_hp--;
    GameManager::getInstance().minusAHeart();
}

// Hàm update
void Player::update(float dt)
{
    _velocity = _psbodyPlayer->getVelocity();
    if (_currentState != PlayerState::Hit)
    {    
        if (_currentState != PlayerState::WallJump && _velocity.y < -1)
        {
            this->fall();
        }
    }

    //AXLOG("position: %f, %f, %f", _position.x, _position.y, this->getPositionY());
    AXLOG("velocity: %f, %f", _velocity.x, _velocity.y);
    //AXLOG("velocity: %f, %f", this->getPhysicsBody()->getVelocity().x, this->getPhysicsBody()->getVelocity().y);
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