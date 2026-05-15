#ifndef WORLD_H
#define WORLD_H

#include "asteroid.hpp"
#include "raylib.h"
#include "entity.hpp"
#include "ship.hpp"
#include "bot.hpp"
#include "spatial_hash.hpp"
#include <vector>

class World{
private:
    int Max_entities;               //entity cap
    int Entity_count;               //entity counter
    int alive;                      //count of currently living bots
    int Obstacle_count;             //amount of obstacles to spawn
    int Duration;                   //number of ticks to simulate
    int timer;                      //ticks passed
    bool finished;                  //is simulation done

    const float Cell_size;          //size of unit square for collision
    int World_width;                //width
    int World_height;               //height
    std::vector<Entity*> Entities;  //vector with pointers to all entities
    std::vector<Bot> population;    //vector with all bots

    std::vector<Texture> Textures;  //texture dictionary
    SpatialHashGrid grid;           //collision hash map

public:

    //constructors & destructors
    World(int botCount, int obstCount, int width, int height, float cell_size, std::vector<Texture2D>& textures, int duration);
    World(int botCount, int obstCount, int width, int height, float cell_size, std::vector<Texture2D>& textures, int duration, std::vector<std::vector<double>> genomes);
    ~World();

    //getters & setters
    int getTime();
    void setTime(int newTime);
    bool isFinished();
    std::vector<std::pair<std::vector<double>,float>> getResult();

    //methods
    Entity* Spawn_entity(int type);
    void update();
    bool check_valid_spawn(Entity* candidate);
    void Draw();
    void DrawExtra();
};

#endif
