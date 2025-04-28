#include "SettingBoard.h"
#include "SpriteManager.h"
#include "DataSettingMusic.h"
#include "ui/UISlider.h"
#include "MobileButtons.h"
#include "SceneManager.h"
#include "SoundManager.h"

using namespace ax;

SettingBoard* SettingBoard::createSettingBoard()
{

    SettingBoard* ret = new SettingBoard();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SettingBoard::init()
{
    if (!Layer::init())
    return false;

    // Tạo 1 nút bấm setting
    auto setting_normal = SpriteManager::getInstance().createSprite("buttonSetting");
    setting_normal->setOpacity(200);
    auto setting_selected = SpriteManager::getInstance().createSprite("buttonSetting");
    setting_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_setting = MenuItemSprite::create(setting_normal, setting_selected, AX_CALLBACK_0(SettingBoard::onSetting, this));
    buttonSettingInGame = Menu::create(button_setting, nullptr);;
    button_setting->setPosition(Vec2(610, 330));
    button_setting->setScale(1.5f);
    buttonSettingInGame->setPosition(Vec2::ZERO);
    this->addChild(buttonSettingInGame);

    // Tạo 1 setting board
    settingBoard = SpriteManager::getInstance().createSprite("settingBoard");
    settingBoard->setPosition(Vec2(320, 180));
    settingBoard->setVisible(false);
    this->addChild(settingBoard);

    // Tạo các nút bật tắt music, nút return, nút restart, nút exit
    isMuteMusic = SoundManager::getdataMusic()->isMuteMusic();
    if (isMuteMusic)
        musicONOFF = SpriteManager::getInstance().createSprite("buttonOff");
    else
        musicONOFF = SpriteManager::getInstance().createSprite("buttonOn");
    auto button_toggleMusic = MenuItemSprite::create(musicONOFF, musicONOFF, AX_CALLBACK_0(SettingBoard::toggleMusic, this));
    button_toggleMusic->setScale(0.9f);

    isMuteSFX = SoundManager::getdataMusic()->isMuteSFX();
    if (isMuteSFX)
        sfxONOFF = SpriteManager::getInstance().createSprite("buttonOff");
    else
        sfxONOFF = SpriteManager::getInstance().createSprite("buttonOn");
    auto button_toggleSFX = MenuItemSprite::create(sfxONOFF, sfxONOFF, AX_CALLBACK_0(SettingBoard::toggleSFX, this));
    button_toggleSFX->setScale(0.9f);

    auto return_normal = SpriteManager::getInstance().createSprite("buttonReturn");
    auto return_selected = SpriteManager::getInstance().createSprite("buttonReturn");
    return_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_return = MenuItemSprite::create(return_normal, return_selected, AX_CALLBACK_0(SettingBoard::onReturnScene, this));

    auto restart_normal = SpriteManager::getInstance().createSprite("buttonRestart");
    auto restart_selected = SpriteManager::getInstance().createSprite("buttonRestart");
    restart_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_restart = MenuItemSprite::create(restart_normal, restart_selected, AX_CALLBACK_0(SettingBoard::onRestart, this));

    auto exit_normal = SpriteManager::getInstance().createSprite("buttonExit2");
    auto exit_selected = SpriteManager::getInstance().createSprite("buttonExit2");
    exit_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_exit = MenuItemSprite::create(exit_normal, exit_selected, AX_CALLBACK_0(SettingBoard::onReturnMainMenu, this));

    auto buttons = Menu::create(button_toggleMusic, button_toggleSFX, button_return, button_restart, button_exit, nullptr);
    button_toggleMusic->setPosition(Vec2(150, 159));
    button_toggleSFX->setPosition(Vec2(150, 129));
    button_return->setPosition(Vec2(10, 10));
    button_restart->setPosition(Vec2(80, 10));
    button_exit->setPosition(Vec2(165, 10));
    buttons->setPosition(Vec2::ZERO);
    settingBoard->addChild(buttons);

    // Tạo slider
    auto slider = ui::Slider::create();
    slider->loadBarTexture("res/Options/load_bar.png"); // thanh load
    slider->loadSlidBallTextures("res/Options/slide_ball.png"); // nút kéo

    slider->setScale(1.5f);
    slider->setPosition(Vec2(96, 60));
    slider->setPercent(SoundManager::getdataMusic()->getPercentVolume());

    // Xử lý sự kiện khi người chơi kéo slider
    slider->addEventListener([&](Object* sender, ui::Slider::EventType type) 
    {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            auto s = dynamic_cast<ui::Slider*>(sender);
            float v = s->getPercent() / 100.0f;  // Chuyển giá trị 0-100 thành 0.0-1.0
            SoundManager::setVolume(v);
            AXLOG("music percent: %d", s->getPercent());
        }
    });
    settingBoard->addChild(slider);

    return true;
}

// Chuyển đổi music
void SettingBoard::toggleMusic()
{
    SoundManager::playEffect(AudioPaths::CLICK);
    isMuteMusic = !isMuteMusic;
    SoundManager::getdataMusic()->toggleMusic();
    if(isMuteMusic)
    {
        AXLOG("Music Off");
        musicONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOff"));
        SoundManager::pause_Music();
    }
    else
    {
        AXLOG("Music On");
        musicONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOn"));
        SoundManager::resume_Music();
    }
}

// Chuyển đổi sfx
void SettingBoard::toggleSFX()
{
    SoundManager::playEffect(AudioPaths::CLICK);
    isMuteSFX = !isMuteSFX;
    SoundManager::getdataMusic()->toggleSFX();
    if(isMuteSFX)
    {
        AXLOG("SFX Off");
        sfxONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOff"));
    }
    else
    {
        AXLOG("SFX On");
        sfxONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOn"));
    }  
}

// Hiển thị setting
void SettingBoard::onSetting()
{
    AXLOG("Pause Game");
    _director->pause(); // dừng scene, dừng update(), action
    this->getScene()->getPhysicsWorld()->setSpeed(0.0f); // dừng vật lý
    auto mobileButtons = this->getParent()->getChildByName("Mobile_Buttons");
    mobileButtons->setPosition(mobileButtons->getPosition() + Vec2(0, -1000));
    mobileButtons->pause();
    settingBoard->setVisible(true);
    buttonSettingInGame->setVisible(false);
    SoundManager::playEffect(AudioPaths::CLICK);
}

// Quay lại trò chơi
void SettingBoard::onReturnScene()
{
    AXLOG("Resume Game");
    _director->resume();
    this->getScene()->getPhysicsWorld()->setSpeed(1.0f);
    auto mobileButtons = this->getParent()->getChildByName("Mobile_Buttons");
    mobileButtons->setPosition(mobileButtons->getPosition() + Vec2(0, 1000));
    mobileButtons->resume();
    settingBoard->setVisible(false);
    buttonSettingInGame->setVisible(true);
    SoundManager::playEffect(AudioPaths::CLICK);

}

// Restart game
void SettingBoard::onRestart()
{
    AXLOG("Restart Level");
    SoundManager::playEffect(AudioPaths::CLICK);
    this->onReturnScene();
    SceneManager::restart_currentScene();   
}

// Exit
void SettingBoard::onReturnMainMenu()
{
    AXLOG("Return Main Menu");
    SoundManager::playEffect(AudioPaths::CLICK);
    this->onReturnScene();
    SceneManager::create_and_replace_currentScene(SceneType::MainMenu_Scene); 
}