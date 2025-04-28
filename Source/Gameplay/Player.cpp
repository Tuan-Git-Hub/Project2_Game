#include "Player.h"
#include "Utilities.h"
#include "SpriteManager.h"
#include "MobileButtons.h"
#include "GameManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "CameraController.h"

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

    // Lấy thông tin HP từ GameManager
    this->_hp = GameManager::getInstance().getNumberOfHearts();
    // Tạo hành động đứng yên Idle
    auto idleFrames = SpriteManager::getInstance().createVectorSpriteFrame("Player_1_Idle_%d.png", 11);
    this->initWithSpriteFrame(idleFrames.front());
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 1.0f/10);
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
        Repeat::create(Animate::create(hitAnimation), 2),
        DelayTime::create(0.5f),
        CallFunc::create([=] { 
            if (_touchpoint) this->idle(); 
            else this->fall();
            isHit = false; }),
        nullptr
    );
    _hitAction->retain();

    // Tạo physicsbody this->getContentSize().x * 0.6f, this->getContentSize().y * 0.6f
    _psbodyPlayer = PhysicsBody::createBox(Size(Vec2(18, 26)), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    _psbodyPlayer->setPositionOffset(Vec2(0, -3)); // Dời PhysicsBody
    _psbodyPlayer->setDynamic(true);
    _psbodyPlayer->setCategoryBitmask(ObjectBitmask::Player); // Gán bitmask
    _psbodyPlayer->setContactTestBitmask(0xFFFFFFFF); // Cần để lắng nghe kiểm tra va chạm với toàn bộ
    _psbodyPlayer->setCollisionBitmask(0xFFFFFFFF); // Cho va chạm
    _psbodyPlayer->setAngularVelocity(0); // Vô hiệu hóa tốc độ xoay
    _psbodyPlayer->setRotationEnable(false); // Không cho phép vật thể xoay
    
    
    this->addComponent(_psbodyPlayer);
    

    // Gán các sự kiện
    // Chạm nút
    MobileButtons::leftMove = [this]() { this->onLeftKeyPressed(); };
    MobileButtons::rightMove = [this]() { this->onRightKeyPressed(); };
    MobileButtons::jumpMove = [this]() { this->onJumpKeyPressed(); };
    MobileButtons::stopMove = [this]() { this->onKeyReleased(); };
    // Thêm mạng HP
    GameManager::getInstance().addAHeartForPlayer = [this]() { this->_hp++; };

    scheduleUpdateWithPriority(2);
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
    _velocity = _psbodyPlayer->getVelocity();
    if (isOnIce)
        _velocity.y = 0.0f;
    else {
        SoundManager::playEffectWithCooldown(AudioPaths::PLAYER_RUN, 0.1f);
        _velocity.setZero();
    }
    _psbodyPlayer->setVelocity(_velocity);

    isOnGround = true;
    hasDoubleJump = true;
    setState(PlayerState::Idle);
}

