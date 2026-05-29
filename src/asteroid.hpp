#ifndef ASTEROID
#define ASTEROID

#include "raylib.h"
#include "entity.hpp"

class Asteroid : public Entity {
    private:
    int max_speed = 50;
    int min_radius = 50;
    int max_radius = 150;

    public:
    // constructor
    Asteroid(float x, float y, int window_w, int window_h, Texture2D& ast_tex, unsigned int id, std::function<void(void)> callback);
};


#endif