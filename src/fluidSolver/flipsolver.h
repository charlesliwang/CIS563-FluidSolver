#ifndef FLIPSOLVER_H
#define FLIPSOLVER_H


#include "fluidSolver.hpp"
#include "grid.h"

using namespace std;

class FlipSolver : public FluidSolver
{
public:
    FlipSolver();
    FlipSolver(float cx, float cy, float cz, float px, float py, float pz, float separation);
    MACGrid mac_grid;
    void construct_mac_grid(int i, int j, int k);
    void init();
    void store_particle_velocity_to_grid();
    vec3 interpolate_velocity(const vec3& pos, MACGrid& grid);
};
#endif // FLIPSOLVER_H
