/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "../../include/rendering/game.h"
#include "../../include/rendering/resource_manager.h"
#include "../../include/rendering/sprite_renderer.h"
#include "../../include/rendering/game_object.h"
#include "../../include/rendering/game_level.h"
#include <vector>

// Game-related State data
SpriteRenderer              *Renderer;
GameObject                  *Player;
GameObject                  *Ghost_1;     
GameObject                  *Ghost_2; 
GameObject                  *Ghost_3; 
std::vector<GameObject*>     Ghosts;
static int GameOverFlag = 0;
static int WinFlag = 0;

int getGameOver(void)
{
    return GameOverFlag;
}
int getWin(void)
{
    return WinFlag;
}
Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../../include/rendering/shader.vs", "../../include/rendering/shader.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    //set render
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    //load textures
    ResourceManager::LoadTexture("../../resources/background.jpg", false, "background");
    ResourceManager::LoadTexture("../../block.png", false, "block");
    ResourceManager::LoadTexture("../../block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../../pacman.png", false, "pacman");
    ResourceManager::LoadTexture("../../ghost_red.jpg", false, "ghost_red");
    ResourceManager::LoadTexture("../../ghost_blue.png", false, "ghost_blue");
    ResourceManager::LoadTexture("../../ghost_yellow.png", false, "ghost_yellow");
    ResourceManager::LoadTexture("../../coke.jpg", false, "coke");
    GameLevel one; one.Load("../../resources/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    //game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
    glm::vec2 ghostPos1 = glm::vec2(this->Width / 3.0f - PLAYER_SIZE.x / 3.0f, this->Height / 3.0f - PLAYER_SIZE.y / 3.0f);
    glm::vec2 ghostPos2 = glm::vec2(this->Width / 3.0f - PLAYER_SIZE.x / 3.0f, this->Height / 3.0f - PLAYER_SIZE.y / 3.0f);
    glm::vec2 ghostPos3 = glm::vec2(this->Width / 3.0f - PLAYER_SIZE.x / 2.0f, this->Height / 5.0f - PLAYER_SIZE.y / 2.0f);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("pacman"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f));
    Ghost_1 = new GameObject(ghostPos1, PLAYER_SIZE, ResourceManager::GetTexture("ghost_red"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f));
    Ghost_2 = new GameObject(ghostPos2, PLAYER_SIZE, ResourceManager::GetTexture("ghost_red"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f));
    Ghost_3 = new GameObject(ghostPos3, PLAYER_SIZE, ResourceManager::GetTexture("ghost_yellow"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f));
    Ghosts.push_back(Ghost_1);
    Ghosts.push_back(Ghost_2);
    Ghosts.push_back(Ghost_3);
    //DEBUG
    GameObject                 tempObject;
    std::vector<GameObject>    tempObjects;
    GameLevel                  tempLevel;
    tempLevel = this->getGameLevel(0);
    tempObjects = tempLevel.getGameObject();
    tempObject = tempObjects[3];
    std::cout<< tempObject.Position.x << std::endl;
    std::cout<< tempObject.Position.y << std::endl;
    //std::cout<< Player->Position.y << std::endl;
}

void Game::Update(float dt)
{
    float velocity = GHOST_VELOCITY * dt;
    //delete the Food when you make a collision
    this->DoCollisions();
    CheckMoveColissions(Ghosts[0]);
    CheckMoveColissions(Ghosts[1]);
    CheckMoveColissions(Ghosts[2]);
    
    if (CheckCollision(*Player, *(Ghosts[0])) || CheckCollision(*Player, *(Ghosts[1])) || CheckCollision(*Player, *(Ghosts[2])))
    { GameOverFlag = 1;}

    (Ghosts[0])->MoveRandom(velocity);
    (Ghosts[1])->MoveRandom(velocity);
    (Ghosts[2])->MoveRandom(velocity);
    this->Food = (this->getGameLevel(0)).getRemainingFood();
    std::cout<< this-> Food <<  std::endl;
    if(Food == 0)
    {
        WinFlag = 1;
    }
}

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].Food)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Player, box))
            {
                if (!box.IsSolid)
                    box.Destroyed = true;
            }
        }
    }
}  
void Game::CheckMoveColissions(GameObject * object)
{
    for (int i = 0; i < 4; i++)
    {
        (object->CurrentCollision)[i] = false;
    }
    for (GameObject &box : this->Levels[this->Level].Bricks)
        {
            if (CheckCollisionRight(*object, box))
            {
                (object->CurrentCollision)[0] = true;
            }
            if (CheckCollisionLeft(*object, box))
            {
                (object->CurrentCollision)[1] = true;
            }
            if (CheckCollisionUp(*object, box))
            {
                (object->CurrentCollision)[2] = true;
            }
            if (CheckCollisionDown(*object, box))
            {
                (object->CurrentCollision)[3] = true;
            }
        }
}

void Game::ProcessInput(float dt)
{   
   if (this->State == GAME_ACTIVE)
   {
        //TODO: convert to a function
        CheckMoveColissions(Player);

        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            Player->Rotation = 0.0f;
            if ((Player->Position.x) >= 0.0f && ((Player->CurrentCollision)[1] == 0))
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            Player->Rotation = 0.0f;
            if ((Player->Position.x <= this->Width - Player->Size.x) && ((Player->CurrentCollision)[0] == 0)) 
                Player->Position.x +=velocity;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            Player->Rotation = 90.0f;
            if ((Player->Position.y <= this->Height - Player->Size.y) && ((Player->CurrentCollision)[3] == 0))
                Player->Position.y += velocity;
        }
         if (this->Keys[GLFW_KEY_W])
        {
            Player->Rotation = 270.0f;
            if ((Player->Position.y >= 0.0f) && ((Player->CurrentCollision)[2] == 0))
                Player->Position.y -= velocity;
        }
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        if(GameOverFlag == 1)
        {
            Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f));
        }
        //Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        this->Levels[this->Level].Draw(*Renderer);
        //this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        Ghosts[0]->Draw(*Renderer);
        Ghosts[1]->Draw(*Renderer);
        Ghosts[2]->Draw(*Renderer);
        //std::cout<< Player->Position.x << std::endl;
        //std::cout<< Player->Position.y << std::endl;
    }
}