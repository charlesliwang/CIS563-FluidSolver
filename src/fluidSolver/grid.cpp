#include "grid.h"


Grid::Grid() {}

Grid::Grid(int i, int j, int k) {
    dim = vec3(i,j,k);
    clear_grid(0);
}

float &Grid::operator()(int i, int j, int k)
{
    return data[k*dim.x*dim.y + j*dim.x + i];
}

int &Grid::influences(int i, int j, int k)
{
    return num_infl[k*dim.x*dim.y + j*dim.x + i];
}

void Grid::replace(int i, int j, int k, float val) {
    data[k*dim.x*dim.y + j*dim.x + i] = val;
}

void Grid::clear_grid(float v)
{
    data.clear();
    num_infl.clear();
    int n = 0;
    while (n < dim.x*dim.y*dim.z) {
        data.push_back(v);
        num_infl.push_back(1);
        n++;
    }
}

void Grid::save_old_data()
{
    old_data = data;
}

vec3 Grid::get_position(int i, int j, int k)
{
    //for now, assume all cells are cubes
    //cout << "offset: " << offset.x << " " << offset.y << " " << offset.z << endl;
    float width = cell_width;
    return offset + vec3(i*width,j*width,k*width) - (0.5f*overall_size);
}

MACGrid::MACGrid() {}

MACGrid::MACGrid(vec3 size, vec3 dim, Grid u, Grid v, Grid w, Grid p, Grid t, float width)
{
    this->size = size;
    this->dim = dim;
    gridU = u;
    gridV = v;
    gridW = w;
    gridP = p;
    gridType = t;
    gridU.cell_width = width;
    gridV.cell_width = width;
    gridW.cell_width = width;
    gridP.cell_width = width;
}

void MACGrid::save_old_data()
{
    oldU = gridU;
    oldV = gridV;
    oldW = gridW;
}

void MACGrid::velocity_to_grid(vec3 part_pos, vec3 part_vel)
{
    vec3 new_pos = part_pos + size/2.0f;
    float x_grid_units = new_pos.x * cell_width;
    float y_grid_units = new_pos.y * cell_width;
    float z_grid_units = new_pos.z * cell_width;
    int x = int(x_grid_units);
    int y = int(y_grid_units);
    int z = int(z_grid_units);
    //cout << "idx test: " << idx.x << " " << idx.y << " " << idx.z << endl;
    vector<float> temp = gridV.data;
    //GRID U
    for (int i = -1; i < 1; i++) {
        for (int j = -1; j < 1; j++) {
            for (int k = -1; k < 1; k++) {
                vec3 u_pos = gridU.get_position(x+i,y+j,z+k);
                float dist = distance(u_pos,part_pos);
                gridU(x+i,y+j,z+k) += part_vel.x/dist;
                gridU.influences(x+i,y+j,z+k)++;
            }
        }
    }
    gridV.data = temp;
    //GRID V
    for (int i = -1; i < 1; i++) {
        for (int j = -1; j < 1; j++) {
            for (int k = -1; k < 1; k++) {
                vec3 v_pos = gridV.get_position(x+i,y+j,z+k);
                float dist = distance(v_pos,part_pos);
                //cout << "before: " << gridV(x+i,y+j,z+k) << endl;
                gridV(x+i,y+j,z+k) += part_vel.y/dist;
                gridV.influences(x+i,y+j,z+k)++;
                //cout << "after: " << gridV(x+i,y+j,z+k) << endl;
            }
        }
    }
    //GRID W
    for (int i = -1; i < 1; i++) {
        for (int j = -1; j < 1; j++) {
            for (int k = 1; k < 1; k++) {
                vec3 w_pos = gridW.get_position(x+i,y+j,z+k);
                float dist = distance(w_pos,part_pos);
                gridW(x+i,y+j,z+k) += part_vel.z/dist;
                gridW.influences(x+i,y+j,z+k)++;
            }
        }
    }
}

