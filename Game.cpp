#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "src/ECS/Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,2060,2280};

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& enemy(manager.addEntity());

// const char* mapfile = "assets/terrain_tex.png";


// auto& enemies(manager.getGroup(groupEnemies));

Game::~Game(){

}

Game::Game(){

}


void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen){
    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystems initialised!..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window){
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!" << std::endl;
        }

        isRunning = true;
    }
    else{
        isRunning = false;
    }

    map = new Map("assets/terrain_tex.png", 3, 32);

    map->LoadMap("assets/map.map", 30, 30);

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("assets/player_anims.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    enemy.addComponent<TransformComponent>(360, 520, 2);
    enemy.addComponent<SpriteComponent>("assets/enemy.png", true);
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(groupPlayers);
    // wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
    // wall.addComponent<SpriteComponent>("assets/dirt.png");
    // wall.addComponent<ColliderComponent>("wall");
    // wall.addGroup(groupMap);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handleEvents(){
    
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

void Game::update(){
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    manager.refresh();
    manager.update();

    for(auto& c : colliders){
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if(Collision::AABB(cCol, playerCol)){
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    camera.x = player.getComponent<TransformComponent>().position.x - 400;
    camera.y = player.getComponent<TransformComponent>().position.y - 320;

    if(camera.x < 0){
        camera.x = 0;
    }
    if(camera.y < 0){
        camera.y = 0;
    }
    if(camera.x > camera.w){
        camera.x = camera.w;
    }
    if(camera.y > camera.h){
        camera.y = camera.h;
    }
}

void Game::render(){
    SDL_RenderClear(renderer);
    for(auto& t : tiles){
        t->draw();
    }

    for(auto& c : colliders){
        c->draw();
    }

    for(auto& p : players){
        p->draw();
    }
    // for(auto& e : enemies){
    //     e->draw();
    // }

    SDL_RenderPresent(renderer);
}

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}