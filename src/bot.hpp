#ifndef AG_H
#define AG_H

#include <cmath>
#include <vector>
#include "ship.hpp"
#include "rng.hpp"

class Bot{

private:
    Ship* ship;
    std::vector<double> genome;
    double score;

public:
    // constructors ----------------------------------------------------------------------------

    Bot(Ship* body, std::vector<double> dna);

    // getters & setters ------------------------------------------------------------------------

    std::vector<double> get_genome();

    void set_genome(std::vector<double> new_genome);

    bool get_alive_status();

    float get_coll_radius();

    float get_score();
    // methods ----------------------------------------------------------------------------------
    std::vector<bool> movement_decision();
    void movement();

};


#endif