//
//  scene.hpp
//  Thanda

#include <json/json.h>
#include <iostream>
#include <fstream>
#include "../fluidSolver/fluidSolver.hpp"
#include "../fluidSolver/flipsolver.h"
#include "../geom/geom.hpp"
using namespace std;
using namespace Json;

class Scene {
private:
public:
    Scene();
    FlipSolver *fluid_solver;
    Cube *bounds;
    void read_json();

};
