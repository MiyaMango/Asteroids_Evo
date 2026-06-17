<<<<<<< HEAD
using namespace std;

#include "entity.hpp"
#include <raymath.h>
#include <cmath>
#include <memory>

// constructor -----------------------------------------------------------------------------

Entity::Entity(float x, float y, int window_w, int window_h, Texture2D& ent_texture, unsigned int id, std::function<void(void)> callback)
: texture(ent_texture),
screenWidth(window_w),
screenHeight(window_h),
coll_count(0),
timeGap(0),
position({x,y}),
source({ 0, 0, ent_texture.width/1.0f, ent_texture.height/1.0f }),
distancemoved(0),
speed_angle(0),
angularvelocity(0),
facing_angle(0),
speeds({0,0}),
abs_speed(0),
collisionradius(0),
id(id),
active(true), 
killable(false),
alignment_coefficient(0),
callback(callback)
{
}
// getters e setters ------------------------------------------------------------------------

Vector2 Entity::get_position(){
    return(position);
}

float Entity::get_collRadius(){
    return(collisionradius);
}

float Entity::get_distanceMoved(){
    return(distancemoved);
}


// methods ----------------------------------------------------------------------------------
//entity update: returns 1 if entity died in this tick
void Entity::update(){
    if(!active) return;
    timeGap--;

    facing_angle += angularvelocity;
    position.x += speeds.x;
    position.y += speeds.y;
    if(facing_angle > 360*DEG2RAD) facing_angle -= DEG2RAD*360.0;
    if(facing_angle < 0) facing_angle += DEG2RAD*360.0;
    abs_speed = sqrt(speeds.x*speeds.x + speeds.y*speeds.y);
    distancemoved += abs_speed;
    speed_angle = atan2f(speeds.y, speeds.x);

    alignment_coefficient = calculate_alignment_coefficient();
    
    //this code block is for bouncing off of edges
    if(position.x + speeds.x >= screenWidth || position.x + speeds.x <= 0){
            position.x -= speeds.x;
            speeds.x *= -1;
            coll_count++;
            if(killable){ 
                kill();
            }
    }
    if(position.y + speeds.y >= screenHeight || position.y + speeds.y <= 0){
            position.y -= speeds.y;
            speeds.y *= -1;
            coll_count++;
            if(killable){ 
                kill();
            }
    }
}

void Entity::Draw(){
    if(!active) return;

    Rectangle dest;
    switch(type){
        case (0):
        dest = {position.x, position.y, collisionradius*2.3f, collisionradius*2};
        break;
        default:
        dest = {position.x, position.y, collisionradius*2, collisionradius*2};
        break;
    }
    Vector2 origin = {dest.width/2.0f, dest.height/2.0f};
    DrawTexturePro(texture, source, dest, origin, RAD2DEG*facing_angle, WHITE);
}

void Entity::DrawExtra(){
    if(!active) return;

    Vector2 velocity_endpoint = {position.x + 10*speeds.x, position.y + 10*speeds.y};
    Vector2 arrowhead_1 = {velocity_endpoint.x - abs_speed*cosf(speed_angle+DEG2RAD*30.0), velocity_endpoint.y - abs_speed*sinf(speed_angle+DEG2RAD*30)};
    Vector2 arrowhead_2 = {velocity_endpoint.x - abs_speed*cosf(speed_angle-DEG2RAD*30.0), velocity_endpoint.y - abs_speed*sinf(speed_angle-DEG2RAD*30)};
    Vector2 arrowhead_3 = {velocity_endpoint.x, velocity_endpoint.y};
    DrawLineV(position, velocity_endpoint, RED);
    DrawTriangle(arrowhead_1, arrowhead_2, arrowhead_3, RED);
    
    DrawCircleLines(position.x, position.y, collisionradius, RED);
}

float Entity::calculate_alignment_coefficient(){
    Vector2 velocity_end = {speeds.x, speeds.y};
    velocity_end = Vector2Normalize(velocity_end);
    Vector2 facing_end = {cos(facing_angle), sin(facing_angle)};
    facing_end = Vector2Normalize(facing_end);
    return Vector2DotProduct(velocity_end, facing_end);
}

