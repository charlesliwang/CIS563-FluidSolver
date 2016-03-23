#ifndef GRID_H
#define GRID_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../geom/geom.hpp"

using namespace std;

class Grid {
private:

public:
    Grid();
    Grid(int i, int j, int k);
    vector<float> data;
    vec3 dimension;
    float cell_width;
    float &operator()(int i, int j, int k);
    vec3 get_position(int i, int j, int k);
    vec3 offset;
    vec3 overall_size;
    void replace(int i, int j, int k, float val);
};

class MACGrid {
private:

public:
    MACGrid();
    MACGrid(vec3 dim, Grid u, Grid v, Grid w, Grid p, float width);
    vec3 dim;
    Grid gridU;
    Grid gridV;
    Grid gridW;
    Grid gridP;
    float cell_width;
    void velocity_to_grid(vec3 part_pos, vec3 part_vel);
    vec3 interpolate_velocity(vec3 &pos);
    vec3 check_grid_data();
};


#endif // GRID_H
