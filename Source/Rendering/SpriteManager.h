#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "axmol.h"
#include <unordered_map>

class SpriteManager
{
public:
    static SpriteManager& getInstance(); // Lấy singleton để đảm bảo các ảnh texture được load 1 lần, danh sách đã load là duy nhất
    void loadTextures(const std::vector<std::pair<std::string, const char*>>& textures); // Load ảnh image vào texture
    ax::Sprite* createSprite(const std::string& name); // Tạo Sprite từ texture
    ax::Texture2D* getTextureByName(const std::string& name); // Lấy texture trong container
    void loadSpriteFrames(const std::vector<const char*>& fPaths); // Load file vào SpriteFrameCache
    ax::Vector<ax::SpriteFrame*> createVectorSpriteFrame(const char *format, int count); // Tạo ra 1 container chứa các SpriteFrame
    ax::Sprite* getScreenshot(ax::Scene* scene); // Tạo sprite screen shot
private:
    SpriteManager() {}; //Đảm bảo rằng chỉ có thể tạo ra một instance duy nhất
    SpriteManager(const SpriteManager&) = delete; // Ngăn sao chép
    SpriteManager& operator=(const SpriteManager&) = delete; // Ngăn gán object
    std::unordered_map<std::string, ax::Texture2D*> texturesCache; // Lưu trữ texture
};


#endif  // __SPRITE_MANAGER_H__