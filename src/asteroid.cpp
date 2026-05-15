#include "asteroid.hpp"
#include "raylib.h"
#include <cmath>


Asteroid::Asteroid(float x, float y, int window_w, int window_h, Texture2D& ast_tex, unsigned int id)
: Entity(x,y,window_w,window_h,ast_tex,id)
{
    collisionradius     = GetRandomValue(min_radius,max_radius);
    speeds              = {0.01f*GetRandomValue(-max_speed,max_speed),0.01f*GetRandomValue(-max_speed,max_speed)};
    type                = 1;
}