void Entity::change_speed_collision(Entity* other) {
    // Vetor normal da colisão (direção entre os centros)
    Vector2 diff = {
        other->position.x - this->position.x,
        other->position.y - this->position.y
    };

    float d = sqrtf(diff.x * diff.x + diff.y * diff.y);
    if (d == 0.0f) return; // evita explosão

    Vector2 n = { diff.x / d, diff.y / d };
    Vector2 t = { -n.y, n.x };

    // Projeção das velocidades
    float vA_n = this->speeds.x * n.x + this->speeds.y * n.y;  // A normal
    float vA_t = this->speeds.x * t.x + this->speeds.y * t.y;  // A tangente

    float vB_n = other->speeds.x * n.x + other->speeds.y * n.y; // B normal
    float vB_t = other->speeds.x * t.x + other->speeds.y * t.y; // B tangente

    // Troca das componentes normais
    float mA = this->collisionradius*this->collisionradius;
    float mB = other->collisionradius*other->collisionradius;
    float new_vA_n = ( (mA - mB) * vA_n + 2 * mB * vB_n ) / (mA + mB);
    float new_vB_n = ( 2 * mA * vA_n + (mB - mA) * vB_n ) / (mA + mB);
    // Reconstrução das velocidades no espaço 2D
    this->speeds.x = new_vA_n * n.x + vA_t * t.x;
    this->speeds.y = new_vA_n * n.y + vA_t * t.y;

    other->speeds.x = new_vB_n * n.x + vB_t * t.x;
    other->speeds.y = new_vB_n * n.y + vB_t * t.y;
}

void Entity::kill(){
    active = false;
    callback();
}

void Entity::unstuck(Entity* other, float dist){
    if(dist == 0) return;
    float overlap = this->collisionradius + other->collisionradius - dist;
    if(overlap >= 0){
        Vector2 normal = {(other->position.x - this->position.x)/dist, (other->position.y - this->position.y)/dist};
        this->position.x -= normal.x*overlap*0.5f;
        this->position.y -= normal.y*overlap*0.5f;
        other->position.x += normal.x*overlap*0.5f;
        other->position.y += normal.y*overlap*0.5f;
    }
}

bool Entity::coll_check(Entity* other){
    if(!active || !other->active) return false;
    float dist = Vector2Distance(this->position, other->position);
    if(this->collisionradius + other->collisionradius >= dist) return true;
    return false;
}

void Entity::coll_response(Entity* other){
    float dist = Vector2Distance(this->position, other->position);
    unstuck(other, dist);

    if(timeGap < 1){
        coll_count++;
        timeGap = 8;
    }
    if(other->timeGap < 1){
        other->coll_count = other->coll_count + 1;
        other->timeGap = 8;
    }
    if(this->killable) kill();
    if(other->killable) other->kill();
    
    this->change_speed_collision(other);
}

void Entity::randomize_position(){
    position = { 1.0f*GetRandomValue(0,screenWidth) , 1.0f*GetRandomValue(0,screenHeight)};
}


=======
using namespace std;

#include "entity.hpp"
#include <raymath.h>
#include <cmath>
#include <memory>

// constructor -----------------------------------------------------------------------------

Entity::Entity(float x, float y, int window_w, int window_h, Texture2D& ent_texture, unsigned int id, std::function<void(void)> callback)
: texture(ent_texture),
screenWidth(window_w),
screenHeight(window_h),
coll_count(0),
timeGap(0),
position({x,y}),
source({ 0, 0, ent_texture.width/1.0f, ent_texture.height/1.0f }),
distancemoved(0),
speed_angle(0),
angularvelocity(0),
facing_angle(0),
speeds({0,0}),
abs_speed(0),
collisionradius(0),
id(id),
active(true), 
killable(false),
alignment_coefficient(0),
callback(callback)
{
}
// getters e setters ------------------------------------------------------------------------

Vector2 Entity::get_position(){
    return(position);
}

float Entity::get_collRadius(){
    return(collisionradius);
}

float Entity::get_distanceMoved(){
    return(distancemoved);
}


// methods ----------------------------------------------------------------------------------
//entity update: returns 1 if entity died in this tick
void Entity::update(){
    if(!active) return;
    timeGap--;

    facing_angle += angularvelocity;
    position.x += speeds.x;
    position.y += speeds.y;
    if(facing_angle > 360*DEG2RAD) facing_angle -= DEG2RAD*360.0;
    if(facing_angle < 0) facing_angle += DEG2RAD*360.0;
    abs_speed = sqrt(speeds.x*speeds.x + speeds.y*speeds.y);
    distancemoved += abs_speed;
    speed_angle = atan2f(speeds.y, speeds.x);

    alignment_coefficient = calculate_alignment_coefficient();
    
    //this code block is for bouncing off of edges
    if(position.x + speeds.x >= screenWidth || position.x + speeds.x <= 0){
            position.x -= speeds.x;
            speeds.x *= -1;
            coll_count++;
            if(killable){ 
                kill();
            }
    }
    if(position.y + speeds.y >= screenHeight || position.y + speeds.y <= 0){
            position.y -= speeds.y;
            speeds.y *= -1;
            coll_count++;
            if(killable){ 
                kill();
            }
    }
}

