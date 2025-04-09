#include "SpriteManager.h"
#include "Utilities.h"

using namespace ax;

SpriteManager& SpriteManager::getInstance()
{
    static SpriteManager instance;
    return instance;
}

void SpriteManager::loadTextures(const std::vector<std::pair<std::string, const char*>>& textures)
{
    auto texCache = Director::getInstance()->getTextureCache();
    for (const auto& [name, filePath] : textures)  // Lặp qua danh sách ảnh
    {
        // Kiểm tra đường dẫn hợp lệ
        if (!filePath || std::strlen(filePath) == 0) 
        {
            Utilities::problemLoading("(file: null)"); // Nếu có lỗi sẽ báo
            continue;
        }

        auto texture = texCache->addImage(filePath); // Load texture vào cache
        if (!texture)
        {
            Utilities::problemLoading(filePath); // Nếu có lỗi sẽ báo
        }
        else
        {
            texturesCache[name] = texture; // Lưu lại vào container unordered_map
        }
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
    if (texturesCache.find(name) != texturesCache.end())
    {
        return texturesCache[name];
    }
    else
    {
        AXLOG("Error! Please load the texture before");
    }
    return nullptr;
}


void SpriteManager::loadSpriteFrames(const std::vector<const char*>& fPaths)
{
    auto sfCache = SpriteFrameCache::getInstance();
    for (const char* filePath : fPaths)
    {
        // Kiểm tra đường dẫn hợp lệ
        if (!filePath || std::strlen(filePath) == 0) 
        {
            Utilities::problemLoading("(file: null)"); // Nếu có lỗi sẽ báo
        }
        else
            sfCache->addSpriteFramesWithFile(filePath);
    }
}

Vector<SpriteFrame*> SpriteManager::createVectorSpriteFrame(const char *format, int count)
{
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

Sprite* SpriteManager::getScreenshot(Scene* scene)
{
    auto winSize = Director::getInstance()->getWinSize();
    auto renderTexture = RenderTexture::create(winSize.width, winSize.height);
    renderTexture->begin();
    scene->visit();  // 'this' là Scene hiện tại
    renderTexture->end();

    // Đảm bảo hình được cập nhật ngay
    Director::getInstance()->getRenderer()->render();

    // Lấy sprite từ RenderTexture
    auto screenshotSprite = renderTexture->getSprite();
    return screenshotSprite;
}