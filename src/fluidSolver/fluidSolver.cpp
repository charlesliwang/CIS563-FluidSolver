//
//  fluidSolver.cpp
//  Thanda


#include "fluidSolver.hpp"

FluidSolver::FluidSolver() {}

FluidSolver::FluidSolver(float cx, float cy, float cz, float px, float py, float pz, float separation){
    container_size = vec3(cx,cy,cz);
    particles_size = vec3(px,py,pz);
    particle_separation = separation;
    particles = new vector<vec3>();
    colors = new vector<vec4>();
    std::cout << particle_separation << std::endl;
}

vec3 FluidSolver::get_container_bounds() {
    return container_size;
}
vec3 FluidSolver:: get_particles_size() {
    return particles_size;
}

float FluidSolver:: get_separation() {
    return particle_separation;
}

void FluidSolver::create_particles() {
    float id = 0;
    float i = 0;
    while(i < particles_size.x) {
        float j = 0;
        while(j < particles_size.y) {
            float k = 0;
            while(k < particles_size.z){
                particles->push_back(
                            vec3(i - particles_size.x/2.0f,
                                 j - particles_size.y/2.0f,
                                 k - particles_size.z/2.0f));
                particles_id.push_back(Particle(id));
                colors->push_back(vec4(i - particles_size.x/2.0f,
                                       j - particles_size.y/2.0f,
                                       k - particles_size.z/2.0f, 1.0f));
                k += particle_separation;
            }
            j += particle_separation;
        }
        i += particle_separation;
    }
}

void FluidSolver::update(Cube *cube)
{
    for(int i = 0; i < particles->size(); i++) {
        particles_id.at(i).vel += g/6000.0f;
        particles->at(i) = particles->at(i) + vec3(0,particles_id.at(i).vel,0);
        if(cube->collision(particles->at(i))) {
            colors->at(i) = vec4(1.0f,0,0,1.0f);
        }
    }

}

Particle::Particle(int id)
{
    this->id = id;
    vel = 0;
}