void Entity::Draw(){
    if(!active) return;

    Rectangle dest;
    switch(type){
        case (0):
        dest = {position.x, position.y, collisionradius*2.3f, collisionradius*2};
        break;
        default:
        dest = {position.x, position.y, collisionradius*2, collisionradius*2};
        break;
    }
    Vector2 origin = {dest.width/2.0f, dest.height/2.0f};
    DrawTexturePro(texture, source, dest, origin, RAD2DEG*facing_angle, WHITE);
}

void Entity::DrawExtra(){
    if(!active) return;

    Vector2 velocity_endpoint = {position.x + 10*speeds.x, position.y + 10*speeds.y};
    Vector2 arrowhead_1 = {velocity_endpoint.x - abs_speed*cosf(speed_angle+DEG2RAD*30.0), velocity_endpoint.y - abs_speed*sinf(speed_angle+DEG2RAD*30)};
    Vector2 arrowhead_2 = {velocity_endpoint.x - abs_speed*cosf(speed_angle-DEG2RAD*30.0), velocity_endpoint.y - abs_speed*sinf(speed_angle-DEG2RAD*30)};
    Vector2 arrowhead_3 = {velocity_endpoint.x, velocity_endpoint.y};
    DrawLineV(position, velocity_endpoint, RED);
    DrawTriangle(arrowhead_1, arrowhead_2, arrowhead_3, RED);
    
    DrawCircleLines(position.x, position.y, collisionradius, RED);
}

float Entity::calculate_alignment_coefficient(){
    Vector2 velocity_end = {speeds.x, speeds.y};
    velocity_end = Vector2Normalize(velocity_end);
    Vector2 facing_end = {cos(facing_angle), sin(facing_angle)};
    facing_end = Vector2Normalize(facing_end);
    return Vector2DotProduct(velocity_end, facing_end);
}

void Entity::change_speed_collision(Entity* other) {
    // Vetor normal da colisão (direção entre os centros)
    Vector2 diff = {
        other->position.x - this->position.x,
        other->position.y - this->position.y
    };

    float d = sqrtf(diff.x * diff.x + diff.y * diff.y);
    if (d == 0.0f) return; // evita explosão

    Vector2 n = { diff.x / d, diff.y / d };
    Vector2 t = { -n.y, n.x };

    // Projeção das velocidades
    float vA_n = this->speeds.x * n.x + this->speeds.y * n.y;  // A normal
    float vA_t = this->speeds.x * t.x + this->speeds.y * t.y;  // A tangente

    float vB_n = other->speeds.x * n.x + other->speeds.y * n.y; // B normal
    float vB_t = other->speeds.x * t.x + other->speeds.y * t.y; // B tangente

    // Troca das componentes normais
    float mA = this->collisionradius*this->collisionradius;
    float mB = other->collisionradius*other->collisionradius;
    float new_vA_n = ( (mA - mB) * vA_n + 2 * mB * vB_n ) / (mA + mB);
    float new_vB_n = ( 2 * mA * vA_n + (mB - mA) * vB_n ) / (mA + mB);
    // Reconstrução das velocidades no espaço 2D
    this->speeds.x = new_vA_n * n.x + vA_t * t.x;
    this->speeds.y = new_vA_n * n.y + vA_t * t.y;

    other->speeds.x = new_vB_n * n.x + vB_t * t.x;
    other->speeds.y = new_vB_n * n.y + vB_t * t.y;
}

void Entity::kill(){
    active = false;
    callback();
}

void Entity::unstuck(Entity* other, float dist){
    if(dist == 0) return;
    float overlap = this->collisionradius + other->collisionradius - dist;
    if(overlap >= 0){
        Vector2 normal = {(other->position.x - this->position.x)/dist, (other->position.y - this->position.y)/dist};
        this->position.x -= normal.x*overlap*0.5f;
        this->position.y -= normal.y*overlap*0.5f;
        other->position.x += normal.x*overlap*0.5f;
        other->position.y += normal.y*overlap*0.5f;
    }
}

bool Entity::coll_check(Entity* other){
    if(!active || !other->active) return false;
    float dist = Vector2Distance(this->position, other->position);
    if(this->collisionradius + other->collisionradius >= dist) return true;
    return false;
}

void Entity::coll_response(Entity* other){
    float dist = Vector2Distance(this->position, other->position);
    unstuck(other, dist);

    if(timeGap < 1){
        coll_count++;
        timeGap = 8;
    }
    if(other->timeGap < 1){
        other->coll_count = other->coll_count + 1;
        other->timeGap = 8;
    }
    if(this->killable) kill();
    if(other->killable) other->kill();
    
    this->change_speed_collision(other);
}

void Entity::randomize_position(){
    position = { 1.0f*GetRandomValue(0,screenWidth) , 1.0f*GetRandomValue(0,screenHeight)};
}


>>>>>>> 325110fb93a5c8689592507857878177653534ad
