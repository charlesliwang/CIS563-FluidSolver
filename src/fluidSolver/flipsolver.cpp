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

void FlipSolver::update()
{
    update_colors();
    mac_grid.save_old_data();
    mac_grid.clear_grids();
    update_grid_cell_types();
    store_particle_velocity_to_grid();
    mac_grid.add_grav();
    mac_grid.enforce_boundary_conditions();
    mac_grid.pressure_solve();
    mac_grid.pressure_update();
    mac_grid.extrapolate_velocities();
    update_particle_velocities();
    update_particle_positions();
    boundary_collisions();
    //mac_grid.check_grid_data();

}

void FlipSolver::construct_mac_grid(int i, int j, int k) {
    float off_x = (float)container_size.x/((float)i*2.0f);
    float off_y = (float)container_size.x/((float)i*2.0f);
    float off_z = (float)container_size.x/((float)i*2.0f);
    Grid u(i+1,j,k);
    u.offset = vec3(0,off_y,off_z);
    u.overall_size = container_size;
    Grid v(i,j+1,k);
    v.offset = vec3(off_x,0,off_z);
    v.overall_size = container_size;
    Grid w(i,j,k+1);
    w.offset = vec3(off_x,off_y,0);
    w.overall_size = container_size;
    Grid p(i,j,k);
    p.offset = vec3(off_x,off_y,off_z);
    p.overall_size = container_size;
    Grid t(i,j,k);
    t.offset = vec3(off_x,off_y,off_z);
    t.overall_size = vec3(i,j,k);
    mac_grid = MACGrid(container_size,vec3(i,j,k),u,v,w,p,t,container_size.x/i);
    mac_grid.cell_width = container_size.x/float(i);
}

void FlipSolver::init() {
    //default resolution
    int i = 7;
    int j = 7;
    int k = 7;
    construct_mac_grid(i,j,k);
    create_particles();
}

void FlipSolver::store_particle_velocity_to_grid() {
    for(int i = 0; i < particles->size(); i++) {
        vec3 part_pos = particles->at(i);
        vec3 part_vel = particles_id.at(i).vel;
        mac_grid.velocity_to_grid(part_pos,part_vel);
    }
    mac_grid.adjust_grid_velocities();
}

vec3 FlipSolver::interpolate_velocity(const vec3 &pos, MACGrid &grid) {
    vec3 new_pos = pos + grid.dim/2.0f;
    float x_grid_units = new_pos.x * grid.cell_width;
    float y_grid_units = new_pos.y * grid.cell_width;
    float z_grid_units = new_pos.z * grid.cell_width;
    vec3 idx = vec3(int(x_grid_units),int(y_grid_units),int(z_grid_units));
    vec3 grid_pos = grid.gridP.get_position(idx[0],idx[1],idx[2]);
    float x = grid.gridU.interpolate(pos,grid_pos,idx);
    float y = grid.gridV.interpolate(pos,grid_pos,idx);
    float z = grid.gridW.interpolate(pos,grid_pos,idx);
    //cout << x << " " << y << " " << " " << z << endl;
    return vec3(x,y,z);
}

vec3 FlipSolver::interpolate_old_velocity(const vec3 &pos, MACGrid &grid)
{
    vec3 new_pos = pos + grid.dim/2.0f;
    float x_grid_units = new_pos.x * grid.cell_width;
    float y_grid_units = new_pos.y * grid.cell_width;
    float z_grid_units = new_pos.z * grid.cell_width;
    vec3 idx = vec3(int(x_grid_units),int(y_grid_units),int(z_grid_units));
    vec3 grid_pos = grid.gridP.get_position(idx[0],idx[1],idx[2]);
    float x = grid.oldU.interpolate(pos,grid_pos,idx);
    float y = grid.oldV.interpolate(pos,grid_pos,idx);
    float z = grid.oldW.interpolate(pos,grid_pos,idx);
    return vec3(x,y,z);
}

void FlipSolver::update_grid_cell_types()
{
    mac_grid.clear_cell_type();
    for(vec3 pos : *particles) {
        mac_grid.mark_fluid(pos);
    }
    mac_grid.set_solid_bounds();
}

void FlipSolver::boundary_collisions()
{
    float cell_width = mac_grid.cell_width;
    for(int i = 0; i < particles->size(); i++) {
        bool update = false;
        vec3 new_velocity = particles_id.at(i).vel;
        vec3 clamped_position = particles->at(i);
        if(particles->at(i).x < mac_grid.size.x*-0.5f + cell_width) {
            new_velocity.x = -0.1f*new_velocity.x;
            clamped_position.x = mac_grid.size.x*-0.5f + cell_width;
            update = true;
        }
        if(particles->at(i).x > mac_grid.size.x*0.5f - cell_width) {
            new_velocity.x = -0.1f*new_velocity.x;
            clamped_position.x = mac_grid.size.x*0.5f - cell_width;
            update = true;
        }
        if(particles->at(i).y < mac_grid.size.y*-0.5f + cell_width) {
            new_velocity.y = -0.1f*new_velocity.y;
            clamped_position.y = mac_grid.size.y*-0.5f + cell_width;
            update = true;
        }
        if(particles->at(i).y > mac_grid.size.y*0.5f - cell_width) {
            new_velocity.y = -0.1f*new_velocity.y;
            clamped_position.y = mac_grid.size.y*0.5f - cell_width;
            update = true;
        }
        if(particles->at(i).z < mac_grid.size.z*-0.5f + cell_width) {
            new_velocity.z = -0.1f*new_velocity.z;
            clamped_position.z = mac_grid.size.z*-0.5f + cell_width;
            update = true;
        }
        if(particles->at(i).z > mac_grid.size.z*0.5f - cell_width) {
            new_velocity.z = -0.1f*new_velocity.z;
            clamped_position.z = mac_grid.size.z*0.5f - cell_width;
            update = true;
        }
        if(update) {
            particles->at(i) = clamped_position;
            //particles_id.at(i).vel = new_velocity;
        }
    }
}

vec3 FlipSolver::get_picflip_advect(int i)
{
    vec3 new_vel = interpolate_velocity(particles->at(i),mac_grid);
    vec3 old_grid_vel = interpolate_old_velocity(particles->at(i),mac_grid);
    vec3 pic = new_vel*0.05f;
    vec3 flip = (particles_id.at(i).vel + (new_vel-old_grid_vel)) * 0.95f;

//    particles_id.at(i).vel = pic + flip;
    return pic + flip;
    //return pic*20.0f;
}

void FlipSolver::update_particle_velocities()
{
    for(int i = 0; i < particles->size(); i++) {
        particles_id.at(i).vel = get_picflip_advect(i);
    }
}

void FlipSolver::update_particle_positions()
{
    for(int i = 0; i < particles->size(); i++) {
        particles->at(i) += particles_id.at(i).vel;
        vec3 test = particles->at(i);
    }
}

void FlipSolver::update_colors() {
    for(int i = 0; i < particles_id.size(); i++) {
        colors->at(i) = vec4(fabsf(particles_id.at(i).vel.x)*20.0f,
                             fabsf(particles_id.at(i).vel.y)*20.0f,
                             fabsf(particles_id.at(i).vel.z)*20.0f,1.0f);
    }
}
