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


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec2    Position, Size, Velocity;
    glm::vec3    Color;
    float        Rotation;
    bool         IsSolid;
    bool         Destroyed;
    std::vector<bool> CurrentCollision; //1 - right; 2 - left; 3 - up; 4 - down
    // render state
    Texture2D   Sprite; 
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f),
        glm::vec2 velocity = glm::vec2(0.0f, 0.0f), std::vector<bool> CurrentCollision = {false, false, false, false});
    // draw sprite
    virtual void Draw(SpriteRenderer &renderer);
    void MoveRandom(float velocity);
};
bool CheckCollision(GameObject &one, GameObject &two); // AABB - AABB collision
bool CheckCollisionUp(GameObject &one, GameObject &two); 
bool CheckCollisionDown(GameObject &one, GameObject &two); 
bool CheckCollisionRight(GameObject &one, GameObject &two); 
bool CheckCollisionLeft(GameObject &one, GameObject &two); 
#endif