#include <camera.hpp>

//private method
void Camera::_updateCameraVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

//public method
Camera::Camera(glm::vec3 position, glm::vec3 up,
           float yaw,float pitch) : _front(glm::vec3(0.0f, 0.0f, -1.0f)), 
                                    _movement_speed(SPEED),
                                    _mouse_sensitivity(SENSITIVITY),
                                    _zoom(ZOOM){
    _position = position;
     _worldUp = up;
    _yaw = yaw;
    _pitch = pitch;
    _updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, 
           float upX, float upY, float upZ,
           float yaw, float pitch) : _front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                     _movement_speed(SPEED),
                                     _mouse_sensitivity(SENSITIVITY),
                                     _zoom(ZOOM){
    _position = glm::vec3(posX, posY, posZ);
    _worldUp = glm::vec3(upX, upY, upZ);
    _yaw = yaw;
    _pitch = pitch;
    _updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(_position, _position + _front, _up);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime){
    float velocity = _movement_speed * deltaTime;
    if(direction == FORWARD){
        _position += _front * velocity;
    }
    if(direction == BACKWARD){
        _position -= _front * velocity;
    }
    if(direction == LEFT){
        _position -= _right * velocity;
    }
    if(direction == RIGHT){
        _position += _right * velocity;
    }
    if(direction == RAISE){
        _position.y += velocity;
    }
    if(direction == DROP){
        _position.y -= velocity;
    }
}
void Camera::processMouseMovement(float offsetX, float offsetY, GLboolean constrainPitch){
    offsetX *= _mouse_sensitivity;
    offsetY *= +_mouse_sensitivity;

    _yaw += offsetX;
    _pitch += offsetY;

    if(constrainPitch){
        if(_pitch > 89.0f){
            _pitch = 89.0f;
        }
        if(_pitch < -89.0f){
            _pitch = -89.0f;
        }
    }
    _updateCameraVectors();
}
void Camera::processMouseScroll(float offsetY){
    if(_zoom >= 1.0f && _zoom <= 45.0f){
        _zoom -= offsetY;
    }
    if(_zoom <= 1.0f){
        _zoom = 1.0f;
    }
    if(_zoom >= 45.0){
        _zoom = 45.0f;
    }
}

float Camera::getZoom(){
    return _zoom;
}
