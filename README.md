In this project, I am trying to teach an evolutive algorithm to play an asteroids-like game. 
Their goal is simply to move forward as much as possible. Robots, which control ships, gain points every time they move forward. They will gain less points if they are drifting sideways, and they will lose points for moving backwards. They will also lose points for collisions, and may eventually die after too many collisions. After a set amount of simulation time, they will reproduce and the best bots will propagate their genes.

This project is ongoing and in early development.

To compile and run on Windows: Install [w64devkit](https://github.com/skeeto/w64devkit). Navigate to the /src directory, and use ``'make run'``.

To compile and run on Linux: Make sure you have the ``'libgl1-mesa-dev'`` package installed. If you don't, install it using ``sudo apt install libgl1-mesa-dev``.
Navigate to the /src directory, and use ``'make run'``.
