#pragma once
#include "Components.hpp"
#include "../../Vector2D.hpp"

class TransformComponent : public Component{
    public:
        Vector2D position;
        Vector2D velocity;

        int height = 32;
        int width = 32;
        int scale = 1;

        int speed = 5;

        TransformComponent(){
            position.Zero();
        }

        TransformComponent(int sc){
            position.x = 400;
            position.y = 520;
            scale = sc;
        }

        TransformComponent(float x, float y, int sc){
            position.x = x;
            position.y = y;
            scale = sc;
        }

        TransformComponent(float x, float y, int h, int w, int sc){
            position.x = x;
            position.y = y;
            height = h;
            width = w;
            scale = sc;
        }

        void init() override{
            velocity.Zero();
        }

        void update() override{
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
};