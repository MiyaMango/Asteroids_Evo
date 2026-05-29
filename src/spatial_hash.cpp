#include <vector>
#include <cmath>
#include "spatial_hash.hpp"


using namespace std;

inline pair<int,int> SpatialHashGrid::getCell(Vector2& p) {
    return {
        int(floor(p.x / cellSize)),
        int(floor(p.y / cellSize))
    };
}

inline size_t SpatialHashGrid::hashCell(int x, int y) {
    const size_t p1 = 73856093;
    const size_t p2 = 19349663;
    return size_t(x) * p1 ^ size_t(y) * p2;
}

SpatialHashGrid::SpatialHashGrid(float cellSize)
    :cellSize(cellSize) 
{
}

void SpatialHashGrid::clear(){
    grid.clear();
}

void SpatialHashGrid::insert(Entity* e) {
    auto [cx, cy] = getCell(e->position);
    float radius = e->get_collRadius();
    // Aumentamos um pouco a margem de inserção para garantir que objetos grandes sejam vistos
    float cell_radius = ceil(radius/cellSize);
    for (int ox = -cell_radius; ox <= cell_radius; ++ox) {
        for (int oy = -cell_radius; oy <= cell_radius; ++oy) {
            size_t h = hashCell(cx + ox, cy + oy);
            grid[h].push_back(e);
        }
    }
}

void SpatialHashGrid::build(vector<Entity*>& entities) {
    clear();
    for (auto e : entities) {
        if (e->active)
            insert(e);
    }
}

void SpatialHashGrid::computeCollisions(vector<Entity*>& entities) {
    for (auto e : entities) {
        if (!e->active) continue;

        auto [cx, cy] = getCell(e->position);
        float coll_radius = e->get_collRadius();
        int hash_radius = ceil(coll_radius/cellSize);

        for (int ox = -hash_radius; ox <= hash_radius; ++ox) {
            for (int oy = -hash_radius; oy <= hash_radius; ++oy) {
                size_t h = hashCell(cx + ox, cy + oy);
                auto it = grid.find(h);
                if (it == grid.end()) continue;

                for (Entity* other : it->second) {
                    if (other == e) continue;
                    if (!other->active) continue;
                    // Otimização: Só checa colisão se ID for maior para não checar A vs B e B vs A
                    if (other->id <= e->id) continue; 

                    if (e->coll_check(other)) {
                        e->coll_response(other);
                    }

                }
            }
        }
    }
}

// --- ATUALIZAÇÃO DA VISÃO ---
void SpatialHashGrid::computeFOV(vector<Entity*>& entities){
    for (auto e : entities) {
        if(!e->active) continue;
        
        // Só naves processam visão
        if(Ship* s = dynamic_cast<Ship*>(e)){
            
            s->reset_sensors();
            s->sense_walls(); 

            auto [cx,cy] = getCell(s->position);
            
            // O raio de busca no grid deve ser baseado no alcance da visão (300px), não no raio de colisão
            float vision_range = 150.0f; // Tem que bater com o ray_max_dist do bot
            int hash_radius = ceil(vision_range / cellSize);

            for (int ox = -hash_radius; ox <= hash_radius; ++ox) {
                for (int oy = -hash_radius; oy <= hash_radius; ++oy) {
                    size_t h = hashCell(cx + ox, cy + oy);
                    auto it = grid.find(h);
                    if (it == grid.end()) continue;

                    for (Entity* other : it->second) {
                        if (other == s) continue;
                        if (!other->active) continue;

                        s->update_sensor_with_entity(other);
                    }
                }
            }
        }
    }
}

