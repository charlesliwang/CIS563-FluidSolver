#include "grid.h"
typedef Eigen::Triplet<double> trip;

Grid::Grid() {}



Grid::Grid(int i, int j, int k) {
    dim = vec3(i,j,k);
    clear_grid(0);
}

float &Grid::operator()(int i, int j, int k)
{
    return data[k*dim.x*dim.y + j*dim.x + i];
}

int Grid::grid_to_array(int i, int j, int k) {
    return k*dim.x*dim.y + j*dim.x + i;
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
        num_infl.push_back(0);
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

float Grid::interpolate(vec3 pos, vec3 grid_center, vec3 idx) {
    vector<float> u_vels;
    vector<float> u_weights;
    float i = idx.x;
    float j = idx.y;
    float k = idx.z;
    u_vels.push_back(data[grid_to_array(i,j,k)]);
    u_weights.push_back(distance(get_position(i,j,k), pos));
    if(i < dim.x) {
        u_vels.push_back(data[i+1,j,k]);
        u_weights.push_back(distance(get_position(i+1,j,k), pos));
    }
    if(pos.y > grid_center.y - 0.0001) {
        if (j < dim.y) {
            u_vels.push_back(data[i,j+1,k]);
            u_weights.push_back(distance(get_position(i,j+1,k),pos));
            if(i < dim.x) {
                u_vels.push_back(data[i+1,j+1,k]);
                u_weights.push_back(distance(get_position(i+1,j+1,k),pos));
            }
        }
    } else {
        if (j > 0) {
            u_vels.push_back(data[i,j-1,k]);
            u_weights.push_back(distance(get_position(i,j-1,k),pos));
            if(i < dim.x) {
                u_vels.push_back(data[i+1,j-1,k]);
                u_weights.push_back(distance(get_position(i+1,j-1,k),pos));
            }
        }
    } if (pos.z > grid_center.z  - 0.0001) {
        if (k < dim.z) {
            u_vels.push_back(data[i,j,k+1]);
            u_weights.push_back(distance(get_position(i,j,k+1),pos));
            if(i < dim.x) {
                u_vels.push_back(data[i+1,j,k+1]);
                u_weights.push_back(distance(get_position(i+1,j,k+1),pos));
            }
        }
    } else {
        if (k > 0) {
            u_vels.push_back(data[i,j,k-1]);
            u_weights.push_back(distance(get_position(i,j,k-1),pos));
            if(i < dim.x) {
                u_vels.push_back(data[i+1,j,k-1]);
                u_weights.push_back(distance(get_position(i+1,j,k-1),pos));
            }
        }
    } if (pos.z > grid_center.z  - 0.0001 && pos.y > grid_center.y  - 0.0001) {
        if (k < dim.z && j < dim.y) {
            u_vels.push_back(data[i,j+1,k+1]);
            u_weights.push_back(distance(get_position(i,j,k+1),pos));
            if(i < dim.x) {
                u_vels.push_back(data[i+1,j+1,k+1]);
                u_weights.push_back(distance(get_position(i+1,j,k+1),pos));
            }
        }
    } else if (pos.z < grid_center.z - 0.0001 && pos.y < grid_center.y  - 0.0001){
        if (k > 0 && j > 0) {
            u_vels.push_back(data[i,j-1,k-1]);
            u_weights.push_back(distance(get_position(i,j-1,k-1),pos));
            if(i < dim.x) {
                u_vels.push_back(data[i+1,j-1,k-1]);
                u_weights.push_back(distance(get_position(i+1,j-1,k-1),pos));
            }
        }
    }
    float total_weight = 0;
    for(float weight : u_weights) {
        total_weight += weight;
    }
    float interp_vel = 0;
    for(int incr = 0; incr < u_vels.size(); incr++) {
        interp_vel += u_vels[incr]*(u_weights[incr]/total_weight);
    }
    return interp_vel;
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
                gridU(x+i,y+j,z+k) += part_vel.x*dist;
                gridU.influences(x+i,y+j,z+k) += dist;
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
                gridV(x+i,y+j,z+k) += part_vel.y*dist;
                gridV.influences(x+i,y+j,z+k) += dist;
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
                gridW(x+i,y+j,z+k) += part_vel.z*dist;
                gridW.influences(x+i,y+j,z+k) += dist;
            }
        }
    }
}

