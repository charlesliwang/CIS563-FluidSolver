//
//  camera.cpp
//  Thanda
//

#include "camera.hpp"

Camera::Camera(){}

Camera::Camera(float width, float height) {
    this->width = width;
    this->height = height;
    fovy = 45;
    near_clip = 0.0001;
    far_clip = 10000;
    zoom = 20;
    camera_target = vec3(0,0,0);
    phi = 0;
    theta = 0;
}

void Camera::update_camera() {
    camera_position = vec3(rotate(mat4(1.0f),theta,vec3(0,1.0f,0)) * rotate(mat4(1.0f),phi,vec3(1.0f,0,0))
            * translate(mat4(1.0f),vec3(0,0,zoom)) * vec4(camera_target,1.0f));
    camera_direction = camera_target - camera_position;
}

mat4 Camera::viewproj_matrix() {
    vec3 world_up;
    if(phi >90 && phi< 270) {
        world_up = vec3(0,-1,0);
    } else {
        world_up = vec3(0,1,0);
    }
    camera_right = normalize(cross(camera_direction,world_up));
    camera_up = normalize(cross(camera_right,camera_direction));
    return perspective(fovy, width / height, near_clip, far_clip)*lookAt(camera_position, camera_target, camera_up);
}

