//
//  scene.cpp
//  Thanda

#include "scene.hpp"

Scene::Scene() {

}

void Scene::read_json() {
    bool file_not_loaded = true;
    while(file_not_loaded) {
        string json_name = "scene.json";

        cout << "Please enter json directory: ";
        getline (cin, json_name);
        Value root;
        Reader reader;
        std::ifstream json_stream (json_name, std::ifstream::binary);
        if(reader.parse(json_stream,root,false)) {
            file_not_loaded = false;
            Value container_size = root.removeMember("containerDim");
            float cx = container_size.removeMember("scaleX").asFloat();
            float cy = container_size.removeMember("scaleY").asFloat();
            float cz = container_size.removeMember("scaleZ").asFloat();
            Value particles_size = root.removeMember("particleDim");
            float px = particles_size.removeMember("boundX").asFloat();
            float py = particles_size.removeMember("boundY").asFloat();
            float pz = particles_size.removeMember("boundZ").asFloat();
            float particle_separation = root.removeMember("particleSeparation").asFloat();
            fluid_solver = new FlipSolver(cx,cy,cz,px,py,pz,particle_separation);
            bounds = new Cube();
            bounds->setBounds(fluid_solver->get_container_bounds());
            fluid_solver->init();
            //vec3 test = fluid_solver->mac_grid.gridP.get_position(4,2,3);
//            fluid_solver->mac_grid.check_grid_data();
//            fluid_solver->store_particle_velocity_to_grid();
//            fluid_solver->update_grid_cell_types();
//            fluid_solver->mac_grid.enforce_boundary_conditions();
//            fluid_solver->mac_grid.extrapolate_velocities();
//            fluid_solver->mac_grid.check_grid_data();
//            vec3 test_pos(2.0f,0,0);
//            vec3 test_interp = fluid_solver->interpolate_velocity(test_pos,fluid_solver->mac_grid);
//            cout << test_interp.x << " " << test_interp.y << " " << test_interp.z << endl;
            //cout << "test " << test.x << " " << test.y << " " << test.z << endl;

            /* TESTING CONVERTING INDEX TO WORLD SPACE */

//            vec3 part_pos(2,0,1);
//            vec3 new_pos = part_pos + vec3(5.0f,5.0f,5.0f)/2.0f;
//            float x_grid_units = new_pos.x;
//            float y_grid_units = new_pos.y;
//            float z_grid_units = new_pos.z;
//            vec3 idx = vec3(int(x_grid_units),int(y_grid_units),int(z_grid_units));
//            cout << "test2 " << idx.x << " " << idx.y << " " << idx.z << endl;

        } else {
            cout << "Invalid path, please try again \n";
        }
    }
}
