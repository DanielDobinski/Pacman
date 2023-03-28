
#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // level state
    std::vector<GameObject> Bricks;
    std::vector<GameObject> Food;
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer &renderer);

    std::vector<GameObject> getGameObject()
        {return Bricks;}
    int getFoodAmount()
        {return this->Food.size();}
    int getRemainingFood()
    {
        int amount = 0;
        for (GameObject &tile : this->Food)
        {
            if (!tile.Destroyed)
                amount++;
        }
        return amount;
    }


private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif