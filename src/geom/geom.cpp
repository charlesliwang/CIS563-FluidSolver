//
//  geom.cpp
//  Thanda

#include "geom.hpp"
Geometry::Geometry()
{}


Cube::Cube() {

}

bool Cube::collision(vec3 point)
{
    if(point.x > bounds.x/2.0f || point.x < -bounds.x/2.0f) {
        return true;
    }
    if(point.y > bounds.y/2.0f || point.y < -bounds.y/2.0f) {
        return true;
    }
    if(point.z > bounds.z/2.0f || point.z < -bounds.z/2.0f) {
        return true;
    }
    return false;
}

void Cube::setBounds(vec3 bounds) {
    this->bounds = bounds;
}

vector<vec3> Cube::get_vert_pos() {
    return vert_pos;
}


void Cube::create() {
    vert_pos.clear();
    vert_pos.push_back(vec3(bounds.x/2.0f,bounds.y/2.0f,bounds.z/2.0f));
    vert_pos.push_back(vec3(bounds.x/2.0f,-bounds.y/2.0f,bounds.z/2.0f));
    vert_pos.push_back(vec3(bounds.x/2.0f,-bounds.y/2.0f,-bounds.z/2.0f));
    vert_pos.push_back(vec3(bounds.x/2.0f,bounds.y/2.0f,-bounds.z/2.0f));
    vert_pos.push_back(vec3(-bounds.x/2.0f,bounds.y/2.0f,bounds.z/2.0f));
    vert_pos.push_back(vec3(-bounds.x/2.0f,-bounds.y/2.0f,bounds.z/2.0f));
    vert_pos.push_back(vec3(-bounds.x/2.0f,-bounds.y/2.0f,-bounds.z/2.0f));
    vert_pos.push_back(vec3(-bounds.x/2.0f,bounds.y/2.0f,-bounds.z/2.0f));
    idx.push_back(0);
    idx.push_back(1);
    idx.push_back(1);
    idx.push_back(2);
    idx.push_back(2);
    idx.push_back(3);
    idx.push_back(3);
    idx.push_back(0);
    idx.push_back(0);
    idx.push_back(4);
    idx.push_back(1);
    idx.push_back(5);
    idx.push_back(2);
    idx.push_back(6);
    idx.push_back(3);
    idx.push_back(7);
    idx.push_back(4);
    idx.push_back(5);
    idx.push_back(5);
    idx.push_back(6);
    idx.push_back(6);
    idx.push_back(7);
    idx.push_back(7);
    idx.push_back(4);

}

void Cube::draw() {

}
