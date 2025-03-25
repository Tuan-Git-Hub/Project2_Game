#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "axmol.h"
#include <unordered_map>

class SpriteManager
{
public:
    static SpriteManager& getInstance(); // Lấy singleton để đảm bảo các ảnh texture được load 1 lần, danh sách đã load là duy nhất
    void loadTexture(const std::string& name, const char* filePath); // Load ảnh image vào texture
    ax::Sprite* createSprite(const std::string& name); // Tạo Sprite từ texture
    void loadSpriteFrame(const char* filePath); // Load file vào SpriteFrameCache
    ax::Vector<ax::SpriteFrame*> createVectorSpriteFrame(const char* filePath, const char *format, int count); // Tạo ra 1 container chứa các SpriteFrame
private:
    std::unordered_map<std::string, ax::Texture2D*> texturesCache; // Lưu trữ texture
};


#endif  // __SPRITE_MANAGER_H__