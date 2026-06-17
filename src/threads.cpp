#include <mutex>
#include <condition_variable>
#include "world.hpp"

struct Worker {
    std::thread th;
    std::mutex m;
    std::condition_variable cv_start;
    std::condition_variable cv_done;
    World* world = nullptr;
    
    bool do_update = false;
    bool terminate = false;
    bool done = true;

    void start() {
        th = std::thread([this]() {
            while (true) {
                std::unique_lock<std::mutex> lock(m);
                // Sleep until the main thread gives us work (or tells us to quit)
                cv_start.wait(lock, [this] { return do_update || terminate; });
                if (terminate) break;

                // Do the physics simulation
                if (world && !world->isFinished()) {
                    world->update();
                }

                // Mark as done and wake up the main thread
                do_update = false;
                done = true;
                lock.unlock();
                cv_done.notify_one();
            }
        });
    }

    void triggerUpdate(World* w) {
        std::lock_guard<std::mutex> lock(m);
        world = w;
        do_update = true;
        done = false;
        cv_start.notify_one();
    }

    void waitDone() {
        std::unique_lock<std::mutex> lock(m);
        cv_done.wait(lock, [this] { return done; });
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(m);
            terminate = true;
            do_update = true; // ensure thread wakes up to see termination flag
        }
        cv_start.notify_one();
        if (th.joinable()) th.join();
    }
};