void MACGrid::adjust_grid_velocities()
{
    for(int i = 0; i < dim.x+1; i++) {
        for(int j = 0; j < dim.y; j++) {
            for(int k = 0; k < dim.z; k++) {
                if (gridU.influences(i,j,k) > 0 ) {
                    gridU(i,j,k) /= float(gridU.influences(i,j,k));
                }

            }
        }
    }
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y+1; j++) {
            for(int k = 0; k< dim.z; k++) {
                if (gridV.influences(i,j,k) > 0 ) {
                    gridV(i,j,k) /= float(gridV.influences(i,j,k));
                }
            }
        }
    }
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y; j++) {
            for(int k = 0; k< dim.z+1; k++) {
                if (gridW.influences(i,j,k) > 0 ) {
                    gridW(i,j,k) /= float(gridW.influences(i,j,k));
                }
            }
        }
    }
}



vec3 MACGrid::check_grid_data() {
    //ASSUMING 5x5x5
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                cout << i << " " << j << " " << k << endl;
                cout << gridType(i,j,k) << endl;
                cout << gridV(i,j,k) << endl;
            }
        }
    }
}

vec3 MACGrid::get_cell_index(vec3 pos) {
    vec3 new_pos = (pos + size/2.0f) / cell_width;
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

void MACGrid::set_solid_bounds() {
    for(int y = 0; y < dim.y; y++) {
        for(int z = 0; z < dim.z; z++) {
            gridType(0,y,z) = 1;
            gridType(dim.x - 1,y,z) = 1;
        }
    }
    for(int y = 0; y < dim.y; y++) {
        for(int x = 0; x < dim.x; x++) {
            gridType(x,y,0) = 1;
            gridType(x,y, dim.z - 1) = 1;

        }
    }
    for(int x = 0; x < dim.x; x++) {
        for(int z = 0; z < dim.z; z++) {
            gridType(x,0,z) = 1;
            gridType(x,dim.y - 1,z) = 1;

        }
    }
}

void MACGrid::pressure_update()
{
    float scale = 0.3/(cell_width);
    for(int i = 1; i < dim.x; i++) {
        for(int j = 1; j < dim.y; j++) {
            for(int k = 1; k < dim.z; k++) {
                if(gridType(i-1,j,k) < 0 ||
                        gridType(i,j,k) < 0) {
                    if(gridType(i-1,j,k) > 0 ||
                            gridType(i,j,k) > 0) {
                        gridU(i,j,k) = 0;
                    } else {
                        gridU(i,j,k) -= scale*(gridP(i,j,k)-gridP(i-1,j,k));
                    }
                }
                if(gridType(i,j-1,k) < 0 ||
                     gridType(i,j,k) < 0) {
                    if(gridType(i,j-1,k) > 0 ||
                         gridType(i,j,k) > 0) {
                        gridV(i,j,k) = 0;
                    } else {
                        gridV(i,j,k) -= scale*(gridP(i,j,k)-gridP(i,j-1,k));
                    }
                }
                if(gridType(i,j,k-1) < 0 ||
                     gridType(i,j,k) < 0) {
                    if(gridType(i,j,k-1) > 0 ||
                         gridType(i,j,k) > 0) {
                        gridV(i,j,k) = 0;
                    } else {
                        gridW(i,j,k) -= scale*(gridP(i,j,k)-gridP(i,j-1,k));
                    }
                }
            }
        }
    }
}


void MACGrid::enforce_boundary_conditions()
{
    for(int y = 0; y < dim.y; y++) {
        for(int z = 0; z < dim.z; z++) {
            gridU(dim.x,y,z) = 0;
            gridU(0,y,z) = 0;
            gridU(1,y,z) = 0;
            gridU(dim.x-1,y,z) = 0;
//            gridV(dim.x-1,y,z) = 0;
//            gridV(0,y,z) = 0;
//            gridW(dim.x-1,y,z) = 0;
//            gridW(0,y,z) = 0;
        }
    }
    for(int x = 0; x < dim.x; x++) {
        for(int z = 0; z < dim.z; z++) {
            gridV(x,dim.y,z) = 0;
            gridV(x,0,z) = 0;
            gridV(x,1,z) = 0;
            gridV(x,dim.y-1,z) = 0;
//            gridU(x,dim.y-1,z) = 0;
//            gridU(x,0,z) = 0;
//            gridW(x,dim.y-1,z) = 0;
//            gridW(x,0,z) = 0;
        }
    }
    for(int y = 0; y < dim.y; y++) {
        for(int x = 0; x < dim.x; x++) {
            gridW(x,y,dim.z) = 0;
            gridW(x,y,0) = 0;
            gridW(x,y,1) = 0;
            gridW(x,y,dim.z-1) = 0;
//            gridU(x,y,dim.z-1) = 0;
//            gridU(x,y,0) = 0;
//            gridV(x,y,dim.z-1) = 0;
//            gridV(x,y,0) = 0;
        }
    }
}

void MACGrid::extrapolate_velocities()
{
    for (int i = 0; i < dim.x; i++) {
        for (int j = 0; j < dim.y; j++) {
            for (int k = 0; k < dim.z; k++) {
                if(gridType(i,j,k) >= 0) {
                    float u = 0;
                    float v = 0;
                    float w = 0;
                    int n = 0;
                    if(i > 0 && gridType(i-1,j,k) < 0) {
                        u += gridU(i-1,j,k);
                        v += gridV(i-1,j,k);
                        w += gridW(i-1,j,k);
                        n++;
                    } if(i < dim.x - 1 && gridType(i+1,j,k) < 0) {
                        u += gridU(i+1,j,k);
                        v += gridV(i+1,j,k);
                        w += gridW(i+1,j,k);
                        n++;
                    } if(j > 0 && gridType(i,j-1,k) < 0) {
                        u += gridU(i,j-1,k);
                        v += gridV(i,j-1,k);
                        w += gridW(i,j-1,k);
                        n++;
                    } if(j < dim.y - 1 && gridType(i,j+1,k) < 0) {
                        u += gridU(i,j+1,k);
                        v += gridV(i,j+1,k);
                        w += gridW(i,j+1,k);
                        n++;
                    } if(k > 0 && gridType(i,j,k-1) < 0) {
                        u += gridU(i,j,k-1);
                        v += gridV(i,j,k-1);
                        w += gridW(i,j,k-1);
                        n++;
                    } if(k < dim.z - 1 && gridType(i,j,k+1) < 0) {
                        u += gridU(i,j,k+1);
                        v += gridV(i,j,k+1);
                        w += gridW(i,j,k+1);
                        n++;
                    }
                    if (n == 0) n = 1.0f;
                    gridU(i,j,k) = u/float(n);
                    gridV(i,j,k) = v/float(n);
                    gridW(i,j,k) = w/float(n);
                }

            }
        }
    }

}

void MACGrid::clear_grids()
{
    clear_cell_type();
    gridU.clear_grid(0);
    gridV.clear_grid(0);
    gridW.clear_grid(0);
    gridP.clear_grid(0);
}

void MACGrid::add_grav()
{
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y + 1; j++) {
            for(int k = 0; k < dim.z; k++) {
                gridV(i,j,k) = -9.8f*0.3f;
            }
        }
    }
}

