# CIS563-FluidSolver
(Credit : CIS565 README)

Fluid Solver Submission guidelines:


- If you have modified any of the CMakeLists.txt files at all (aside from the list of CORE_SRC), you must test that your project can build. Beware of any build issues.

- Open a GitHub pull request so that we can see that you have finished. The title should be "Submission: YOUR NAME".

- In the body of the pull request, include a link to your repository.

- Submit on canvas with a direct link to your pull request on GitHub


And you're done!


README:
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