#include <iostream>

#include <cmath>
#include <vector>
#include <ctime>
#include <algorithm>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "asteroid.hpp"
#include "ship.hpp"
#include "entity.hpp"
#include "world.hpp"
#include "evolution.hpp"   
#include "bot.hpp"

// --- SETTINGS ---

#define BOT_COUNT 10
#define ASTEROID_COUNT 10

#define CELL_SIZE 32
#define worldWidth 1500
#define worldHeight 1000
#define GENOME_SIZE 32
#define WORLD_COUNT 4

using namespace std;
using Genome = vector<vector<double>>;
using ScoredGenome = pair<vector<double>, float>;
using ScoredGenomeList = vector<ScoredGenome>;

int main() {
    // init ------------------------------------------------------------------------------------

    SetRandomSeed((time(NULL)));
    InitWindow(worldWidth, worldHeight, "RonaldoBots");
    SetTargetFPS(60);

    bool draw_graphics = true;
    bool draw_debug = true;
    bool capped_fps = true;
    bool paused = false;
    float gen_duration = 2000;
    int generation = 1;
    
    //------------------------------------------------------------------------------------------


    // load textures ---------------------------------------------------------------------------
    
    std::vector<Texture2D> textures;
    textures.emplace_back(LoadTexture("../assets/millenium.png"));
    textures.emplace_back(LoadTexture("../assets/asteroid.png"));
    textures.emplace_back(LoadTexture("../assets/xwing.png"));

    //------------------------------------------------------------------------------------------

    // create universe -------------------------------------------------------------------------

    vector<World> universe;

    // create first worlds with random genomes
    for(int i=0; i<WORLD_COUNT ; i++){
        World first_world = World(BOT_COUNT, ASTEROID_COUNT,worldWidth,worldHeight,CELL_SIZE, textures, gen_duration);
        universe.push_back(first_world);
    }
    printf("starting generation %d ------------------------------- \n", generation);
    //------------------------------------------------------------------------------------------

    // main loop -------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        
        //update worlds if not paused
        if(!paused){
            for(int i = 0; i<WORLD_COUNT ; i++) universe[i].update();
        }

        //on epoch finish:
        if (all_of(universe.begin(), universe.end(), [&](auto& a) { return a.isFinished(); })){
            
            //take avg of scores from all universes
            ScoredGenomeList avgScores = universe[0].getResult(); // save the genome from the first universe
            printf("\n----\n");
            for(int j = 0; j < BOT_COUNT; j++){
                    printf("score of bot %d in world %d: %f ", j, avgScores[j].second);
            }
            printf("\n----\n");

            // accumulate bot scores to calculate avg
            for (int i=1; i<WORLD_COUNT; i++){
                const ScoredGenomeList& currentUniverseResults = universe[i].getResult();
                for(int j = 0; j < BOT_COUNT; j++){
                    avgScores[j].second += currentUniverseResults[j].second;
                    printf("score of bot %d in world %d: %f ", j, i, currentUniverseResults[j].second);
                }
                printf("\n----\n");
            }

            //free the universe
            universe.clear();

            //calculate avg
            for (int i=0; i<BOT_COUNT; i++){
                avgScores[i].second /= WORLD_COUNT;
                printf("avg score of bot %d in generation %d: %f\n", i, generation, avgScores[i].second);
            }

            float avg_generation_score;
            float accum = 0;
            for (int i=0; i<BOT_COUNT; i++){
                accum += avgScores[i].second;
            }
            accum /= BOT_COUNT*1.0;
            printf("generation %d avg score: %f\n", generation, accum);

            //use avg to evolve algorithm
            Evo evo(BOT_COUNT, avgScores, GENOME_SIZE);
            Genome next_genomes = evo.repopulation();

            //make new universes
            for(int i=0; i<WORLD_COUNT; i++){
                World new_world = World(BOT_COUNT, ASTEROID_COUNT,worldWidth,worldHeight,CELL_SIZE, textures, gen_duration, next_genomes);
                universe.push_back(new_world);
            }
            generation++;
            printf("starting generation %d ------------------------------- \n", generation);
        }

        // Draw --------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);

            if(draw_graphics){
                universe[0].Draw();
                if(draw_debug) universe[0].DrawExtra();
            }
            
            // GUI ----------------------------------------------------------------------------
            DrawFPS(worldWidth - 100, 10);
            DrawText(TextFormat("GENERATION: %i", generation), 10, 50, 20, GREEN);
            DrawText(TextFormat("TIME: %i", universe[0].getTime()), 10, 85, 20, GREEN);
            DrawText(TextFormat("WORLDS: %d", WORLD_COUNT), 200, 50, 20, GREEN);
            GuiSliderBar((Rectangle){120, 20, 200, 20 }, "Generation Duration", TextFormat("%.0f", gen_duration), &gen_duration, 250, 3000);
            if (GuiButton((Rectangle){370, 20, 120, 30 }, "Toggle Graphics")) draw_graphics = !draw_graphics;
            if (GuiButton((Rectangle){500, 20, 100, 30 }, "Toggle Debug")) draw_debug = !draw_debug;
            if (GuiButton((Rectangle){610, 20, 120, 30 }, "Toggle FPS cap")){
                if(capped_fps){
                    SetTargetFPS(0);
                }else{
                    SetTargetFPS(60);
                }
                capped_fps = !capped_fps;
            }
            if (GuiButton((Rectangle){740, 20, 50, 30 }, "Pause")){
                paused = !paused;
            }
            //----------------------------------------------------------------------------------
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }
    //------------------------------------------------------------------------------------------

    CloseWindow();

    return 0;
}

