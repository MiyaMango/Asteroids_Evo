<<<<<<< HEAD

#include "bot.hpp"
#include <cmath>
#include <vector>
#include "ship.hpp"
#include "rng.hpp"

using namespace std;

// constructors -----------------------------------------------------------------------------

Bot::Bot(Ship* body, vector<double> dna)
{
    ship = body;
    score = 0;
    body->facing_angle = GetRandomValue(0,359);
    genome = dna;
}

// getters & setters ------------------------------------------------------------------------

vector<double> Bot::get_genome(){
    return(genome);
}

void Bot::set_genome(vector<double> new_genome){
    genome.clear();
    genome = new_genome;
}

bool Bot::get_alive_status(){
    return ship->active;
}

float Bot::get_coll_radius(){
    return(ship->get_collRadius());
}

// methods ----------------------------------------------------------------------------------

//after-round score post-processing
float Bot::get_score(){
    score = ship->score;
    return score;
}    

//decide which way to move
vector<bool> Bot::movement_decision(){

    vector<double> sensors = ship->getSensors();
    vector<bool> output;

    int gen_sensor_size = 7;

    for(int i=0; i<4; i++){
        double weightedSum = 0;
        for(int j = 0; j < sensors.size(); j++){
            int pos = j + i*gen_sensor_size;
            weightedSum += (sensors[j] * genome[pos]);
        }
        double threshold = genome[gen_sensor_size * 4 + i];
    
        if(weightedSum >= threshold){
            output.push_back(true);
        }
        output.push_back(false);
        

    }
    return(output); 
}

void Bot::movement(){
    vector<bool> inputs = movement_decision();
    ship->movement(inputs);
=======

#include "bot.hpp"
#include <cmath>
#include <vector>
#include "ship.hpp"
#include "rng.hpp"

using namespace std;

// constructors -----------------------------------------------------------------------------

Bot::Bot(Ship* body, vector<double> dna)
{
    ship = body;
    score = 0;
    body->facing_angle = GetRandomValue(0,359);
    genome = dna;
}

// getters & setters ------------------------------------------------------------------------

vector<double> Bot::get_genome(){
    return(genome);
}

void Bot::set_genome(vector<double> new_genome){
    genome.clear();
    genome = new_genome;
}

bool Bot::get_alive_status(){
    return ship->active;
}

float Bot::get_coll_radius(){
    return(ship->get_collRadius());
}

// methods ----------------------------------------------------------------------------------

//after-round score post-processing
float Bot::get_score(){
    score = ship->score;
    return score;
}    

//decide which way to move
vector<bool> Bot::movement_decision(){

    vector<double> sensors = ship->getSensors();
    vector<bool> output;

    int gen_sensor_size = 7;

    for(int i=0; i<4; i++){
        double weightedSum = 0;
        for(int j = 0; j < sensors.size(); j++){
            int pos = j + i*gen_sensor_size;
            weightedSum += (sensors[j] * genome[pos]);
        }
        double threshold = genome[gen_sensor_size * 4 + i];
    
        if(weightedSum >= threshold){
            output.push_back(true);
        }
        output.push_back(false);
        

    }
    return(output); 
}

void Bot::movement(){
    vector<bool> inputs = movement_decision();
    ship->movement(inputs);
>>>>>>> 325110fb93a5c8689592507857878177653534ad
}