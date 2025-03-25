#include "MapManager.h"
#include "Utilities.h"

using namespace ax;

void MapManager::addPhysicBodyToTileMap(TMXTiledMap* map)
{
    // Tạo physic body cho map
    auto collisionObjectLayer = map->getLayer("Layer1");
    if (collisionObjectLayer)
    {
        for (int row = 0; row < collisionObjectLayer->getLayerSize().height; row++)
        {
            for (int col = 0; col < collisionObjectLayer->getLayerSize().width; col++)
            {
                // Lấy thông tin Tile tại vị trí (col, row)
                auto tile = collisionObjectLayer->getTileAt(Vec2(col, row)); // Lấy tile
                Vec2 tilePosition = collisionObjectLayer->getPositionAt(Vec2(col, row)); // Lấy vị trí tile
                Size tileSize = map->getTileSize(); // Lấy kích thước
                if (tile)
                {
                    auto bodyGround = PhysicsBody::createBox(tileSize, PhysicsMaterial(1.0f, 0.0f, 0.0f));
                    bodyGround->setContactTestBitmask(0xFFFFFFFF); // Cần để lắng nghe kiểm tra va chạm
                    bodyGround->setDynamic(false);

                    auto node = Node::create();
                    node->setPosition(tilePosition + Vec2(tileSize.width / 2, tileSize.height / 2) + Vec2(map->getPosition().x , map->getPosition().y));
                    node->setPhysicsBody(bodyGround);
                    node->setTag(1);

                    map->addChild(node, 0);
                }
            }
        }
    }
}