Eigen::SparseMatrix<double> MACGrid::get_matrix_A()
{
    //find number of fluid or empty (<= 0) neighbors
    /*for each of the 6 coefficients:
     * mark as -1 for fluid
     * mark as 0 for other
    */
    double delta_t = 0.3f;
    double rho = 1.0f;
    double delta_x = cell_width * cell_width;
    vector<trip> coefs;
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y; j++) {
            for(int k = 0; k < dim.z; k++) {
                if(gridType(i,j,k) < 0) {
                    bool flag = false;
                    int idx_row = gridP.grid_to_array(i,j,k);
                    float coef = 0;
                    if(gridType(i - 1,j,k) <= 0.1) {
                        if(flag) cout << " 1 " << endl;
                        coef += 1.0f;
                        if(gridType(i - 1,j,k) < 0) {
                            double coef_temp = -1.0f;
                            double idx_col = gridP.grid_to_array(i - 1,j,k);
                            coefs.push_back(trip(idx_row,idx_col,coef_temp*(delta_t/(rho*delta_x))));
                        }
                    }
                    if(gridType(i + 1,j,k) <= 0.1) {
                        if(flag) cout << " 2 " << endl;
                        coef += 1.0f;
                        if(gridType(i + 1,j,k) < 0) {
                            double coef_temp = -1.0f;
                            double idx_col = gridP.grid_to_array(i + 1,j,k);
                            coefs.push_back(trip(idx_row,idx_col,coef_temp*(delta_t/(rho*delta_x))));
                        }
                    }
                    if(gridType(i,j - 1,k) <= 0.1) {
                        if(flag) cout << " 3 " << endl;
                        coef += 1.0f;
                        if(gridType(i,j - 1,k) < 0) {
                            double coef_temp = -1.0f;
                            double idx_col = gridP.grid_to_array(i,j - 1,k);
                            coefs.push_back(trip(idx_row,idx_col,coef_temp*(delta_t/(rho*delta_x))));
                        }
                    }
                    if(gridType(i,j + 1,k) <= 0.1) {
                        if(flag) cout << " 4 " << endl;
                        coef += 1.0f;
                        if(gridType(i,j + 1,k) < 0) {
                            double coef_temp = -1.0f;
                            double idx_col = gridP.grid_to_array(i,j + 1,k);
                            coefs.push_back(trip(idx_row,idx_col,coef_temp*(delta_t/(rho*delta_x))));
                        }
                    }
                    if(gridType(i,j,k - 1) <= 0.1) {
                        if(flag) cout << " 5 " << endl;
                        coef += 1.0f;
                        if(gridType(i,j, k - 1) < 0) {
                            double coef_temp = -1.0f;
                            double idx_col = gridP.grid_to_array(i,j,k - 1);
                            coefs.push_back(trip(idx_row,idx_col,coef_temp*(delta_t/(rho*delta_x))));
                        }
                    }
                    if(gridType(i,j,k + 1) <= 0.1) {
                        if(flag) cout << " 6 " << endl;
                        coef += 1.0f;
                        if(gridType(i,j, k + 1) < 0) {
                            double coef_temp = -1.0f;
                            double idx_col = gridP.grid_to_array(i,j,k + 1);
                            coefs.push_back(trip(idx_row,idx_col,coef_temp*(delta_t/(rho*delta_x))));
                        }
                    }
                    if(flag) cout << "what: " << coef << endl;
                    coefs.push_back(trip(idx_row,idx_row,coef*(delta_t/(rho*delta_x))));
                }
            }
        }
    }
    int rows = dim.x*dim.y*dim.z;
    Eigen::SparseMatrix<double> m(rows,rows);
    m.setZero();
    m.reserve(Eigen::VectorXi::Constant(rows,rows));
    m.setFromTriplets(coefs.begin(), coefs.end());
    return m;
}

