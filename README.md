# CIS563-FluidSolver
(Credit : CIS565 README)

Fluid Solver Submission guidelines:


- If you have modified any of the CMakeLists.txt files at all (aside from the list of CORE_SRC), you must test that your project can build. Beware of any build issues.

- Open a GitHub pull request so that we can see that you have finished. The title should be "Submission: YOUR NAME".

- In the body of the pull request, include a link to your repository.

- Submit on canvas with a direct link to your pull request on GitHub


And you're done!


README:

SPRINT 2: 
Running the program will prompt you to enter the json to be used in this scene (file directory is relative to the executable).
If you enter an invalid json, it will prompt you to enter the file again.
ALL REQUIRED FEATURES HAVE BEEN IMPLEMENTED

notes:
Time step: In the viewer class, the time step of the simulation can be adjusted by changing the value of float time_step on line 259
Gravity: At each update, all values of the V grid have been reset to a small negative value (-2.0f);
Update Particle Velocity: For this sprint I implemented forward Euler, rather than RK2.

adjusted from last sprint:
store_particle_velocity_to_grid:
 - the velocity of each grid is scaled by the number of particles that influence it

still probably somewhat broken:
 - interpolation of velocity needs to be written cleaner

SPRINT 1:

Running the program will prompt you to enter the json to be used in this scene (file directory is relative to the executable).
If you enter an invalid json, it will prompt you to enter the file again.
All functionality was implemented.

Running the program as is will generate a default MACGrid of dimensions 5x5x5.
There are tests for the store_particle_velocity_to_grid and interpolate_velocity are in the scene.cpp file
The print statements for the tests are commented out.

store_particle_velocity_to_grid:
 - the stored velocity of each particle is weighted by v/dist where
 	- v is the velocity in the respective axis
 	- dist is the distance between the particle and the center of the face of the respective axis

interpolate_velocity:
 - given a position, this function will return a vec3 of the interpolated velocity of the faces nearest to that position.

FROM VIEWER PROJECT:

Citation:
base code for VBO/VAO/EBO indexing:
http://www.learnopengl.com/#!Getting-started/Hello-Triangle
 - Helped me understand how to properly set up a gl context
this helped with understanding linking shaders with new attributes (color):
http://learnopengl.com/#!Getting-started/Shaders
 - Helped me understand binding buffers in a particular order

Running the program will prompt you to enter the json to be used in this scene (file directory is relative to the executable).
If you enter an invalid json, it will prompt you to enter the file again.
All functionality was implemented.

Interfacing:
Arrow keys will rotate you around the origin
Maya/Houdini mouse controls are also implemented (hold space + mouse drags)