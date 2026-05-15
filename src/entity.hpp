#ifndef ENTITY
#define ENTITY

#include "raylib.h"

class Entity {

private:
    Texture2D& texture;            // texture to use
    Rectangle source;              // region of the texture to use  

public:
    int screenHeight, screenWidth; // entity needs to know screen size   
    int coll_count;                // number of collisions happened
    int timeGap;                   // frames to wait until next collision count

    Vector2 position;              // position x,y
    Vector2 speeds;                // velocity x,y

    float speed_angle;             // angle of the speed vector (rad)
    float facing_angle;            // direction the entity is facing (rad)
    float angularvelocity;         // angular speed (rad)
    float abs_speed;               // speed modulo
    float collisionradius;         // collision radius
    float distancemoved;           // distance moved in entity's lifetime
    float alignment_coefficient;
    
    bool active;                   // is entity active
    bool killable;                 // is entity killable
    unsigned int id;               // entity id
    unsigned int type;             // entity type (0 = player, 1 = asteroid, 2 = bot)

    // Constructor
    Entity(float x, float y, int window_w, int window_h, Texture2D& ent_texture, unsigned int id);

    // Destructor
    virtual ~Entity() = default;

    // Getters & Setters
    Vector2 get_position();
    float get_collRadius();
    float get_distanceMoved();

    // Methods
    virtual void update();
    virtual void Draw();
    virtual void DrawExtra();
    
    float calculate_alignment_coefficient();
    void change_speed_collision(Entity* other);
    void kill();
    void unstuck(Entity* other, float dist);
    bool coll_check(Entity* other);
    void coll_response(Entity* other);
    void randomize_position();
};

#endif