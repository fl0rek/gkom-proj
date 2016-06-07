#include "RotationController.h"



RotationController::RotationController(float angle, glm::vec3 axis) :
	angle(angle), axis(axis) {
	this->setFrame(0.);
}

void RotationController::setFrame(const float t) {
	//AnimationController::setFrame(t);

	glm::mat4 r = glm::mat4(1);
	r = glm::rotate(r, this->angle * t, this->axis);
	this->transformation_current = r;
}

RotationController::~RotationController() { }
