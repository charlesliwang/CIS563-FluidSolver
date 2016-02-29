//
//  camera.hpp
//  Thanda
//

#ifndef camera_hpp
#define camera_hpp


// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera {
private:

public:
    Camera();
    vec3 camera_position;
    vec3 camera_target;
    vec3 camera_direction;
    vec3 camera_right;
    vec3 camera_up;
    GLfloat phi;
    GLfloat theta;
    GLfloat zoom;
    GLfloat fovy;
    GLfloat width;
    GLfloat height;
    GLfloat near_clip;
    GLfloat far_clip;
    Camera(float width, float height);
    void update_camera();
    mat4 viewproj_matrix();


};



#endif /* camera_hpp */
