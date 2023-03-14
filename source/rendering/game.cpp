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


// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
GameObject      *Ghost_1;


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
    ResourceManager::LoadTexture("../../ghost_red.jpg", false, "ghost");
    ResourceManager::LoadTexture("../../coke.jpg", false, "coke");
    GameLevel one; one.Load("../../resources/one.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Level = 0;
    //game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
    glm::vec2 ghostPos = glm::vec2(this->Width / 4.0f - PLAYER_SIZE.x / 4.0f, this->Height / 4.0f - PLAYER_SIZE.y / 4.0f);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("pacman"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f));
    Ghost_1 = new GameObject(ghostPos, PLAYER_SIZE, ResourceManager::GetTexture("ghost"), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f));
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
    //delete the bricks when you make a collision
    this->DoCollisions();
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

void Game::CheckMoveColissions()
{
    for (int i = 0; i < 4; i++)
    {
        (Player->CurrentCollision)[i] = false;
    }
    for (GameObject &box : this->Levels[this->Level].Bricks)
        {
            if (CheckCollisionRight(*Player, box))
            {
                (Player->CurrentCollision)[0] = true;
            }
            if (CheckCollisionLeft(*Player, box))
            {
                (Player->CurrentCollision)[1] = true;
            }
            if (CheckCollisionUp(*Player, box))
            {
                (Player->CurrentCollision)[2] = true;
            }
            if (CheckCollisionDown(*Player, box))
            {
                (Player->CurrentCollision)[3] = true;
            }
        }
}

void Game::ProcessInput(float dt)
{   
   if (this->State == GAME_ACTIVE)
   {
        //TODO: convert to a function
        CheckMoveColissions();

        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if ((Player->Position.x) >= 0.0f && ((Player->CurrentCollision)[1] == 0))
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if ((Player->Position.x <= this->Width - Player->Size.x) && ((Player->CurrentCollision)[0] == 0)) 
                Player->Position.x +=velocity;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if ((Player->Position.y <= this->Height - Player->Size.y) && ((Player->CurrentCollision)[3] == 0))
                Player->Position.y += velocity;
        }
         if (this->Keys[GLFW_KEY_W])
        {
            if ((Player->Position.y >= 0.0f) && ((Player->CurrentCollision)[2] == 0))
                Player->Position.y -= velocity;
        }
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f));
        //Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        this->Levels[this->Level].Draw(*Renderer);
        //this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        Ghost_1->Draw(*Renderer);
        //std::cout<< Player->Position.x << std::endl;
        //std::cout<< Player->Position.y << std::endl;
    }
}