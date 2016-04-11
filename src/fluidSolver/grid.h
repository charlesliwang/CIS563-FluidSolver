#ifndef GRID_H
#define GRID_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../geom/geom.hpp"
#include <math.h>
#include <Eigen/Sparse>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

using namespace std;

class Grid {
private:

public:
    Grid();
    Grid(int i, int j, int k);
    vector<float> data;
    vector<float> old_data;
    vector<int> num_infl;
    vec3 dim;
    float cell_width;
    float &operator()(int i, int j, int k);
    int &influences(int i, int j, int k);
    vec3 get_position(int i, int j, int k);
    vec3 offset;
    vec3 overall_size;
    void replace(int i, int j, int k, float val);
    void clear_grid(float v);
    void save_old_data();
    float interpolate(vec3 pos, vec3 grid_center, vec3 idx);
    int grid_to_array(int i, int j, int k);
};

class MACGrid {
private:

public:
    MACGrid();
    MACGrid(vec3 size, vec3 dim, Grid u, Grid v, Grid w, Grid p, Grid t, float width);
    vec3 dim;
    vec3 size;
    Grid gridU;
    Grid gridV;
    Grid gridW;
    Grid oldU;
    Grid oldV;
    Grid oldW;
    Grid gridP;
    Grid gridType;
    Grid gridDiv;
    float cell_width;
    void save_old_data();
    void velocity_to_grid(vec3 part_pos, vec3 part_vel);
    void adjust_grid_velocities();
    vec3 interpolate_velocity(vec3 &pos);
    vec3 check_grid_data();
    void clear_cell_type();
    vec3 get_cell_index(vec3 pos);
    void mark_fluid(vec3 pos);
    void enforce_boundary_conditions();
    void extrapolate_velocities();
    void clear_grids();
    void add_grav();
    Eigen::SparseMatrix<double> get_matrix_A();
    Eigen::VectorXd get_neg_div();
    void pressure_solve();
    void set_solid_bounds();
    void pressure_update();
};


#endif // GRID_H
