//
//  geom.hpp
//  Thanda

#ifndef geom_hpp
#define geom_hpp
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <vector>
using namespace glm;
using namespace std;

class Geometry {
private:
    vector<vec3> vert_pos;
public:
    Geometry();
    virtual bool collision(vec3 point) = 0;
    virtual void create() = 0;
    virtual void draw() = 0;
};

class Cube : Geometry {
private:
public:
    Cube();
    vec3 bounds;
    vector<vec3> vert_pos;
    vector<GLuint> idx;
    virtual bool collision(vec3 point);
    virtual void create();
    virtual void draw();
    void setBounds(vec3 bounds);
    vector<vec3> get_vert_pos();
};

#endif /* geom_hpp */
