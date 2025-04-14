#include "MapManager.h"
#include "Utilities.h"
#include "GameObjectManager.h"

using namespace ax;

void MapManager::addPhysicBodyToTileMap(TMXTiledMap* map)
{
    // Tạo physic body cho map
    auto collisionObjectLayer = map->getLayer("Layer1");

    // Gộp khối trước
    std::vector<std::pair<Vec2, Vec2>> cP;
    if (collisionObjectLayer)
    {
        for (int row = 0; row < collisionObjectLayer->getLayerSize().height; row++)
        {
            std::pair<Vec2, Vec2> po = {Vec2(-1, -1), Vec2(-1, -1)};
            for (int col = 0; col < collisionObjectLayer->getLayerSize().width; col++)
            {
                // Lấy thông tin Tile tại vị trí (col, row)
                auto tile = collisionObjectLayer->getTileAt(Vec2(col, row)); // Lấy tile
                if (tile)
                {
                    // Gộp theo hàng
                    if (po.first.x == -1)
                    {
                        po.first = Vec2(col, row);
                        po.second = Vec2(col, row);
                    }
                    else
                    {
                        if (po.first.x + 1 == col || po.second.x + 1 == col)
                        {
                            po.second = Vec2(col, row); // mở rộng về bên phải
                        }
                        else
                        {
                            cP.push_back(po); // nếu khối đang xét không phải liền kề thì kết thúc 1 khối, đẩy vào danh sách và bắt đầu lại
                            po.first = Vec2(col, row);
                            po.second = Vec2(col, row);
                        }
                    }
                }
            }
            // Đẩy khối cuối cùng vào container
            if (po.first.x != -1)
            {
                cP.push_back(po);
            }

            // Gộp theo chiều dọc giữa các hàng (nếu vị trí và chiều ngang khớp)
            if (!cP.empty() && row != 0)
            {
                for (int i = 0; i < cP.size(); i++)
                {
                    // bỏ qua ô đã gộp
                    if (cP[i].first == Vec2(-1, -1)) 
                        continue;
                    for (int j = i + 1; j < cP.size(); j++)
                    {
                        // bỏ qua ô đã gộp
                        if (cP[i].first == Vec2(-1, -1)) 
                            continue;
                        if (cP[i].first.x == cP[j].first.x && cP[i].second.x == cP[j].second.x && cP[i].second.y + 1 == cP[j].first.y)
                        {
                            cP[i].second = cP[j].second; // merge khối và đánh dấu khối đã bị merge
                            cP[j].first = Vec2(-1, -1);
                            cP[j].second = Vec2(-1, -1);
                            break;
                        }
                    }
                }
                // Xóa khối đã đánh dấu
                for (auto it = cP.begin(); it != cP.end(); ) 
                {
                    if (*it == std::pair<Vec2, Vec2>{Vec2(-1, -1), Vec2(-1, -1)})
                        it = cP.erase(it);
                    else
                        ++it;
                }
            }
        }
    }

    // Tạo physicbody cho khối đã gộp
    if (!cP.empty())
    {
        for (int i = 0; i < cP.size(); i++)
        {
            Size tileSize = map->getTileSize(); // Lấy kích thước
            Vec2 tilePo1 = collisionObjectLayer->getPositionAt(cP[i].first); // Lấy vị trí tile, mặc định vị trí là góc dưới cùng bên trái của tile
            Vec2 tilePo2 = collisionObjectLayer->getPositionAt(cP[i].second);

            // Tạo physicBody
            auto widthBody = std::abs(tilePo1.x - tilePo2.x) + tileSize.x;
            auto heightBody = std::abs(tilePo1.y - tilePo2.y) + tileSize.y;
            auto bodyGround = PhysicsBody::createBox(Size(widthBody, heightBody), PhysicsMaterial(1.0f, 0.0f, 0.2f));
            bodyGround->setCategoryBitmask(ObjectBitmask::Ground);
            bodyGround->setContactTestBitmask(ObjectBitmask::Player); // Cần để lắng nghe va chạm với người chơi
            bodyGround->setCollisionBitmask(0xFFFFFFFF); // Cho va chạm với tất cả
            bodyGround->setDynamic(false);

            // Tạo node gán
            auto node = Node::create();
            auto posX = std::min(tilePo1.x, tilePo2.x) + widthBody / 2;
            auto posY = std::min(tilePo1.y, tilePo2.y) + heightBody / 2;
            node->setPosition(Vec2(posX, posY));
            node->setPhysicsBody(bodyGround);
            map->addChild(node);
        }
    }
}

// auto collisionObjectLayer = map->getLayer("Layer1");
    
//     if (collisionObjectLayer)
//     {
//         for (int row = 0; row < collisionObjectLayer->getLayerSize().height; row++)
//         {
//             for (int col = 0; col < collisionObjectLayer->getLayerSize().width; col++)
//             {
//                 // Lấy thông tin Tile tại vị trí (col, row)
//                 auto tile = collisionObjectLayer->getTileAt(Vec2(col, row)); // Lấy tile
//                 Vec2 tilePosition = collisionObjectLayer->getPositionAt(Vec2(col, row)); // Lấy vị trí tile
//                 Size tileSize = map->getTileSize(); // Lấy kích thước
//                 if (tile)
//                 {
//                     AXLOG("tile: %d, %d", col, row);
//                     AXLOG("tilePosition: %f, %f", tilePosition.x, tilePosition.y);
//                     auto bodyGround = PhysicsBody::createBox(tileSize, PhysicsMaterial(0.0f, 0.0f, 1.0f));
//                     bodyGround->setCategoryBitmask(ObjectBitmask::Ground);
//                     bodyGround->setContactTestBitmask(ObjectBitmask::Player); // Cần để lắng nghe va chạm với người chơi
//                     bodyGround->setCollisionBitmask(0xFFFFFFFF); // Cho va chạm với tất cả
//                     bodyGround->setDynamic(false);

//                     auto node = Node::create();
                    
//                     node->setPosition(tilePosition + Vec2(tileSize.width / 2, tileSize.height / 2));
//                     node->setPhysicsBody(bodyGround);
//                     node->setTag(1);
                    
//                     map->addChild(node);
//                 }
//             }
//         }
//     }
