#include "flipsolver.h"

FlipSolver::FlipSolver()
{

}

FlipSolver::FlipSolver(float cx, float cy, float cz, float px, float py, float pz, float separation)
{
    container_size = vec3(cx,cy,cz);
    particles_size = vec3(px,py,pz);
    particle_separation = separation;
    particles = new vector<vec3>();
    colors = new vector<vec4>();
}

void FlipSolver::construct_mac_grid(int i, int j, int k) {

    float off_x = (float)container_size.x/((float)i*2.0f);
    float off_y = (float)container_size.x/((float)i*2.0f);
    float off_z = (float)container_size.x/((float)i*2.0f);
    Grid u(i+1,j,k);
    u.offset = vec3(0,off_y,off_z);
    u.overall_size = vec3(i,j,k);
    Grid v(i,j+1,k);
    v.offset = vec3(off_x,0,off_z);
    v.overall_size = vec3(i,j,k);
    Grid w(i,j,k+1);
    w.offset = vec3(off_x,off_y,0);
    w.overall_size = vec3(i,j,k);
    Grid p(i,j,k);
    p.offset = vec3(off_x,off_y,off_z);
    p.overall_size = vec3(i,j,k);
    mac_grid = MACGrid(container_size,u,v,w,p,container_size.x/i);
    mac_grid.cell_width = container_size.x/i;
}

void FlipSolver::init() {
    //default resolution
    int i = 5;
    int j = 5;
    int k = 5;
    construct_mac_grid(i,j,k);
    create_particles();
}

void FlipSolver::store_particle_velocity_to_grid() {
    for(int i = 0; i < particles->size(); i++) {
        vec3 part_pos = particles->at(i);
        vec3 part_vel = particles_id.at(i).vel;
        mac_grid.velocity_to_grid(part_pos,part_vel);
    }
}

