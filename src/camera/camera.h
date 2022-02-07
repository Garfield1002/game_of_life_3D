#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {
 private:
  glm::vec3 up_, right_;

  unsigned int *screen_width_, *screen_height;

  float yaw_, pitch_;

  float movement_speed_{2.5f};
  float mouse_sensitivity_{0.1f};

  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors() {
    glm::vec3 front(cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
                    sin(glm::radians(pitch_)),
                    sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)));
    front_ = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(
        front_,
        glm::vec3(0, 1, 0)));  // normalize the vectors, because their length
                               // gets closer to 0 the more you look up or down
                               // which results in slower movement.
    up_ = glm::normalize(glm::cross(right_, front_));
  };
  void updateCameraVectors_old() {
    // calculate the new Front vector
    glm::vec3 front(cos(pitch_) * sin(yaw_), sin(pitch_),
                    cos(pitch_) * cos(yaw_));

    // also re-calculate the Right and Up vector
    right_ = glm::vec3(sin(yaw_ - 3.14f / 2.0f), 0, cos(yaw_ - 3.14f / 2.0f));

    up_ = glm::cross(right_, front_);
  }

 public:
  float zoom_{45.0f};
  glm::vec3 position_, front_;
  glm::mat4 projection;

  // constructor with vectors
  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
      : position_{position},
        yaw_{yaw},
        pitch_{pitch},
        projection{glm::perspective(glm::radians(45.0f), 1920.0F / 1080.0F,
                                    0.1f, 100.0f)} {
    updateCameraVectors();
  }

  inline glm::mat4 view() const {
    return glm::lookAt(position_, position_ + front_, up_);
  }

  // processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movement_speed_ * deltaTime;
    switch (direction) {
      case FORWARD:
        position_ +=
            glm::vec3(cos(glm::radians(yaw_)), 0, sin(glm::radians(yaw_))) *
            velocity;
        break;
      case BACKWARD:
        position_ -=
            glm::vec3(cos(glm::radians(yaw_)), 0, sin(glm::radians(yaw_))) *
            velocity;
        break;
      case LEFT:
        position_ -= glm::vec3(cos(glm::radians(yaw_ + 90)), 0,
                               sin(glm::radians(yaw_ + 90))) *
                     velocity;
        break;
      case RIGHT:
        position_ += glm::vec3(cos(glm::radians(yaw_ + 90)), 0,
                               sin(glm::radians(yaw_ + 90))) *
                     velocity;
        break;
      case UP:
        position_ += glm::vec3(0, 1, 0) * velocity;
        break;
      case DOWN:
        position_ -= glm::vec3(0, 1, 0) * velocity;
        break;
      default:
        break;
    }
  }

  // processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset) {
    yaw_ += xoffset * mouse_sensitivity_;
    pitch_ += yoffset * mouse_sensitivity_;

    if (pitch_ > 80) pitch_ = 80;
    if (pitch_ < -80) pitch_ = -80;

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f) zoom_ = 1.0f;
    if (zoom_ > 45.0f) zoom_ = 45.0f;
  }
};
