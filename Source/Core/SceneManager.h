#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "axmol.h"

enum class SceneType
{
    None,
    MainMenu_Scene,
    GameOver_Scene,
    Level_1_Scene,
    Level_2_Scene,
    Level_3_Scene
};

class SceneManager
{
private:
    static SceneType _sceneType;
public:
    static SceneType getCurrentScene() { return _sceneType; }; // Lấy tên scene hiện tại
    static ax::Scene* createScene(SceneType type); // Tạo scene
    static void create_and_replace_currentScene(SceneType type); // Tạo và thay thế scene hiện tại
    static void restart_currentScene(); // restart scene hiện tại
};

#endif  // __SCENE_MANAGER_H__