vec3 FlipSolver::interpolate_velocity(const vec3 &pos, MACGrid &grid) {
    vec3 new_pos = pos + grid.dim/2.0f;
    float x_grid_units = new_pos.x * grid.cell_width;
    float y_grid_units = new_pos.y * grid.cell_width;
    float z_grid_units = new_pos.z * grid.cell_width;
    vec3 idx = vec3(int(x_grid_units),int(y_grid_units),int(z_grid_units));
    //U
    vector<float> u_vels;
    vector<float> u_weights;
    u_vels.push_back(grid.gridU(idx[0],idx[1],idx[2]));
    u_weights.push_back(normalize(
                            distance(grid.gridU.get_position(idx[0],idx[1],idx[2]),
                                     pos)));
    u_vels.push_back(grid.gridU(idx[0]+1,idx[1],idx[2]));
    u_weights.push_back(normalize(
                            distance(grid.gridU.get_position(idx[0]+1,idx[1],idx[2]),
                                     pos)));
    if(pos.y > grid.gridP.get_position(idx[0],idx[1],idx[2]).y) {
        u_vels.push_back(grid.gridU(idx[0],idx[1]+1,idx[2]));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0],idx[1]+1,idx[2]),
                                         pos)));
        u_vels.push_back(grid.gridU(idx[0]+1,idx[1]+1,idx[2]));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0]+1,idx[1]+1,idx[2]),
                                         pos)));
    } else {
        u_vels.push_back(grid.gridU(idx[0],idx[1]+1,idx[2]));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0],idx[1]-1,idx[2]),
                                         pos)));
        u_vels.push_back(grid.gridU(idx[0]+1,idx[1]-1,idx[2]));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0]+1,idx[1]-1,idx[2]),
                                         pos)));
    } if (pos.z > grid.gridP.get_position(idx[0],idx[1],idx[2]).z) {
        u_vels.push_back(grid.gridU(idx[0],idx[1],idx[2]+1));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0],idx[1],idx[2]+1),
                                         pos)));
        u_vels.push_back(grid.gridU(idx[0]+1,idx[1],idx[2]+1));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0]+1,idx[1],idx[2]+1),
                                         pos)));
    } else {
        u_vels.push_back(grid.gridU(idx[0],idx[1],idx[2]-1));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0],idx[1],idx[2]-1),
                                         pos)));
        u_vels.push_back(grid.gridU(idx[0]+1,idx[1],idx[2]-1));
        u_weights.push_back(normalize(
                                distance(grid.gridU.get_position(idx[0]+1,idx[1],idx[2]-1),
                                         pos)));
    }
    float total_weight = 0;
    for(float weight : u_weights) {
        total_weight += weight;
    }
    float interp_vel = 0;
    for(int i = 0; i < u_vels.size(); i++) {
        interp_vel += u_vels[i]*(u_weights[i]/total_weight);
    }
    //V
    vector<float> v_vels;
    vector<float> v_weights;
    v_vels.push_back(grid.gridV(idx[0],idx[1],idx[2]));
    v_weights.push_back(normalize(
                            distance(grid.gridV.get_position(idx[0],idx[1],idx[2]),
                                     pos)));
    v_vels.push_back(grid.gridV(idx[0],idx[1]+1,idx[2]));
    v_weights.push_back(normalize(
                            distance(grid.gridV.get_position(idx[0],idx[1]+1,idx[2]),
                                     pos)));
    if(pos.x > grid.gridP.get_position(idx[0],idx[1],idx[2]).x) {
        v_vels.push_back(grid.gridV(idx[0]+1,idx[1],idx[2]));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0]+1,idx[1],idx[2]),
                                         pos)));
        v_vels.push_back(grid.gridV(idx[0]+1,idx[1]+1,idx[2]));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0]+1,idx[1]+1,idx[2]),
                                         pos)));
    } else {
        v_vels.push_back(grid.gridV(idx[0]-1,idx[1],idx[2]));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0]-1,idx[1],idx[2]),
                                         pos)));
        v_vels.push_back(grid.gridV(idx[0]-1,idx[1]+1,idx[2]));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0]-1,idx[1]+1,idx[2]),
                                         pos)));
    } if (pos.z > grid.gridP.get_position(idx[0],idx[1],idx[2]).z) {
        v_vels.push_back(grid.gridV(idx[0],idx[1],idx[2]+1));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0],idx[1],idx[2]+1),
                                         pos)));
        v_vels.push_back(grid.gridV(idx[0],idx[1]+1,idx[2]+1));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0],idx[1]+1,idx[2]+1),
                                         pos)));
    } else {
        v_vels.push_back(grid.gridV(idx[0],idx[1],idx[2]-1));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0],idx[1],idx[2]-1),
                                         pos)));
        v_vels.push_back(grid.gridV(idx[0],idx[1]+1,idx[2]-1));
        v_weights.push_back(normalize(
                                distance(grid.gridV.get_position(idx[0],idx[1]+1,idx[2]-1),
                                         pos)));
    }
    float total_weight_v = 0;
    for(float weight : v_weights) {
        total_weight_v += weight;
    }
    float interp_vel_v = 0;
    for(int i = 0; i < v_vels.size(); i++) {
        interp_vel_v += v_vels[i]*(v_weights[i]/total_weight_v);
    }
    //W
    vector<float> w_vels;
    vector<float> w_weights;
    w_vels.push_back(grid.gridW(idx[0],idx[1],idx[2]));
    w_weights.push_back(normalize(
                            distance(grid.gridW.get_position(idx[0],idx[1],idx[2]),
                                     pos)));
    w_vels.push_back(grid.gridW(idx[0],idx[1],idx[2]+1));
    w_weights.push_back(normalize(
                            distance(grid.gridW.get_position(idx[0],idx[1],idx[2]+1),
                                     pos)));
    if(pos.x > grid.gridP.get_position(idx[0],idx[1],idx[2]).x) {
        w_vels.push_back(grid.gridW(idx[0]+1,idx[1],idx[2]));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0]+1,idx[1],idx[2]),
                                         pos)));
        w_vels.push_back(grid.gridW(idx[0]+1,idx[1],idx[2]+1));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0]+1,idx[1],idx[2]+1),
                                         pos)));
    } else {
        w_vels.push_back(grid.gridW(idx[0]-1,idx[1],idx[2]));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0]-1,idx[1]-1,idx[2]),
                                         pos)));
        w_vels.push_back(grid.gridW(idx[0]-1,idx[1],idx[2]+1));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0]-1,idx[1],idx[2]+1),
                                         pos)));
    } if (pos.y > grid.gridP.get_position(idx[0],idx[1],idx[2]).y) {
        w_vels.push_back(grid.gridW(idx[0],idx[1]+1,idx[2]));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0],idx[1]+1,idx[2]),
                                         pos)));
        w_vels.push_back(grid.gridW(idx[0],idx[1]+1,idx[2]+1));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0],idx[1]+1,idx[2]+1),
                                         pos)));
    } else {
        w_vels.push_back(grid.gridW(idx[0],idx[1]-1,idx[2]));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0],idx[1]-1,idx[2]),
                                         pos)));
        w_vels.push_back(grid.gridW(idx[0],idx[1]-1,idx[2]+1));
        w_weights.push_back(normalize(
                                distance(grid.gridW.get_position(idx[0],idx[1]-1,idx[2]+1),
                                         pos)));
    }
    float total_weight_w = 0;
    for(float weight : w_weights) {
        total_weight_w += weight;
    }
    float interp_vel_w = 0;
    for(int i = 0; i < w_vels.size(); i++) {
        interp_vel_w += w_vels[i]*(w_weights[i]/total_weight_w);
    }
    return vec3(interp_vel,interp_vel_v,interp_vel_w);
}

