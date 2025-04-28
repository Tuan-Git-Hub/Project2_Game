#include "MapManager.h"
#include "Utilities.h"
#include "GameObjectManager.h"

using namespace ax;

// Tạo physic body cho các layer
void MapManager::addPhysicBodyToTileMap(TMXTiledMap* map)
{
    auto allLayers = map->getLayers(); // Lấy các Layer
    if (allLayers.empty())
        return;
    for (const auto& obLayer : allLayers)
    {    
        auto nameLayer = obLayer->getLayerName();
        if (nameLayer == "Ground" || nameLayer == "Sand" || nameLayer == "Mud" || nameLayer == "Ice")
        {
            // Gộp khối trước
            std::vector<std::pair<Vec2, Vec2>> cP;
            for (int row = 0; row < obLayer->getLayerSize().height; row++)
            {
                std::pair<Vec2, Vec2> po = {Vec2(-1, -1), Vec2(-1, -1)};
                for (int col = 0; col < obLayer->getLayerSize().width; col++)
                {
                    // Lấy thông tin Tile tại vị trí (col, row)
                    auto tile = obLayer->getTileAt(Vec2(col, row)); // Lấy tile
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

            // Tạo physicbody cho khối đã gộp
            if (!cP.empty())
            {
                for (int i = 0; i < cP.size(); i++)
                {
                    Size tileSize = map->getTileSize(); // Lấy kích thước
                    Vec2 tilePo1 = obLayer->getPositionAt(cP[i].first); // Lấy vị trí tile, mặc định vị trí là góc dưới cùng bên trái của tile
                    Vec2 tilePo2 = obLayer->getPositionAt(cP[i].second);

                    // Tạo physicBody
                    auto widthBody = std::abs(tilePo1.x - tilePo2.x) + tileSize.x;
                    auto heightBody = std::abs(tilePo1.y - tilePo2.y) + tileSize.y;
                    if (nameLayer == "Mud")
                        heightBody = std::abs(tilePo1.y - tilePo2.y) + tileSize.y/2;

                    auto bodyGround = PhysicsBody::createBox(Size(widthBody, heightBody), PhysicsMaterial(1.0f, 0.0f, 0.2f));
                    if (nameLayer == "Ground")
                        bodyGround->setCategoryBitmask(ObjectBitmask::Ground);
                    else if (nameLayer == "Sand")
                        bodyGround->setCategoryBitmask(ObjectBitmask::Sand);
                    else if (nameLayer == "Mud")
                        bodyGround->setCategoryBitmask(ObjectBitmask::Mud);
                    else if (nameLayer == "Ice")
                        bodyGround->setCategoryBitmask(ObjectBitmask::Ice);
                    bodyGround->setContactTestBitmask(ObjectBitmask::Player); // Cần để lắng nghe va chạm với người chơi
                    bodyGround->setCollisionBitmask(0xFFFFFFFF); // Cho va chạm với tất cả
                    bodyGround->setDynamic(false);

                    // Tạo node gán
                    auto node = Node::create();
                    auto posX = std::min(tilePo1.x, tilePo2.x) + widthBody / 2;
                    auto posY = std::min(tilePo1.y, tilePo2.y) + heightBody / 2;
                    node->setPosition(Vec2(posX, posY));
                    node->setPhysicsBody(bodyGround);
                    map->addChild(node, 10);
                }
            }
        }
    }
}

void MapManager::createObjectsFromTMXTileMap(TMXTiledMap* map)
{
    auto allObjectGroups = map->getObjectGroups(); // Lấy các Object Layer
    for (const auto& obGroup : allObjectGroups)
    {
        auto nameGroup = obGroup->getGroupName();
        if (nameGroup == "Item" || nameGroup == "Trap")
        {
            auto allObjects = obGroup->getObjects(); // Lấy các object trong group
            for (const auto& obj : allObjects)
            {
                // Lấy thông số object
                ValueMap objectInfo = obj.asValueMap();

                std::string name = objectInfo.count("name") ? objectInfo["name"].asString() : "None";
                float x = objectInfo["x"].asFloat();
                float y = objectInfo["y"].asFloat(); // kích thước y được axmol tự điều chỉnh = kích thước map(chiều cao) - tọa độ y(trong tilemap) - kích thước object(height) // nếu tilemap sử dụng insert tile
                float width = objectInfo.count("width") ? objectInfo["width"].asFloat() : 0.0f;
                float height = objectInfo.count("height") ? objectInfo["height"].asFloat() : 0.0f;
                float rotation = objectInfo.count("rotation") ? objectInfo["rotation"].asFloat() : 0.0f;

                // Vị trí đặt object cho phù hợp với góc xoay
                float delta_x = (width / 2) * cos(AX_DEGREES_TO_RADIANS(-rotation)) - (height / 2) * sin(AX_DEGREES_TO_RADIANS(-rotation));
                float delta_y = (width / 2) * sin(AX_DEGREES_TO_RADIANS(-rotation)) + (height / 2) * cos(AX_DEGREES_TO_RADIANS(-rotation));
                if (name == "Trampoline" || name == "Fire" || name == "Fan")
                {
                    delta_x = (width / 2) * cos(AX_DEGREES_TO_RADIANS(-rotation));
                    delta_y = (width / 2) * sin(AX_DEGREES_TO_RADIANS(-rotation));
                }
                float po_x = x  + delta_x;
                float po_y = (y + height) + delta_y; // Cộng thêm 1 height vì y ở trên thấp hơn

                // Phân loại
                ItemType item_Type = ItemType::None;
                TrapType trap_Type = TrapType::GenericTrap;

                if (name == "Box1")
                    item_Type = ItemType::Box1;
                else if (name == "Box2")
                    item_Type = ItemType::Box2;
                else if (name == "Box3")
                    item_Type = ItemType::Box3;
                else if (name == "Fruit")
                    item_Type = ItemType::Fruits;
                else if (name == "CheckpointFlag")
                    item_Type = ItemType::CheckpointFlag;
                else if (name == "Spike")
                    trap_Type = TrapType::Spike;
                else if (name == "Trampoline")
                    trap_Type = TrapType::Trampoline;
                else if (name.find("Saw") == 0)
                    trap_Type = TrapType::Saw;
                else if (name == "Fire")
                    trap_Type = TrapType::Fire;
                else if (name.find("RockHead") == 0)
                    trap_Type = TrapType::RockHead;
                else if (name == "Fan")
                    trap_Type = TrapType::Fan;
                 

                if (item_Type != ItemType::None)
                {
                    auto newItem = GameObjectManager::createItem(item_Type);
                    newItem->setPosition(po_x, po_y);
                    newItem->setRotation(rotation);
                    map->addChild(newItem);
                }
                else if (trap_Type != TrapType::GenericTrap)
                {
                    auto newTrap = GameObjectManager::createTrap(trap_Type);
                    newTrap->setPosition(po_x, po_y);
                    newTrap->setRotation(rotation);
                    newTrap->setName(name);
                    map->addChild(newTrap);
                }
                else
                    continue;
                AXLOG("position: %f, %f", po_x, po_y);
            }
        }
        else if (nameGroup == "Trigger")
        {
            auto allObjects = obGroup->getObjects();
            for (const auto& obj : allObjects)
            {
                // Lấy thông số object
                ValueMap objectInfo = obj.asValueMap();
                std::string name = objectInfo.count("name") ? objectInfo["name"].asString() : "None";
                float x = objectInfo["x"].asFloat();
                float y = objectInfo["y"].asFloat();
                AXLOG("trigger_position: %f, %f", x, y);
                float width = objectInfo.count("width") ? objectInfo["width"].asFloat() : 0.0f;
                float height = objectInfo.count("height") ? objectInfo["height"].asFloat() : 0.0f;

                float po_x = x  + (width / 2);
                float po_y = y + (height / 2);

                auto bodyGround = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(0.0f, 0.0f, 0.0f));
                bodyGround->setCategoryBitmask(ObjectBitmask::Trigger);
                bodyGround->setContactTestBitmask(ObjectBitmask::Player);
                bodyGround->setCollisionBitmask(0x00000000);
                bodyGround->setDynamic(false);

                // Tạo node gán
                auto node = Node::create();
                node->setPosition(Vec2(po_x, po_y));
                node->setPhysicsBody(bodyGround);
                if (name == "TurnOn")
                    node->setName("TurnOnHiddenPassage");
                else if (name == "TurnOff")
                    node->setName("TurnOffHiddenPassage");

                map->addChild(node);
            }
        }
    }
}

void MapManager::triggerHiddenMap(Node* node)
{
    auto map = static_cast<TMXTiledMap*>(node->getParent());
    if (node->getName() == "TurnOnHiddenPassage")
    {
        auto layerMap = map->getLayer("CoverLayer");
        if (layerMap && !layerMap->isVisible()) 
        {
            layerMap->setVisible(true);
            AXLOG("Show hidden passage!");
        }
    }
    else if (node->getName() == "TurnOffHiddenPassage")
    {
        auto layerMap = map->getLayer("CoverLayer");
        if (layerMap && layerMap->isVisible()) 
        {
            layerMap->setVisible(false);
            AXLOG("Hide hidden passage!");
        }
    }
}