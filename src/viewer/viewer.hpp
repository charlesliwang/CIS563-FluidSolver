//
//  viewer.hpp
//  Thanda

#ifndef viewer_hpp
#define viewer_hpp

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../scene/scene.hpp"
#include "../camera/camera.hpp"

class Viewer {
    GLFWwindow* window;
private:
    Scene scene;
    Camera camera;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint VAO;
    mat4 mvp;
    GLuint MatrixID;
    vec3 target_0;
    vec3 right_0;
    vec3 up_0;
public:
    Viewer();
    vector<vec3> vertices;
    vector<GLuint> indices;
    vector<vec3> *particles;
    vector<vec4> *particle_colors;
    vector<GLuint> p_idx;
    int create_window();
};

#endif /* viewer_hpp */
