//
//  fluidSolver.hpp
//  Thanda

#ifndef fluidSolver_hpp
#define fluidSolver_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../geom/geom.hpp"


using namespace glm;
using namespace std;


class Particle {
private:

public:
    Particle(int id);
    int id;
    float vel;
};

class FluidSolver  {
private:
    vec3 container_size;
    vec3 particles_size;
    float particle_separation;
    float g = -9.8;

public:
    FluidSolver();
    FluidSolver(float cx, float cy, float cz, float px, float py, float pz, float separation);
    vec3 get_container_bounds();
    vec3 get_particles_size();
    vector <vec3>*particles;
    vector <vec4>*colors;
    vector <Particle> particles_id;
    float get_separation();
    void update(Cube *cube);



    void create_particles();
};


#endif /* fluidSolver_hpp */
