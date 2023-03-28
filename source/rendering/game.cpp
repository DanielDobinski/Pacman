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
#include "../../include/rendering/particle.h"
#include "../../include/rendering/post_processor.h"
#ifndef GAME_SETTINGS_H
    #include "../../include/rendering/game_settings.h"
#endif
#include <vector>

// Game-related State data
SpriteRenderer                  *Renderer;
MoveableObject                  *Player;
MoveableObject                  *Ghost_1;     
MoveableObject                  *Ghost_2; 
MoveableObject                  *Ghost_3; 
ParticleGenerator               *Particles; 
std::vector<MoveableObject*>     Ghosts;
PostProcessor                   *Effects;
static float                            ShakeTime = 0.0f;  
static struct GameEvents_TAG gameEvents;

static void loadTextures()
{
    ResourceManager::LoadTexture("../../resources/background.jpg", false, "background");
    ResourceManager::LoadTexture("../../resources/block.png", false, "block");
    ResourceManager::LoadTexture("../../resources/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../../resources/pacman.png", false, "pacman");
    ResourceManager::LoadTexture("../../resources/ghost_red.jpg", false, "ghost_red");
    ResourceManager::LoadTexture("../../resources/ghost_blue.png", false, "ghost_blue");
    ResourceManager::LoadTexture("../../resources/ghost_yellow.png", false, "ghost_yellow");
    ResourceManager::LoadTexture("../../resources/coke.jpg", false, "food");
    ResourceManager::LoadTexture("../../resources/particle.png", true, "particle"); 
}
static void loadGhosts(Game * gamePtr)
{
    GameLevel one; one.Load("../../resources/one.lvl", gamePtr->Width, gamePtr->Height);
    gamePtr->Levels.push_back(one);
    gamePtr->Level = 0;
    //game objects
    glm::vec2 playerPos = glm::vec2(gamePtr->Width / 2.0f - PLAYER_SIZE.x / 2.0f, gamePtr->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
    glm::vec2 ghostPos1 = glm::vec2(gamePtr->Width / 3.0f - PLAYER_SIZE.x / 3.0f, gamePtr->Height / 3.0f - PLAYER_SIZE.y / 3.0f);
    glm::vec2 ghostPos2 = glm::vec2(gamePtr->Width / 3.0f - PLAYER_SIZE.x / 3.0f, gamePtr->Height / 3.0f - PLAYER_SIZE.y / 3.0f);
    glm::vec2 ghostPos3 = glm::vec2(gamePtr->Width / 3.0f - PLAYER_SIZE.x / 2.0f, gamePtr->Height / 5.0f - PLAYER_SIZE.y / 2.0f);
    Player = new MoveableObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("pacman"), glm::vec3(1.0f), PLAYER_VELOCITY); 
    Ghost_1 = new MoveableObject(ghostPos1, PLAYER_SIZE, ResourceManager::GetTexture("ghost_red"), glm::vec3(1.0f), GHOST_VELOCITY);
    Ghost_2 = new MoveableObject(ghostPos2, PLAYER_SIZE, ResourceManager::GetTexture("ghost_blue"), glm::vec3(1.0f), GHOST_VELOCITY);
    Ghost_3 = new MoveableObject(ghostPos3, PLAYER_SIZE, ResourceManager::GetTexture("ghost_yellow"), glm::vec3(1.0f), GHOST_VELOCITY);
    Ghosts.push_back(Ghost_1);
    Ghosts.push_back(Ghost_2);
    Ghosts.push_back(Ghost_3);
}
Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ghost_1;
    delete Ghost_2;
    delete Ghost_3;
}

void Game::Init()
{
    ResourceManager::LoadShader("../../include/rendering/shader.vs", "../../include/rendering/shader.fs", nullptr, "sprite");
    ResourceManager::LoadShader("../../resources/particle.vs", "../../resources/particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("../../resources/post_processing.vs", "../../resources/post_processing.fs", nullptr, "postprocessing");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
    static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    loadTextures();
    loadGhosts(this);

    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
}

void Game::Update(float dt)
{
    gameEvents = getGameEvents();
    Player->setVelocity(gameEvents._pacmanVelocity);
    for (auto ghost : Ghosts)
         ghost->setVelocity(gameEvents._ghostsVelocity);

    //delete the Food when you make a collision
    this->DoCollisions();
    //update state of possibility of movement for each ghost
    //check if player didn't touch the ghost
    for (auto ghost : Ghosts)
    {
        CheckMoveColissions(ghost);
        if(CheckCollision(*Player, *ghost))
            this->State = GAME_LOSE;
        ghost->MoveRandom(ghost->Velocity * dt);
    }

    this->Food = (this->getGameLevel(0)).getRemainingFood();
    if(Food == 0)
    {
        this->State = GAME_WIN;
    }

    Particles->Update(dt, *Player, 2, glm::vec2(Player->Size / 2.0f));
}

void Game::DoCollisions()
{
    //check every food for collision with the Player
    //if it's present, then destroy the food.
    for (auto & food : this->Levels[this->Level].Food)
    {
        if (!food.Destroyed)
        {
            if (CheckCollision(*Player, food))
            {
                    food.Destroyed = true;
            }
        }
    }
}  

void Game::ProcessInput(float dt)
{   
    if(gameEvents._goThroughWalls == false)
        CheckMoveColissions(Player);
    else
    {
        for(auto collision : Player->CurrentCollision)
        {
            collision = false;
        }
    }
    std::cout << gameEvents._shake << std::endl;
    if(gameEvents._shake == true)
    {
        Effects->Shake = true;
    } else
        Effects->Shake = false;
    if(gameEvents._confuse == true)
    {
        Effects->Confuse = true;
    } else
        Effects->Confuse = false;
    if(gameEvents._chaos == true)
    {
        Effects->Chaos = true;
    } else
        Effects->Chaos = false;

    float velocity = Player->Velocity * dt;
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

void Game::Render()
{
    Effects->BeginRender();
        if(State == GAME_WIN || State == GAME_LOSE)
        {
            Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f));
        }
        this->Levels[this->Level].Draw(*Renderer);
        if(this->State == GAME_ACTIVE)
        {
            if(gameEvents._particles == true)
                Particles->Draw();
            Player->Draw(*Renderer);
        }
        for (auto ghost : Ghosts)
            ghost->Draw(*Renderer);

    Effects->EndRender();
    Effects->Render(glfwGetTime());
}

void Game::CheckMoveColissions(MoveableObject * object)
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

GameLevel Game::getGameLevel(unsigned int l)
{return Levels[l];}

