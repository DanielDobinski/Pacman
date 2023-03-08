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
    ResourceManager::LoadTexture("../../container.jpg", false, "face");
    ResourceManager::LoadTexture("../../block.png", false, "block");
    ResourceManager::LoadTexture("../../block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../../pacman.jpg", false, "pacman");
    GameLevel one; one.Load("../../resources/one.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Level = 0;
    //game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2 - PLAYER_SIZE.y / 2);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("pacman"));
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
     
}

void Game::ProcessInput(float dt)
{
   if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x +=velocity;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (Player->Position.y <= this->Height - Player->Size.y)
                Player->Position.y += velocity;
        }
         if (this->Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 0.0f)
                Player->Position.y -= velocity;
        }
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        //std::cout<< Player->Position.x << std::endl;
        //std::cout<< Player->Position.y << std::endl;
    }
}