Eigen::VectorXd MACGrid::get_neg_div()
{
    Eigen::VectorXd b(int(dim.x*dim.y*dim.z));
    b.setZero(int(dim.x*dim.y*dim.z));
    for(int i = 0; i < dim.x; i++) {
        for(int j = 0; j < dim.y; j++) {
            for(int k = 0; k < dim.z; k++) {
                if(gridType(i,j,k) < 0) {
                    double x = gridU(i+1,j,k) - gridU(i,j,k);
                    double y = gridV(i,j+1,k) - gridV(i,j,k);
                    double z = gridW(i,j,k+1) - gridW(i,j,k);
                    double neg_div = -1.0f*((x+y+z)/cell_width);
                    b(gridP.grid_to_array(i,j,k)) = neg_div;
                }
            }
        }
    }
    return b;
}

void MACGrid::pressure_solve()
{
    //cout << "pressure?????" << endl;
    int size = dim.x*dim.y*dim.z;
    Eigen::SparseMatrix<double> a(size,size);
    a.setZero();
    a = get_matrix_A();
    //cout << a.size() << endl;
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower | Eigen::Upper> cg(a);
//    for (int k=0; k<a.outerSize(); ++k){
//      for (Eigen::SparseMatrix<double>::InnerIterator it(a,k); it; ++it)
//      {
//        cout <<it.value() << endl;
//        cout << "row " << it.row() << endl;   // row index
//        cout << "col " << it.col() << endl;   // col index (here it is equal to k)
//        cout << "index " << it.index() << endl; // inner index, here it is equal to it.row()
//      }
//    }
    bool not_sym = false;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(a.coeff(i,j) != a.coeff(j,i)) {
                cout << "NOT SYMMETRIC" << endl;
                not_sym = true;
            }
        }
    }
    if(not_sym) {
        cout << "UUGGGHHHH" << endl;
    }
    Eigen::VectorXd b;
    b.setZero(size);
    b = get_neg_div();
//    for (int i = 0; i < dim.x*dim.y*dim.z;i++)
//    {
//        cout << "b: " << b(i) <<endl;
//    }

//    cg.compute(a);
    Eigen::VectorXd p(size);
    p.setZero(size);
    p = cg.solve(b);
//    std::cout << "#iterations:     " << cg.iterations() << std::endl;
//    std::cout << "estimated error: " << cg.error()      << std::endl;
    if(cg.info() != Eigen::Success) {
        cout << "UGH" << endl;
        cout << cg.info() << endl;
    }
    for(int i = 0; i < p.size(); i++) {
        //cout << "?? " << p(i) << endl;
        gridP.data[i] = p(i);
    }
}
