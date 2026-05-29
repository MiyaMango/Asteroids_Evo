#include "asteroid.hpp"
#include "raylib.h"
#include <cmath>
#include <functional>


Asteroid::Asteroid(float x, float y, int window_w, int window_h, Texture2D& ast_tex, unsigned int id, std::function<void(void)> callback)
: Entity(x,y,window_w,window_h,ast_tex,id,callback)
{
    collisionradius     = GetRandomValue(min_radius,max_radius);
    speeds              = {0.01f*GetRandomValue(-max_speed,max_speed),0.01f*GetRandomValue(-max_speed,max_speed)};
    type                = 1;
}
