#include "grid.h"


Grid::Grid() {}

Grid::Grid(int i, int j, int k) {
    dimension = vec3(i,j,k);
    int n = 0;
    while (n < i*j*k) {
        data.push_back(0.0f);
        n++;
    }
}

float &Grid::operator()(int i, int j, int k)
{
    return data[k*dimension.x*dimension.y + j*dimension.x + i];
}

void Grid::replace(int i, int j, int k, float val) {
    data[k*dimension.x*dimension.y + j*dimension.x + i] = val;
}

vec3 Grid::get_position(int i, int j, int k)
{
    //for now, assume all cells are cubes
    //cout << "offset: " << offset.x << " " << offset.y << " " << offset.z << endl;
    float width = cell_width;
    return offset + vec3(i*width,j*width,k*width) - (0.5f*overall_size);
}

MACGrid::MACGrid() {}

MACGrid::MACGrid(vec3 dim, Grid u, Grid v, Grid w, Grid p, float width)
{
    this->dim = dim;
    gridU = u;
    gridV = v;
    gridW = w;
    gridP = p;
    gridU.cell_width = width;
    gridV.cell_width = width;
    gridW.cell_width = width;
    gridP.cell_width = width;
}

void MACGrid::velocity_to_grid(vec3 part_pos, vec3 part_vel)
{

    vec3 new_pos = part_pos + dim/2.0f;
    float x_grid_units = new_pos.x * cell_width;
    float y_grid_units = new_pos.y * cell_width;
    float z_grid_units = new_pos.z * cell_width;
    vec3 idx = vec3(int(x_grid_units),int(y_grid_units),int(z_grid_units));
    //cout << "idx test: " << idx.x << " " << idx.y << " " << idx.z << endl;
    //GRID U
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            for (int k = -1; k < 2; k++) {
                vec3 u_pos = gridU.get_position(idx[0]+i,idx[1]+j,idx[2]+k);
                float dist = distance(u_pos,part_pos);
                gridU(idx[0]+i,idx[1]+j,idx[2]+k) += part_vel.x/dist;
            }
        }
    }
    //GRID V
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            for (int k = -1; k < 2; k++) {
                vec3 u_pos = gridV.get_position(idx[0]+i,idx[1]+j,idx[2]+k);
                float dist = distance(u_pos,part_pos);
                gridV(idx[0]+i,idx[1]+j,idx[2]+k) += part_vel.y/dist;
            }
        }
    }
    //GRID W
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            for (int k = -1; k < 2; k++) {
                vec3 u_pos = gridW.get_position(idx[0]+i,idx[1]+j,idx[2]+k);
                float dist = distance(u_pos,part_pos);
                gridW(idx[0]+i,idx[1]+j,idx[2]+k) += part_vel.z/dist;
            }
        }
    }
}

vec3 MACGrid::check_grid_data() {
    //ASSUMING 5x5x5
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
//                cout << i << " " << j << " " << k << endl;
//                cout << gridW(i,j,k) << endl;
            }
        }
    }
}

