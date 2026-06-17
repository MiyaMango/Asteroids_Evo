
#include "world.hpp"
#include "rng.hpp"
#include <cmath>
#include <memory>
#include <string>
#include <functional>

using namespace std;
#define PLAYER_ACTIVE false
    
// constructors & destructors
World::World(int botCount, int obstCount, int width, int height, float cell_size, vector<Texture2D>& textures, int duration)
:Max_entities(botCount + obstCount + 1),
Entity_count(0),
World_width(width),
World_height(height),
Textures(textures),
Cell_size(cell_size),
grid(cell_size),
Duration(duration),
timer(0),
finished(false)
{
    // spawn player
    if(PLAYER_ACTIVE) Spawn_entity(0);

    // spawn obstacles
    for(int i = 0; i < obstCount; i++) Spawn_entity(1);

    // spawn bots with random genes
    alive = botCount;

    for(int i = 0; i < botCount; i++) {
        Ship* ship = dynamic_cast<Ship*>(Spawn_entity(2)); //create the ship
        vector<double> random_genome;
            for(int j = 0; j < 36; j++){
                random_genome.push_back(get_random_double(0,50));
            }
        population.emplace_back(ship, random_genome); //put the bot in the ship
    }
}

World::World(int botCount, int obstCount, int width, int height, float cell_size, vector<Texture2D>& textures, int duration, vector<vector<double>> genomes)
:Max_entities(botCount + obstCount + 1),
Entity_count(0),
World_width(width),
World_height(height),
Textures(textures),
Cell_size(cell_size),
grid(cell_size),
Duration(duration),
timer(0),
finished(false)
{
    // spawn player
    if(PLAYER_ACTIVE) Spawn_entity(0);

    // spawn obstacles
    for(int i = 0; i < obstCount; i++) Spawn_entity(1);

    // spawn bots with desired genes
    alive = botCount;

    for(int i = 0; i < botCount; i++) {
                Ship* ship = dynamic_cast<Ship*>(Spawn_entity(2));;
                if(i == 0) ship->set_best_ship();
                population.emplace_back(ship, genomes[i]);
    }
}

World::~World(){};


//getters & setters -------------------------------------------------------------------------

int World::getTime(){
    return timer;
}

int World::getAlive(){
    return alive;
}

void World::setTime(int newTime){
    timer = newTime;
}

bool World::isFinished(){
    return finished;
}

vector<pair<vector<double>,float>> World::getResult(){
    vector<pair<vector<double>,float>> result(population.size());

    for(int i = 0; i < population.size(); i++){
        result[i].first = population[i].get_genome();
        result[i].second = population[i].get_score();
    }

    return result;
}

// methods ----------------------------------------------------------------------------------

//spawn a new entity. types: 0- player ship, 1- asteroid, 2- bot ship
Entity* World::Spawn_entity(int type){
    Entity* new_entity;
    std::function<void(void)> death_callback = [&](){alive--;};

    switch(type){
        case 0:
            new_entity = new Ship(World_width/2, World_height/2, World_width, World_height, Textures[type], Entity_count++, death_callback);
        break;

        case 1:
            new_entity = new Asteroid(World_width/2,World_height/2,World_width,World_height, Textures[type], Entity_count++, death_callback);
        break;

        case 2:
            new_entity = new Ship(World_width/2,World_height/2,World_width,World_height, Textures[type], Entity_count++, death_callback);
            new_entity->collisionradius = 20;
            new_entity->type = 2;
        break;
    }

    while(check_valid_spawn(new_entity) == false){
        new_entity->randomize_position();
    }

    Entities.push_back(new_entity);
    return new_entity;
}

//update position, check for and respond to collision for all entities
//also update desired movement for all bots
void World::update(){
    if(isFinished()) return;
    if(timer >= Duration || Entities.empty() || alive == 0){
        finished = true;
        return;
    }

    timer++;

    grid.computeFOV(Entities); 
    
    for(auto& bot : population) {
        bot.movement();
    }

    for(auto& e : Entities){
        e->update();
    }

    //check collision between all entities (With Hash map)
    grid.build(Entities);
    grid.computeCollisions(Entities);
}

//checks if an entity isn't colliding with another
bool World::check_valid_spawn(Entity* candidate){

    for(size_t i = 0; i < Entities.size(); ++i){
        if (Entities[i]->active == false) continue;
        if (candidate->coll_check(Entities[i])) return false;
    }
    return true;

}

//draw all entities
void World::Draw(){
    for(auto e: Entities) e->Draw();
}

//drawextra all entities
void World::DrawExtra(){
    for(auto e: Entities) e->DrawExtra();
}