void Player::moveLeft()
{
    setScaleX(-1);
    isTouchLeftWall = false;
    _velocity = _psbodyPlayer->getVelocity();
    if(isOnGround)
    {
        SoundManager::playEffectWithCooldown(AudioPaths::PLAYER_RUN, 0.3f);
        _velocity.x = -_speedRun;
        if (isOnIce)
            _velocity.x *= 1.3f;
        else if (isOnSand)
            _velocity.x *= 0.4f;
        else if (isOnMud)
            _velocity.x = 0.0f;
        setState(PlayerState::Run);
    }
    else
    {
        _velocity.x = -_speedRun;
    }
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::moveRight()
{
    setScaleX(1);
    isTouchRightWall = false;
    _velocity = _psbodyPlayer->getVelocity();
    if(isOnGround)
    {
        SoundManager::playEffectWithCooldown(AudioPaths::PLAYER_RUN, 0.3f);
        _velocity.x = _speedRun;
        if (isOnIce)
            _velocity.x *= 1.3f;
        else if (isOnSand)
            _velocity.x *= 0.4f;
        else if (isOnMud)
            _velocity.x = 0.0f;
        setState(PlayerState::Run);
    }
    else
    {
        _velocity.x = _speedRun;
    }
    _psbodyPlayer->setVelocity(_velocity);
}

void Player::jump()
{
    SoundManager::playEffectWithCooldown(AudioPaths::PLAYER_JUMP, 0.1f);
    _velocity = _psbodyPlayer->getVelocity();
    _velocity.y = _speedJump;
    _psbodyPlayer->setVelocity(_velocity);

    isOnGround = false;
    setState(PlayerState::Jump);
}

void Player::jump_while_on_wall()
{
    SoundManager::playEffectWithCooldown(AudioPaths::PLAYER_JUMP, 0.1f);
    _velocity = _psbodyPlayer->getVelocity();
    if (isTouchLeftWall)
    {
        setScaleX(-1);
        _velocity.x = -50.0f;
    }
    else if (isTouchRightWall)
    {
        setScaleX(1);
        _velocity.x = 50.0f;
    }
    _velocity.y = _speedJump;
    _psbodyPlayer->setVelocity(_velocity);

    setState(PlayerState::Jump);
}

void Player::fall()
{
    _velocity = _psbodyPlayer->getVelocity();
    isOnGround = false;
    setState(PlayerState::Fall);
}

void Player::double_jump()
{
    SoundManager::playEffectWithCooldown(AudioPaths::PLAYER_JUMP, 0.1f);
    _velocity = _psbodyPlayer->getVelocity();
    _velocity.y = _speedJump;
    _psbodyPlayer->setVelocity(_velocity);

    hasDoubleJump = false;
    setState(PlayerState::DoubleJump);
}

void Player::wall_jump_Left()
{
    isTouchLeftWall = true;
    isTouchRightWall = false;
    hasDoubleJump = true;
    setState(PlayerState::WallJump);
}

void Player::wall_jump_Right()
{
    isTouchRightWall = true;
    isTouchLeftWall = false;
    hasDoubleJump = true;
    setState(PlayerState::WallJump);
}

void Player::hit()
{
    SoundManager::playEffect(AudioPaths::PLAYER_HIT);
    _velocity = _psbodyPlayer->getVelocity();
    _velocity.x = (this->getScaleX()) * -50.0f;
    _velocity.y = 200.0f;
    _psbodyPlayer->setVelocity(_velocity);

    isHit = true;
    setState(PlayerState::Hit);
}

// Các hàm kích hoạt khi nhấn nút
void Player::onLeftKeyPressed()
{
    if (isHit || isTouchRightWall || isPush)
        return;
    AXLOG("Player move left");
    this->moveLeft();
}

void Player::onRightKeyPressed()
{
    if (isHit || isTouchLeftWall || isPush)
        return;
    AXLOG("Player move right");
    this->moveRight();
}

void Player::onJumpKeyPressed()
{
    if (isHit || isPush)
        return;
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

void Player::onKeyReleased()
{
    if (isHit || isPush)
        return;
    if (isOnGround)
    {
        this->idle(); 
    }
}

// Kiểm tra điều chỉnh trước va chạm
void Player::checkBeforeCollision()
{
    _position = this->getPosition();
    auto rayCastEnd = _position + Vec2(0, -25);

    this->getScene()->getPhysicsWorld()->rayCast(
        [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) {
            auto body = info.shape->getBody();
            if (body->getCategoryBitmask() == ObjectBitmask::Ground) 
            {
                _velocity = _psbodyPlayer->getVelocity();
                AXLOG("Player close to the ground");
                if (_velocity.y < -300) // Giảm tốc nếu quá nhanh
                {
                    _velocity.y *= 0.5f;
                    _psbodyPlayer->setVelocity(_velocity);
                }
            }
            return true;
        },
        _position,
        rayCastEnd,
        nullptr
    );
}

// Điều khiển khi va chạm bắt đầu
void Player::handleBeginCollisionWith(Node* node, PhysicsContact& contact)
{
    auto contactData = contact.getContactData(); // Lấy dữ liệu
    auto norVector = contactData->normal; // Lấy vector pháp tuyến của va chạm
    auto pointContact = contactData->points[0]; // Lấy điểm va chạm
    _touchpoint += contactData->count; // cộng số điểm tiếp xúc
    AXLOG("contact count: %d", _touchpoint);
    // AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
    // AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);

    //Lưu thông tin va chạm nếu trùng điểm trong 1 thời gian ngắn sẽ bỏ qua
    static std::pair<ax::Vec2, ax::Vec2> _contactDataPoint = {Vec2::ZERO, Vec2::ZERO};
    static double timeContactBefore = 0;
    auto timeContactNow = utils::gettime();
    if ( _contactDataPoint.first.distance(norVector) < 0.1f && _contactDataPoint.second.distance(pointContact) < 0.1f && timeContactNow - timeContactBefore < 1)
        return;
    else
    {
        _contactDataPoint.first = norVector;
        _contactDataPoint.second = pointContact;
        timeContactBefore = timeContactNow;
    }
    // Nếu đang bị Hit thì bỏ qua
    if (isHit)
        return;

    // I. Va chạm với các loại vật thể tĩnh đặc biệt, có thể đứng và di chuyển trên đó
    auto categoryBitmask = node->getPhysicsBody()->getCategoryBitmask();
    if (categoryBitmask == ObjectBitmask::Ground || 
        categoryBitmask == ObjectBitmask::Sand || 
        categoryBitmask == ObjectBitmask::Mud || 
        categoryBitmask == ObjectBitmask::Ice ||
        categoryBitmask == ObjectBitmask::Box ||
        categoryBitmask == ObjectBitmask::FireTrap)
    {
        _position = this->getPosition();
        // 1. Xử lý chung
        if (!isOnGround)
        {
            // Tính dot product (tích vô hướng)
            // Khi tính tích vô hướng 2 vector mà 2 vector có chiều dài là 1 thì sẽ ra cos(θ) góc giữa 2 vector
            if (std::abs(norVector.dot(Vec2(0, -1))) > 0.7f && _position.y >= pointContact.y) // Xem Lực hướng xuống hoặc lên với điểm va chạm để biết trạng thái player
            {
                AXLOG("Player is on soil or box");
                if (categoryBitmask == ObjectBitmask::Ice)
                    isOnIce = true;
                this->idle();
            }
            else if (std::abs(norVector.dot(Vec2(0, 1))) > 0.7f && _position.y <= pointContact.y)
            {
                AXLOG("Player is hit head");
                this->fall();
                if (categoryBitmask == ObjectBitmask::Box)
                {
                    auto box = static_cast<Item*>(node);
                    box->updateItem();
                }
            }
            else if (std::abs(norVector.dot(Vec2(1, 0))) > 0.7f && _position.x <= pointContact.x)
            {
                AXLOG("Player is on left wall");
                this->wall_jump_Left();
            }
            else if (std::abs(norVector.dot(Vec2(-1, 0))) > 0.7f && _position.x >= pointContact.x)
            {
                AXLOG("Player is on right wall");
                this->wall_jump_Right();
            }
        }
        else
        {
            if (std::abs(norVector.dot(Vec2(1, 0))) > 0.7f && _position.x - pointContact.x < 0)
            {
                AXLOG("Player touch the left wall");
                isTouchLeftWall = true;
                this->idle();
            }
            else if (std::abs(norVector.dot(Vec2(-1, 0))) > 0.7f && _position.x - pointContact.x > 0)
            {
                AXLOG("Player touch the right wall");
                isTouchRightWall = true;
                this->idle();
            }
        }
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
        // 2. Xử lý riêng đối với từng đối tượng (nếu có)
        // Va chạm với Fire trap
        if (categoryBitmask == ObjectBitmask::FireTrap)
        {
            AXLOG("Player touch the fire trap");
            AXLOG("contact count: %d", _touchpoint);
            AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
            AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);

            auto rotationAngle = AX_DEGREES_TO_RADIANS(node->getRotation()); // Lấy góc xoay của object
            auto vecCheck = Vec2(0, -1).rotateByAngle(Vec2::ZERO, -rotationAngle); // Xoay vecto kiểm tra theo
            auto positionObject = node->getPosition();
            // AXLOG("poOB: %f, %f", positionObject.x, positionObject.y);
            if (std::abs(norVector.dot(vecCheck)) > 0.7f && positionObject.distance(pointContact) >= 13.0f) // Lấy phương va chạm và khoảng cách để xác định va chạm mặt nào
            {
                if (static_cast<Trap*>(node)->getActiveStatus())
                {
                    this->hit();
                    this->delete_A_HP();
                }
                else
                {
                    static_cast<Trap*>(node)->activateTrap();
                }
            }
        }
    }
    // II. Va chạm với các loại khác, thường là loại không đứng hay di chuyển trên đó
    // Va chạm với spike trap
    else if (categoryBitmask == ObjectBitmask::SpikeTrap)
    {
        AXLOG("Player touch the spike trap");
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
        
        this->hit();
        this->delete_A_HP();
    }
    // Va chạm với saw trap
    else if (categoryBitmask == ObjectBitmask::SawTrap)
    {
        AXLOG("Player touch the spike trap");
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
        
        this->hit();
        this->delete_A_HP();
    }
    // Va chạm với Trampoline
    else if (categoryBitmask == ObjectBitmask::Trampoline)
    {
        AXLOG("Player touch the trampoline trap");
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);

        this->stopAllActions();;
        this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Player_1_Idle_6.png"));
        isPush = true;
        _velocity = Vec2(0, 500);
        _velocity.rotate(Vec2::ZERO, AX_DEGREES_TO_RADIANS(node->getRotation() * -1)); // Xoay theo trampolinee
        _psbodyPlayer->setVelocity(_velocity);
        this->scheduleOnce([this](float) { isPush = false; }, 0.1f, "back_to_normal"); // Trì hoãn để không nhận hành động nào khác trong 1 thời gian
    }
    // Va chạm với win zone
    else if (categoryBitmask == ObjectBitmask::WinZone)
    {
        AXLOG("Player touch the win zone");
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
        isInWindZone = true;
        _nodeObject = node->getParent();
    }
    // III. Va chạm với các loại đặc biệt
    // Va chạm với RockHead
    else if (categoryBitmask == ObjectBitmask::RockHead)
    {
        AXLOG("Player touch the RockHead");
        AXLOG("contact count: %d", _touchpoint);
        AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
        AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);
        isFollowObject = true;
        if (!_nodeObject)
        {
            _nodeObject = node;
            po_Object = _nodeObject->getPosition();
        }
        if (!isOnGround)
        {
            if (std::abs(norVector.dot(Vec2(0, -1))) > 0.7f && _position.y >= pointContact.y)
                this->idle();
            else if (std::abs(norVector.dot(Vec2(0, 1))) > 0.7f && _position.y <= pointContact.y)
                this->fall();
            else if (std::abs(norVector.dot(Vec2(1, 0))) > 0.7f && _position.x <= pointContact.x)
                this->wall_jump_Left();
            else if (std::abs(norVector.dot(Vec2(-1, 0))) > 0.7f && _position.x >= pointContact.x)
                this->wall_jump_Right();
        }
        else
        {
            if (std::abs(norVector.dot(Vec2(1, 0))) > 0.7f && _position.x - pointContact.x < 0)
            {
                isTouchLeftWall = true;
                _psbodyPlayer->setVelocity(Vec2::ZERO);
            }
            else if (std::abs(norVector.dot(Vec2(-1, 0))) > 0.7f && _position.x - pointContact.x > 0)
            {
                isTouchRightWall = true;
                _psbodyPlayer->setVelocity(Vec2::ZERO);
            }
        }
    }
}

