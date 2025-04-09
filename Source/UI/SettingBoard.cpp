#include "SettingBoard.h"
#include "SpriteManager.h"
#include "DataSettingMusic.h"
#include "ui/UISlider.h"
#include "MobileButtons.h"
#include "MainMenuScene.h"
#define USE_AUDIO_ENGINE 1
#if USE_AUDIO_ENGINE
#    include "audio/AudioEngine.h"
#endif

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
    isMuteMusic = DataSettingMusic::getInstance().isMuteMusic();
    if (isMuteMusic)
        musicONOFF = SpriteManager::getInstance().createSprite("buttonOff");
    else
        musicONOFF = SpriteManager::getInstance().createSprite("buttonOn");
    auto button_toggleMusic = MenuItemSprite::create(musicONOFF, musicONOFF, AX_CALLBACK_0(SettingBoard::toggleMusic, this));
    button_toggleMusic->setScale(0.9f);

    isMuteSFX = DataSettingMusic::getInstance().isMuteSFX();
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
    slider->setPercent(DataSettingMusic::getInstance().getPercentVolume());

    // Xử lý sự kiện khi người chơi kéo slider
    slider->addEventListener([&](Object* sender, ui::Slider::EventType type) 
    {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
        {
            auto s = dynamic_cast<ui::Slider*>(sender);
            float v = s->getPercent() / 100.0f;  // Chuyển giá trị 0-100 thành 0.0-1.0
            //AudioEngine::setVolume(name, v);  // Đặt âm lượng (name là ID, tên đặt âm thanh)
            AXLOG("music percent: %d", s->getPercent());
            DataSettingMusic::getInstance().setPercentVolume(s->getPercent());
        }
    });
    settingBoard->addChild(slider);

    return true;
}

// Chuyển đổi music
void SettingBoard::toggleMusic()
{
    isMuteMusic = !isMuteMusic;
    DataSettingMusic::getInstance().toggleMusic();
    if(isMuteMusic)
    {
        AXLOG("Music Off");
        musicONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOff"));
        AudioEngine::pauseAll();
    }
    else
    {
        AXLOG("Music On");
        musicONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOn"));
        AudioEngine::resumeAll();
    }
}

// Chuyển đổi sfx
void SettingBoard::toggleSFX()
{
    isMuteSFX = !isMuteSFX;
    DataSettingMusic::getInstance().toggleSFX();
    if(isMuteSFX)
    {
        AXLOG("SFX Off");
        sfxONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOff"));
        AudioEngine::pauseAll();
    }
    else
    {
        AXLOG("SFX On");
        sfxONOFF->setTexture(SpriteManager::getInstance().getTextureByName("buttonOn"));
        AudioEngine::resumeAll();
    }  
}

// Hiển thị setting
void SettingBoard::onSetting()
{
    AXLOG("Pause Game");
    _director->pause();
    MobileButtons::getInstance()->setPosition(MobileButtons::getInstance()->getPosition() + Vec2(0, -1000));
    settingBoard->setVisible(true);
    buttonSettingInGame->setVisible(false);
}

// Quay lại trò chơi
void SettingBoard::onReturnScene()
{
    AXLOG("Resume Game");
    _director->resume();
    MobileButtons::getInstance()->setPosition(MobileButtons::getInstance()->getPosition() + Vec2(0, 1000));
    settingBoard->setVisible(false);
    buttonSettingInGame->setVisible(true);

}

// Restart game
void SettingBoard::onRestart()
{
    AXLOG("Restart Level");
}

// Exit
void SettingBoard::onReturnMainMenu()
{
    AXLOG("Return Main Menu");
    this->onReturnScene();
    auto mainMenuScene = utils::createInstance<MainMenuScene>();
    _director->replaceScene(mainMenuScene);
}