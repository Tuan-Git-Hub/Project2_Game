#include "OptionsBoard.h"
#include "SpriteManager.h"
#include "MainMenuBoard.h"
#include "DataSettingMusic.h"
#include "ui/UISlider.h"
#include "SoundManager.h"

using namespace ax;

OptionsBoard* OptionsBoard::createOptionsBoard()
{

    OptionsBoard* ret = new OptionsBoard();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionsBoard::init()
{
    if (!Layer::init())
    return false;

    // Tạo 1 option board
    auto optionsboard = SpriteManager::getInstance().createSprite("optionsBoard");
    optionsboard->setPosition(Vec2(480, 180));
    this->addChild(optionsboard);

    // Tạo các nút bật tắt
    isMuteMusic = SoundManager::getdataMusic()->isMuteMusic();
    if (isMuteMusic)
        musicONOFF = SpriteManager::getInstance().createSprite("buttonOff");
    else
        musicONOFF = SpriteManager::getInstance().createSprite("buttonOn");
    auto button_toggleMusic = MenuItemSprite::create(musicONOFF, musicONOFF, AX_CALLBACK_0(OptionsBoard::toggleMusic, this));
    button_toggleMusic->setScale(0.9f);

    isMuteSFX = SoundManager::getdataMusic()->isMuteSFX();
    if (isMuteSFX)
        sfxONOFF = SpriteManager::getInstance().createSprite("buttonOff");
    else
        sfxONOFF = SpriteManager::getInstance().createSprite("buttonOn");
    auto button_toggleSFX = MenuItemSprite::create(sfxONOFF, sfxONOFF, AX_CALLBACK_0(OptionsBoard::toggleSFX, this));
    button_toggleSFX->setScale(0.9f);

    auto return_normal = SpriteManager::getInstance().createSprite("buttonReturn");
    auto return_selected = SpriteManager::getInstance().createSprite("buttonReturn");
    return_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_return = MenuItemSprite::create(return_normal, return_selected, AX_CALLBACK_0(OptionsBoard::onReturn, this));

    auto buttons = Menu::create(button_toggleMusic, button_toggleSFX, button_return, nullptr);
    button_toggleMusic->setPosition(Vec2(150, 127));
    button_toggleSFX->setPosition(Vec2(150, 97));
    button_return->setPosition(Vec2(10, 10));
    buttons->setPosition(Vec2::ZERO);
    optionsboard->addChild(buttons);

    // Tạo slider
    auto slider = ui::Slider::create();
    slider->loadBarTexture("res/Options/load_bar.png"); // thanh load
    slider->loadSlidBallTextures("res/Options/slide_ball.png"); // nút kéo

    slider->setScale(1.5f);
    slider->setPosition(Vec2(96, 28));
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
    optionsboard->addChild(slider);

    return true;
}

void OptionsBoard::toggleMusic()
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

void OptionsBoard::toggleSFX()
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

void OptionsBoard::onReturn()
{
    SoundManager::playEffect(AudioPaths::CLICK);
    AXLOG("Return MainMenu Board");
    this->setPosition(Vec2(1000, 1000));
    MainMenuBoard* mainMenuBoard = dynamic_cast<MainMenuBoard*>(this->getParent());
    mainMenuBoard->setPosition(Vec2::ZERO);
}