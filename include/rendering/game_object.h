/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "../../include/rendering/texture.h"
#include "../../include/rendering/sprite_renderer.h"
#include "../../include/rendering/game_settings.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    glm::vec2    Position, Size;
    glm::vec3    Color;
    float        Rotation;
    bool         Destroyed;
    Texture2D    Sprite; 
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f));
    //functions
    virtual void Draw(SpriteRenderer &renderer);
};

class MoveableObject : public GameObject
{
public:
    float        Velocity;
    int          Direction;
    int          MoveCount;
    std::vector<bool> CurrentCollision; //1 - right; 2 - left; 3 - up; 4 - down
    MoveableObject();
    MoveableObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f),
        float velocity = PLAYER_VELOCITY, std::vector<bool> CurrentCollision = {false, false, false, false},
        int Direction = 0, int MoveCount = 0);

    void MoveRandom(float velocity);
};


bool CheckCollision(GameObject &one, GameObject &two); // AABB - AABB collision
bool CheckCollisionUp(GameObject &one, GameObject &two); 
bool CheckCollisionDown(GameObject &one, GameObject &two); 
bool CheckCollisionRight(GameObject &one, GameObject &two); 
bool CheckCollisionLeft(GameObject &one, GameObject &two); 

#endif