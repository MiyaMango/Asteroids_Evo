#ifndef SPATIAL_HASH_GRID_H
#define SPATIAL_HASH_GRID_H

#include "entity.hpp"
#include "ship.hpp"
#include <vector>
#include <unordered_map>

class SpatialHashGrid {
private:
    float cellSize;
    std::unordered_map<size_t, std::vector<Entity*>> grid;

    inline std::pair<int,int> getCell(Vector2& p);
    inline size_t hashCell(int x, int y);
    
public:
    explicit SpatialHashGrid(float cellSize);

    void clear();
    void insert(Entity* e);
    void build(std::vector<Entity*>& entities);
    void computeCollisions(std::vector<Entity*>& entities);
    void computeFOV(std::vector<Entity*>& entities);

};

#endif