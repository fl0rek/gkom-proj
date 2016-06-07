#pragma once
#include "Drawable.h"
#include "AnimationController.h"

class RotationController :
	public AnimationController {
public:
	RotationController(float angle, glm::vec3 axis);

	virtual void setFrame(const float t) final override;

	~RotationController();

private:
	float angle;
	glm::vec3 axis;

};

