/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "../../include/rendering/game_object.h"


GameObject::GameObject() 
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, std::vector<bool> CurrentCollision) 
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false), CurrentCollision({false, false, false, false}) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
//one is movable, two is brick
bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}  
//one is movable, two is brick
bool CheckCollisionRight(GameObject &one, GameObject &two) 
{
    bool collisionX = false;
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    if (collisionY == false)
        return false;
    if(one.Position.x + one.Size.x + SMALL_PIXEL_NUMBER >= two.Position.x)
        collisionX = true;
    if (one.Position.x > two.Position.x)
        return false;
    return collisionX;
}  
//one is movable, two is brick
bool CheckCollisionLeft(GameObject &one, GameObject &two) 
{
    bool collisionX = false;
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    if (collisionY == false)
        return false;
    if(two.Position.x + two.Size.x + SMALL_PIXEL_NUMBER >= one.Position.x)
        collisionX = true;
    if (one.Position.x < two.Position.x)
        return false;
    return collisionX;
}  
//one is movable, two is brick
bool CheckCollisionUp(GameObject &one, GameObject &two) 
{
    bool collisionY = false;
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    if (collisionX == false)
        return false;
    if (two.Position.y + two.Size.y + SMALL_PIXEL_NUMBER >= one.Position.y)
        collisionY = true;
    if (one.Position.y < two.Position.y)
        return false;
    return collisionY;
}  
//one is movable, two is brick
bool CheckCollisionDown(GameObject &one, GameObject &two) 
{
    bool collisionY = false;
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    if (collisionX == false)
        return false;
    if (one.Position.y + one.Size.y + SMALL_PIXEL_NUMBER >= two.Position.y)
        collisionY = true;
    if (one.Position.y > two.Position.y)
        return false;
    return collisionY;
}  






