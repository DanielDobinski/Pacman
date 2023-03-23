#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H
#include <glm/glm.hpp>
// The Width of the screen
const unsigned int SCR_WIDTH = 800;
// The height of the screen
const unsigned int SCR_HEIGHT = 600;
// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(20.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(300.0f);
const float GHOST_VELOCITY(100.0f);

struct GameEvents_TAG
{
    bool _goThroughWalls;
    float _ghostsVelocity;
    float _pacmanVelocity;
};

extern struct GameEvents_TAG getGameEvents(void);

#endif
