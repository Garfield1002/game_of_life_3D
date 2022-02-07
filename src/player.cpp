#include "player.h"

Player::Player()
    : camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1, 0) {}

Player::~Player() {}