// Điều khiển khi duy trì va chạm
void Player::handlePreSolveCollisionWith(Node* node, PhysicsContact& contact, PhysicsContactPreSolve& solve)
{
    // Nếu đang bị Hit thì bỏ qua
    if (isHit)
        return;

    auto categoryBitmask = node->getPhysicsBody()->getCategoryBitmask();
    // Nếu đang duy trì va chạm với spike trap thì tiếp tục mất máu
    if (categoryBitmask == ObjectBitmask::SpikeTrap)
    {
        AXLOG("Player continues to touch the spike trap");
        this->hit();
        this->delete_A_HP();
    }
    // Nếu tiếp tục chạm sand thì làm chậm
    else if (categoryBitmask == ObjectBitmask::Sand)
    {
        AXLOG("Player continues to touch the sand");
        isOnSand = true;
    }
    // Nếu tiếp tục chạm mud thì làm đứng im
    else if (categoryBitmask == ObjectBitmask::Mud)
    {
        AXLOG("Player continues to touch the mud");
        isOnMud = true;
    }
    // Nếu tiếp tục chạm ice thì tăng tốc
    else if (categoryBitmask == ObjectBitmask::Ice)
    {
        AXLOG("Player continues to touch the ice");
        isOnIce = true;
    }
    // Nếu tiếp tục chạm fire trap
    else if (categoryBitmask == ObjectBitmask::FireTrap)
    {
        AXLOG("Player continues to touch the fire trap");

        auto contactData = contact.getContactData(); // Lấy dữ liệu
        auto norVector = contactData->normal; // Lấy vector pháp tuyến của va chạm
        auto pointContact = contactData->points[0]; // Lấy điểm va chạm

        auto rotationAngle = AX_DEGREES_TO_RADIANS(node->getRotation()); // Lấy góc xoay của object
        auto vecCheck = Vec2(0, -1).rotateByAngle(Vec2::ZERO, -rotationAngle); // Xoay vecto kiểm tra theo
        auto positionObject = node->getPosition();
        if (std::abs(norVector.dot(vecCheck)) > 0.7f && positionObject.distance(pointContact) >= 13.0f) // Lấy phương va chạm và khoảng cách để xác định va chạm mặt nào
        {
            if (static_cast<Trap*>(node)->getActiveStatus())
            {
                this->hit();
                this->delete_A_HP();
            }
        }
    }
}

