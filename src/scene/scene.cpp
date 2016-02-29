//
//  scene.cpp
//  Thanda

#include "scene.hpp"

Scene::Scene() {

}

void Scene::read_json() {
    bool file_not_loaded = true;
    while(file_not_loaded) {
        string json_name = "";

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
            fluid_solver = new FluidSolver(cx,cy,cz,px,py,pz,particle_separation);
            bounds = new Cube();
            bounds->setBounds(fluid_solver->get_container_bounds());
            fluid_solver->create_particles();
        } else {
            cout << "Invalid path, please try again \n";
        }
    }
}
