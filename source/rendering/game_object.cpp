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
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Color(1.0f), Rotation(0.0f), Sprite(), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color) 
    : Position(pos), Size(size), Color(color), Rotation(0.0f), Sprite(sprite), Destroyed(false) { }

MoveableObject::MoveableObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, float velocity, std::vector<bool> CurrentCollision, int direction, int moveCount) 
    : GameObject(pos, size, sprite, color), Velocity(velocity),
     CurrentCollision({false, false, false, false}), Direction(Direction), MoveCount(MoveCount) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

void MoveableObject::setVelocity(float v)
{
    Velocity = v;
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

void MoveableObject::MoveRandom(float velocity)
{
    if(this->MoveCount == 300)
        this->MoveCount = 0;
    if(this->MoveCount == 0)
        {this->Direction = rand() % 4;}

    if (((this->CurrentCollision)[1] == 0) && (this->Direction == 0))
            {this->Position.x -= velocity;this->MoveCount++;}
    else if (((this->CurrentCollision)[0] == 0) && (this->Direction == 1))
            {this->Position.x +=velocity;this->MoveCount++;}
    else if (((this->CurrentCollision)[3] == 0) && (this->Direction == 2))
            {this->Position.y += velocity;this->MoveCount++;}
    else if (((this->CurrentCollision)[2] == 0) && (this->Direction == 3))
            {this->Position.y -= velocity;this->MoveCount++;}
    else
        {this->Direction = rand() % 4;MoveRandom(velocity);}
}