// Điều khiển khi va chạm tách ra
void Player::handleSeparateCollisionWith(Node* node, PhysicsContact& contact)
{
    auto contactData = contact.getContactData(); // Lấy dữ liệu
    _touchpoint -= contactData->count; // trừ đi số điểm đã tiếp xúc
    auto norVector = contactData->normal; // Lấy vector pháp tuyến của va chạm
    auto pointContact = contactData->points[0]; // Lấy điểm va chạm

    AXLOG("contact count: %d", _touchpoint);   
    AXLOG("contact normal: %f, %f", norVector.x, norVector.y);
    AXLOG("contact point: %f, %f", pointContact.x, pointContact.y);

    auto categoryBitmask = node->getPhysicsBody()->getCategoryBitmask();
    
    // Va chạm với win zone
    if (categoryBitmask == ObjectBitmask::WinZone)
    {
        isInWindZone = false;
        if (_nodeObject)
            _nodeObject = nullptr;
        if (isOnGround)
            this->idle();
    }
    

    // I. Thoát hoàn toàn không chạm vào vật thể nào
    if (_touchpoint == 0)
    {
        isOnGround = false;
        isTouchLeftWall = false;
        isTouchRightWall = false;
        isOnSand = false;
        isOnMud = false;
        isOnIce = false;
        isFollowObject = false;
        if (_nodeObject)
            _nodeObject = nullptr;
        return;
    }
    // II. Thoát va chạm nhưng vẫn đang duy trì va chạm với vật thể khác
    // Thoát Va chạm với các loại...
    if (categoryBitmask == ObjectBitmask::Sand)
    {
        isOnSand = false;
    }
    else if (categoryBitmask == ObjectBitmask::Mud)
    {
        isOnMud = false;
    }
    else if (categoryBitmask == ObjectBitmask::Ice)
    {
        isOnIce = false;
        _psbodyPlayer->setVelocity(Vec2(_psbodyPlayer->getVelocity().x * 0, _psbodyPlayer->getVelocity().y));
    }
    else if (categoryBitmask == ObjectBitmask::RockHead)
    {
        isFollowObject = false;
        isTouchLeftWall = false;
        isTouchRightWall = false;
        if (_nodeObject)
            _nodeObject = nullptr;
    }
}

