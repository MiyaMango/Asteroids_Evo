#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <ctime>
#include <memory>
#include <algorithm>
#include <thread>
#include <format>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "asteroid.hpp"
#include "ship.hpp"
#include "fitnessgraph.cpp"
#include "entity.hpp"
#include "world.hpp"
#include "evolution.hpp"   
#include "bot.hpp"
#include "threads.cpp"

// --- SETTINGS ---

#define BOT_COUNT 10
#define ASTEROID_COUNT 10
#define CELL_SIZE 32
#define worldWidth 1600
#define worldHeight 1000
#define GENOME_SIZE 32
#define WORLD_COUNT 8

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
    bool player_active = false;
    bool render_anything = true;
    bool draw_debug = true;
    bool capped_fps = true;
    bool paused = false;
    float gen_duration = 2000;
    int generation = 1;
    Fitness_graph graph = Fitness_graph(10,130,300,100);
    ofstream logfile("log.txt");
    logfile << "-- STARTING NEW SIMULATION --\n";
    logfile << "Initial params:\n";
    logfile << "WORLD_COUNT: " << WORLD_COUNT << "\n";
    logfile << "GENOME_SIZE: " << GENOME_SIZE << "\n";
    logfile << "WORLD WIDTH: " << worldWidth << "\n";
    logfile << "WORLD HEIGHT: " << worldHeight << "\n"; 
    logfile << "GENERATION DURATION: " << gen_duration << "\n";
    
    //------------------------------------------------------------------------------------------


    // load textures ---------------------------------------------------------------------------
    
    vector<Texture2D> textures;
    textures.emplace_back(LoadTexture("../assets/millenium.png"));
    textures.emplace_back(LoadTexture("../assets/asteroid.png"));
    textures.emplace_back(LoadTexture("../assets/xwing.png"));

    //------------------------------------------------------------------------------------------

    // create universe -------------------------------------------------------------------------

    vector<unique_ptr<World>> universe;

    // create first worlds with random genomes
    for(int i = 0; i < WORLD_COUNT; i++) {
        universe.push_back(make_unique<World>(BOT_COUNT, ASTEROID_COUNT, worldWidth, worldHeight, CELL_SIZE, textures, gen_duration, player_active));
    }

    // Initialize Thread Pool
    vector<Worker> workers(WORLD_COUNT);
    for (int i = 0; i < WORLD_COUNT; i++) {
        workers[i].start();
    }

    logfile << format("starting generation {:d} ------------------------------- \n", generation);

    //------------------------------------------------------------------------------------------

    // main loop -------------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        
        //update worlds if not paused
        if(!paused){
            // Wake all threads and assign them their specific world for this tick
            for(int i = 0; i < WORLD_COUNT ; i++) {
                workers[i].triggerUpdate(universe[i].get());
            }
            
            // Wait for all physics simulations to finish before proceeding
            for(int i = 0; i < WORLD_COUNT ; i++) {
                workers[i].waitDone();
            }
        }

        //on epoch finish:
        if (all_of(universe.begin(), universe.end(), [&](auto& a) { return a->isFinished(); })){
            
            //get results from all worlds
            vector<ScoredGenomeList> all_results;
            for(int i = 0; i < WORLD_COUNT; i++) {
                all_results.push_back(universe[i]->getResult());
            }

            //save genomes from first world
            ScoredGenomeList avgScores = all_results[0]; 

            //get trimmed avg for each bot
            for (int j = 0; j < BOT_COUNT; j++) {
                vector<float> bot_scores;
                
                //get this bot's scores from all worlds
                for (int i = 0; i < WORLD_COUNT; i++) {
                    bot_scores.push_back(all_results[i][j].second);
                }

                //sort the scores
                sort(bot_scores.begin(), bot_scores.end());

                float score_sum = 0.0f;
                int valid_scores = 0;

                //ignore the highest and lowest
                for (int k = 1; k < WORLD_COUNT - 1; k++) {
                    score_sum += bot_scores[k];
                    valid_scores++;
                }

                //put the avg in results list
                avgScores[j].second = score_sum/valid_scores;
                
                logfile << format("avg score of bot {:d} in generation {:d} {:f}\n", j, generation, avgScores[j].second);
            }

            //free the universe
            universe.clear();

            //add info to graph
            float accum = 0;
            for (int i=0; i<BOT_COUNT; i++){
                accum += avgScores[i].second;
            }
            accum /= BOT_COUNT*1.0;
            logfile << format("generation {:d} avg score: {:f}\n", generation, accum);
            graph.add_datapoint(accum);

            //use avg to evolve algorithm
            Evo evo(BOT_COUNT, avgScores, GENOME_SIZE);
            Genome next_genomes = evo.repopulation();

            //make new universes
            for(int i = 0; i < WORLD_COUNT; i++) {
                bool spawn = false;
                if(i == 0) spawn = player_active;
                universe.push_back(std::make_unique<World>(BOT_COUNT, ASTEROID_COUNT, worldWidth, worldHeight, CELL_SIZE, textures, gen_duration, next_genomes, spawn));
            }
            generation++;
            logfile << format("starting generation {:d} ------------------------------- \n", generation);
        }

        // Draw --------------------------------------------------------------------------------
        
        BeginDrawing();
        if(render_anything){
            ClearBackground(BLACK);

            if(draw_graphics){
                universe[0]->Draw();
                if(draw_debug) universe[0]->DrawExtra();
            }
            
            // GUI ----------------------------------------------------------------------------
            DrawFPS(worldWidth - 100, 10);
            graph.Draw();
            DrawText(TextFormat("GENERATION: %i", generation), 10, 50, 20, GREEN);
            DrawText(TextFormat("TIME: %i", universe[0]->getTime()), 10, 85, 20, GREEN);
            DrawText(TextFormat("WORLDS: %d", WORLD_COUNT), 200, 50, 20, GREEN);
            DrawText(TextFormat("ALIVE: %d", universe[0]->getAlive()), 320, 50, 10, GREEN);
            GuiSliderBar({120, 20, 200, 20 }, "Generation Duration", TextFormat("%.0f", gen_duration), &gen_duration, 250, 3000);
            if (GuiButton({370, 20, 120, 30 }, "Toggle Graphics")) draw_graphics = !draw_graphics;
            if (GuiButton({500, 20, 100, 30 }, "Toggle Debug")) draw_debug = !draw_debug;
            if (GuiButton({610, 20, 100, 30 }, "Toggle Player")) player_active = !player_active;
            if (GuiButton({730, 20, 120, 30 }, "Toggle FPS cap")){
                if(capped_fps){
                    SetTargetFPS(0);
                }else{
                    SetTargetFPS(60);
                }
                capped_fps = !capped_fps;
            }
            if (GuiButton({870, 20, 50, 30 }, "Pause")){
                paused = !paused;
            }
            //----------------------------------------------------------------------------------
        }
        EndDrawing();

        //--------------------------------------------------------------------------------------
    }
    //------------------------------------------------------------------------------------------

    for (auto& w : workers) {
        w.stop();
    }

    CloseWindow();

    return 0;
}