void MACGrid::adjust_grid_velocities()
{
    for(int i = 0; i < dim.x+1; i++) {
        for(int j = 0; j < dim.y; j++) {
            for(int k = 0; k< dim.z; k++) {
                gridU(i,j,k) /= float(gridU.influences(i,j,k));
            }
        }
    }
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y+1; j++) {
            for(int k = 0; k< dim.z; k++) {
                gridV(i,j,k) /= float(gridV.influences(i,j,k));
            }
        }
    }
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y; j++) {
            for(int k = 0; k< dim.z+1; k++) {
                gridW(i,j,k) /= float(gridW.influences(i,j,k));
            }
        }
    }
}



vec3 MACGrid::check_grid_data() {
    //ASSUMING 5x5x5
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                cout << i << " " << j << " " << k << endl;
                cout << gridType(i,j,k) << endl;
                cout << gridU(i,j,k) << endl;
            }
        }
    }
}

vec3 MACGrid::get_cell_index(vec3 pos) {
    vec3 new_pos = (pos + size/2.0f) * cell_width;
    return new_pos;
}

void MACGrid::clear_cell_type()
{
    gridType.clear_grid(0);
}

void MACGrid::mark_fluid(vec3 pos) {
    vec3 index = get_cell_index(pos);
    gridType(index.x,index.y,index.z) = -1.0f;
}

void MACGrid::enforce_boundary_conditions()
{
    for(int y = 0; y < dim.y; y++) {
        for(int z = 0; z < dim.z; z++) {
            gridU(dim.x,y,z) = 0;
            gridU(0,y,z) = 0;
        }
    }
    for(int x = 0; x < dim.x; x++) {
        for(int z = 0; z < dim.z; z++) {
            gridV(x,dim.y,z) = 0;
            gridV(x,0,z) = 0;
        }
    }
    for(int y = 0; y < dim.y; y++) {
        for(int x = 0; x < dim.x; x++) {
            gridW(x,y,dim.z) = 0;
            gridW(x,y,0) = 0;
        }
    }
}

void MACGrid::extrapolate_velocities()
{
    for (int i = 0; i < dim.x; i++) {
        for (int j = 0; j < dim.y; j++) {
            for (int k = 0; k < dim.z; k++) {
                if(gridType(i,j,k) == 0) {
                    float u = 0;
                    float v = 0;
                    float w = 0;
                    int n = 0;
                    if(i > 0 && gridType(i-1,j,k) < 0) {
                        u += gridU(i-1,j,k);
                        v += gridV(i-1,j,k);
                        w += gridW(i-1,j,k);
                        n++;
                    } if(i < dim.x -1 && gridType(i+1,j,k) < 0) {
                        u += gridU(i+1,j,k);
                        v += gridV(i+1,j,k);
                        w += gridW(i+1,j,k);
                        n++;
                    } if(j > 0 && gridType(i,j-1,k) < 0) {
                        u += gridU(i,j-1,k);
                        v += gridV(i,j-1,k);
                        w += gridW(i,j-1,k);
                        n++;
                    } if(i < dim.y -1 && gridType(i,j+1,k) < 0) {
                        u += gridU(i,j+1,k);
                        v += gridV(i,j+1,k);
                        w += gridW(i,j+1,k);
                        n++;
                    } if(k > 0 && gridType(i,j,k-1) < 0) {
                        u += gridU(i,j,k-1);
                        v += gridV(i,j,k-1);
                        w += gridW(i,j,k-1);
                        n++;
                    } if(i < dim.z -1 && gridType(i,j,k+1) < 0) {
                        u += gridU(i,j,k+1);
                        v += gridV(i,j,k+1);
                        w += gridW(i,j,k+1);
                        n++;
                    }
                    if (n == 0) n = 1.0f;
                    gridU(i,j,k) = u/n;
                    gridV(i,j,k) = v/n;
                    gridW(i,j,k) = w/n;
                }

            }
        }
    }

}

void MACGrid::clear_grids_add_grav()
{
    clear_cell_type();
    gridU.clear_grid(0);
    gridV.clear_grid(-2.0f);
    gridW.clear_grid(0);
    gridP.clear_grid(0);
}