void Player::delete_A_HP()
{
    this->_hp--;
    GameManager::getInstance().minusAHeart();
}

// Hàm update
void Player::update(float dt)
{
    _position = this->getPosition();
    // Chết nếu rơi quá sâu
    if (_position.y < -100.0f)
    {
        GameManager::getInstance().createSceneGameOver();
        return;
    }
    // Đang bị hit thì bỏ qua
    if (isHit)
    {
        AXLOG("velocity_hit: %f, %f", _psbodyPlayer->getVelocity().x, _psbodyPlayer->getVelocity().y);
        return;
    }  
    
    // Những trường hợp cần thay đổi
    if (!isTouchLeftWall && !isTouchRightWall && _psbodyPlayer->getVelocity().y < -1)
    {
        this->fall();
        this->checkBeforeCollision();
    }
    else if (_currentState == PlayerState::WallJump)
    {
        _velocity = _psbodyPlayer->getVelocity();
        _velocity.y = -_speedSlideWall;
        _psbodyPlayer->setVelocity(_velocity);
    }
    if (isFollowObject)
    {
        Vec2 po_ObjectAfter = _nodeObject->getPosition();
        auto _deltaDistance = po_ObjectAfter - po_Object;
        _position += _deltaDistance;
        this->setPosition(_position);
        po_Object = po_ObjectAfter;
    }
    if (isInWindZone)
    {
        _velocity = _psbodyPlayer->getVelocity();
        auto veloPush = Vec2(0, 200);
        veloPush.rotate(Vec2::ZERO, AX_DEGREES_TO_RADIANS(_nodeObject->getRotation() * -1)); // Xoay theo
        AXLOG("veloPush: %f, %f", veloPush.x, veloPush.y);
        _velocity.x = veloPush.x != 0 ? veloPush.x : _velocity.x;
        _velocity.y = veloPush.y != 0 ? veloPush.y : _velocity.y;
        _psbodyPlayer->setVelocity(_velocity);
    }


    AXLOG("isonground: %d", isOnGround);
    AXLOG("velocityI: %f, %f", _psbodyPlayer->getVelocity().x, _psbodyPlayer->getVelocity().y);
    //AXLOG("UDvelocity: %f, %f", _velocity.x, _velocity.y);
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