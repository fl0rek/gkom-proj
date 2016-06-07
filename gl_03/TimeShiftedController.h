#pragma once
#include "Drawable.h"
class TimeShiftedController :
	public Drawable {
public:
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1)) = 0;
	virtual void setFrame(const float t);

	TimeShiftedController(Drawable* d, const float start, const float end);
	virtual ~TimeShiftedController();
};

