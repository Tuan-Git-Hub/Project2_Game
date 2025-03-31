#include "SpriteManager.h"
#include "Utilities.h"

using namespace ax;

SpriteManager& SpriteManager::getInstance()
{
    static SpriteManager instance;
    return instance;
}

void SpriteManager::loadTexture(const std::string& name, const char* filePath)
{
    auto texture = Director::getInstance()->getTextureCache()->addImage(filePath); // Lấy ảnh theo đường dẫn
    if (!texture)
    {
        Utilities::problemLoading(filePath); // Nếu có lỗi sẽ báo
    }
    else
    {
        texturesCache[name] = texture; // Lưu lại vào container unordered_map
    }
}

Sprite* SpriteManager::createSprite(const std::string& name)
{
    if (texturesCache.find(name) != texturesCache.end()) // hàm find() để tìm kiếm name và nếu có sẽ trả một iterator trỏ đến phần tử tìm thấy, nếu không nó sẽ là end()
    {
        return Sprite::createWithTexture(texturesCache[name]); // tạo sprite từ texture đã load
    }
    else
    {
        AXLOG("Error! Please load the texture before");
    }
    return nullptr;
}

Texture2D* SpriteManager::getTextureByName(const std::string& name)
{
    if (texturesCache.find(name) != texturesCache.end()) // hàm find() để tìm kiếm name và nếu có sẽ trả một iterator trỏ đến phần tử tìm thấy, nếu không nó sẽ là end()
    {
        return texturesCache[name]; // tạo sprite từ texture đã load
    }
    else
    {
        AXLOG("Error! Please load the texture before");
    }
    return nullptr;
}


void SpriteManager::loadSpriteFrame(const char* filePath)
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filePath);
}

Vector<SpriteFrame*> SpriteManager::createVectorSpriteFrame(const char* filePath, const char *format, int count)
{
    loadSpriteFrame(filePath);
    auto spriteCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> animFrames;
    char str[100];
    for (int i = 1; i <= count; i++)
    {
        snprintf(str, sizeof(str), format, i);
        SpriteFrame* frame = spriteCache->getSpriteFrameByName(str);
        if (frame)
            animFrames.pushBack(frame);
        else
            Utilities::problemLoading(str); // Gọi hàm báo lỗi nếu không load được ảnh
    }
    return animFrames;
}