#ifndef SHIP_H
#define SHIP_H

#include <raylib.h>
#include <vector>
#include "Entity.hpp"

class Ship : public Entity {
private:
    // Sensor Configurations
    static const int NUM_RAYS = 5;
    float ray_max_dist;             // Maximum vision range
    std::vector<float> ray_sensors; // Stores current values (0.0 to 1.0)

public:
    float acceleration;             // linear acceleration
    float drag;                     // linear drag
    float angular_acceleration;     // angular acceleration
    float angular_drag;             // angular drag
    float desired_movement_accumulator;
    float score;                   
    float max_angular_velocity;     // angular speed cap
    bool is_best;

    // Constructor
    Ship(float x, float y, int window_w, int window_h, Texture2D& ship_tex, unsigned int id, std::function<void(void)> callback);

    // Overridden methods
    void update() override;
    void DrawExtra() override;

    // Ship specific methods
    virtual std::vector<bool> scan_inputs();
    void movement(std::vector<bool> inputs);
    void set_best_ship();
    void reset_sensors();
    void sense_walls();
    void update_sensor_with_entity(Entity* other);
    
    // Returns normalized sensor data for Neural Network input
    std::vector<double> getSensors() const;
};

#endif